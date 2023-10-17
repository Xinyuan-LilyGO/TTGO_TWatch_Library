// #pragma mark - Depend ESPAsyncWebServer & AsyncTCP &  ArduinoJson libraries
/**
 * Base on https://github.com/lewisxhe/TTGO-EPaper-Series
 *
 *   Adapted to TBlock by Lewis 09/11/2020
 * /

/*
cd ~/Arduino/libraries
git clone https://github.com/me-no-dev/ESPAsyncWebServer.git
git clone https://github.com/me-no-dev/AsyncTCP.git
git clone -b 6.x https://github.com/bblanchon/ArduinoJson.git
*/

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPmDNS.h>
#include <Wire.h>
#include "SD.h"
#include "SPI.h"
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "esp_wifi.h"
#include "Esp.h"
#include "config.h"

#define USE_AP_MODE


/*100 * 100 bmp fromat*/
//https://www.onlineconverter.com/jpg-to-bmp
#define BADGE_CONFIG_FILE_NAME  "/badge.data"
#define DEFALUT_AVATAR_BMP      "/avatar.bmp"
#define DEFALUT_QR_CODE_BMP     "/qr.bmp"
#define WIFI_SSID               "Put your wifi ssid"
#define WIFI_PASSWORD           "Put your wifi password"
#define DEFAULT_BRIGHTNESS                          45

#define ENABLE_GxEPD2_GFX           0

#define DEFALUT_FONT FreeMono9pt7b
#define FILESYSTEM   SPIFFS


TTGOClass       *twatch = nullptr;
PCF8563_Class   *rtc    = nullptr;
AXP20X_Class    *power  = nullptr;
Button2         *btn    = nullptr;

void showMianPage(void);
void showQrPage(void);
void drawBitmap(const char *filename, int16_t x, int16_t y, bool with_color);

typedef struct {
    char name[32];
    char link[64];
    char tel[64];
    char company[64];
    char email[64];
    char address[128];
} Badge_Info_t;

typedef enum {
    RIGHT_ALIGNMENT = 0,
    LEFT_ALIGNMENT,
    CENTER_ALIGNMENT,
} Text_alignment;

AsyncWebServer server(80);

Badge_Info_t info;
static const uint16_t input_buffer_pixels = 20;       // may affect performance
static const uint16_t max_palette_pixels = 256;       // for depth <= 8
uint8_t mono_palette_buffer[max_palette_pixels / 8];  // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w
uint8_t input_buffer[3 * input_buffer_pixels];        // up to depth 24
const char *path[2] = {DEFALUT_AVATAR_BMP, DEFALUT_QR_CODE_BMP};
bool pwIRQ = false;

void displayText(const String &str, int16_t y, uint8_t alignment)
{
    int16_t x = 0;
    int16_t x1, y1;
    uint16_t w, h;
    twatch->display.setCursor(x, y);
    twatch->display.getTextBounds(str, x, y, &x1, &y1, &w, &h);

    switch (alignment) {
    case RIGHT_ALIGNMENT:
        twatch->display.setCursor(twatch->display.width() - w - x1, y);
        break;
    case LEFT_ALIGNMENT:
        twatch->display.setCursor(0, y);
        break;
    case CENTER_ALIGNMENT:
        twatch->display.setCursor(twatch->display.width() / 2 - ((w + x1) / 2), y);
        break;
    default:
        break;
    }
    twatch->display.println(str);
}

void saveBadgeInfo(Badge_Info_t *info)
{
    // Open file for writing
    File file = FILESYSTEM.open(BADGE_CONFIG_FILE_NAME, FILE_WRITE);
    if (!file) {
        Serial.println(F("Failed to create file"));
        return;
    }
#if ARDUINOJSON_VERSION_MAJOR == 5
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
#elif ARDUINOJSON_VERSION_MAJOR == 6
    StaticJsonDocument<256> root;
#endif
    // Set the values
    root["company"] = info->company;
    root["name"] = info->name;
    root["address"] = info->address;
    root["email"] = info->email;
    root["link"] = info->link;
    root["tel"] = info->tel;

#if ARDUINOJSON_VERSION_MAJOR == 5
    if (root.printTo(file) == 0)
#elif ARDUINOJSON_VERSION_MAJOR == 6
    if (serializeJson(root, file) == 0)
#endif
    {
        Serial.println(F("Failed to write to file"));
    }
    // Close the file (File's destructor doesn't close the file)
    file.close();
}

void loadDefaultInfo(void)
{
    strlcpy(info.company, "XinYuan Elec.", sizeof(info.company));
    strlcpy(info.name, "LilyGo", sizeof(info.name));
    strlcpy(info.address, "ShenZhen", sizeof(info.address));
    strlcpy(info.email, "lily@lilygo.cc", sizeof(info.email));
    strlcpy(info.link, "www.lilygo.cn", sizeof(info.link));
    strlcpy(info.tel, "0755-83380665", sizeof(info.tel));
    saveBadgeInfo(&info);
}

bool loadBadgeInfo(Badge_Info_t *info)
{
    if (!FILESYSTEM.exists(BADGE_CONFIG_FILE_NAME)) {
        Serial.println("load configure fail");
        return false;
    }

    File file = FILESYSTEM.open(BADGE_CONFIG_FILE_NAME);
    if (!file) {
        Serial.println("Open Fial -->");
        return false;
    }

#if ARDUINOJSON_VERSION_MAJOR == 5
    StaticJsonBuffer<256> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(file);
    if (!root.success()) {
        Serial.println(F("Failed to read file, using default configuration"));
        file.close();
        return false;
    }
    root.printTo(Serial);
#elif ARDUINOJSON_VERSION_MAJOR == 6
    StaticJsonDocument<256> root;
    DeserializationError error = deserializeJson(root, file);
    if (error) {
        Serial.println(F("Failed to read file, using default configuration"));
        file.close();
        return false;
    }
#endif
    if ((const char *)root["company"] == NULL) {
        return false;
    }
    strlcpy(info->company, root["company"], sizeof(info->company));
    strlcpy(info->name, root["name"], sizeof(info->name));
    strlcpy(info->address, root["address"], sizeof(info->address));
    strlcpy(info->email, root["email"], sizeof(info->email));
    strlcpy(info->link, root["link"], sizeof(info->link));
    strlcpy(info->tel, root["tel"], sizeof(info->tel));
    file.close();
    return true;
}

void WebServerStart(void)
{

#ifdef USE_AP_MODE
    uint8_t mac[6];
    char apName[18] = {0};
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    sprintf(apName, "T-Badge-%02X:%02X", mac[4], mac[5]);


    if (!WiFi.softAP(apName)) {
        Serial.println("AP Config failed.");
        return;
    } else {

        Serial.print("MAC: ");
        Serial.println(WiFi.softAPmacAddress());
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("IP: ");
        Serial.println(myIP);
        Serial.printf("Please connect to the %s hotspot and use a browser to fill in the address in the address bar to access.\n", apName);
    }
#else
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.print(".");
        esp_restart();
    }
    Serial.println(F("WiFi connected"));
    Serial.println("");
    Serial.println(WiFi.localIP());
#endif

    if (MDNS.begin("ttgo")) {
        Serial.println("MDNS responder started");
    }

    server.serveStatic("/", FILESYSTEM, "/").setDefaultFile("index.html");

    server.on("css/main.css", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(FILESYSTEM, "css/main.css", "text/css");
    });
    server.on("js/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(FILESYSTEM, "js/jquery.min.js", "application/javascript");
    });
    server.on("js/tbdValidate.js", HTTP_GET, [](AsyncWebServerRequest * request) {
        request->send(FILESYSTEM, "js/tbdValidate.js", "application/javascript");
    });
    server.on("/data", HTTP_POST, [](AsyncWebServerRequest * request) {
        request->send(200, "text/plain", "");

        for (int i = 0; i < request->params(); i++) {
            String name = request->getParam(i)->name();
            String params = request->getParam(i)->value();
            Serial.println(name + " : " + params);
            if (name == "company") {
                strlcpy(info.company, params.c_str(), sizeof(info.company));
            } else if (name == "name") {
                strlcpy(info.name, params.c_str(), sizeof(info.name));
            } else if (name == "address") {
                strlcpy(info.address, params.c_str(), sizeof(info.address));
            } else if (name == "email") {
                strlcpy(info.email, params.c_str(), sizeof(info.email));
            } else if (name == "link") {
                strlcpy(info.link, params.c_str(), sizeof(info.link));
            } else if (name == "tel") {
                strlcpy(info.tel, params.c_str(), sizeof(info.tel));
            }
        }
        saveBadgeInfo(&info);
    });

    server.onFileUpload([](AsyncWebServerRequest * request, const String & filename, size_t index, uint8_t *data, size_t len, bool final) {
        static File file;
        static int pathIndex = 0;
        if (!index) {
            Serial.printf("UploadStart: %s\n", filename.c_str());
            file = FILESYSTEM.open(path[pathIndex], FILE_WRITE);
            if (!file) {
                Serial.println("Open FAIL");
                request->send(500, "text/plain", "hander error");
                return;
            }
        }
        if (file.write(data, len) != len) {
            Serial.println("Write fail");
            request->send(500, "text/plain", "hander error");
            file.close();
            return;
        }

        if (final) {
            Serial.printf("UploadEnd: %s (%u)\n", filename.c_str(), index + len);
            file.close();
            request->send(200, "text/plain", "");
            if (++pathIndex >= 2) {
                pathIndex = 0;
                showMianPage();
            }
        }
    });

    server.onNotFound([](AsyncWebServerRequest * request) {
        request->send(404, "text/plain", "Not found");
    });

    MDNS.addService("http", "tcp", 80);

    server.begin();
}

void showMianPage(void)
{
    drawBitmap(DEFALUT_AVATAR_BMP, 50, 5, true);
    twatch->display.setFont(&FreeMonoBold18pt7b);
    displayText(String(info.name), 120, CENTER_ALIGNMENT);
    twatch->display.setFont(&FreeMono9pt7b);
    displayText(String(info.company), 150, CENTER_ALIGNMENT);
    displayText(String(info.email), 170, CENTER_ALIGNMENT);
    displayText(String(info.link), 190, CENTER_ALIGNMENT);
    twatch->display.display(false);

}

void showQrPage(void)
{
    twatch->display.setFont(&FreeMono9pt7b);
    drawBitmap(DEFALUT_QR_CODE_BMP, 50, 5, true);
    displayText(String(info.address), 130, CENTER_ALIGNMENT);
    displayText(String(info.tel), 150, CENTER_ALIGNMENT);
    displayText(String(info.email), 170, CENTER_ALIGNMENT);
    twatch->display.display(false);
}

uint16_t read16(File &f)
{
    // BMP data is stored little-endian, same as Arduino.
    uint16_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read(); // MSB
    return result;
}

uint32_t read32(File &f)
{
    // BMP data is stored little-endian, same as Arduino.
    uint32_t result;
    ((uint8_t *)&result)[0] = f.read(); // LSB
    ((uint8_t *)&result)[1] = f.read();
    ((uint8_t *)&result)[2] = f.read();
    ((uint8_t *)&result)[3] = f.read(); // MSB
    return result;
}

void drawBitmap(const char *filename, int16_t x, int16_t y, bool with_color)
{
    File file;
    bool valid = false; // valid format to be handled
    bool flip = true;   // bitmap is stored bottom-to-top
    uint32_t startTime = millis();
    if ((x >= twatch->display.width()) || (y >= twatch->display.height()))
        return;
    Serial.println();
    Serial.print("Loading image '");
    Serial.print(filename);
    Serial.println('\'');

    file = FILESYSTEM.open(filename, FILE_READ);
    if (!file) {
        Serial.print("File not found");
        return;
    }

    // Parse BMP header
    if (read16(file) == 0x4D42) {
        // BMP signature
        uint32_t fileSize = read32(file);
        uint32_t creatorBytes = read32(file);
        uint32_t imageOffset = read32(file); // Start of image data
        uint32_t headerSize = read32(file);
        uint32_t width = read32(file);
        uint32_t height = read32(file);
        uint16_t planes = read16(file);
        uint16_t depth = read16(file); // bits per pixel
        uint32_t format = read32(file);
        if ((planes == 1) && ((format == 0) || (format == 3))) {
            // uncompressed is handled, 565 also
            Serial.print("File size: ");
            Serial.println(fileSize);
            Serial.print("Image Offset: ");
            Serial.println(imageOffset);
            Serial.print("Header size: ");
            Serial.println(headerSize);
            Serial.print("Bit Depth: ");
            Serial.println(depth);
            Serial.print("Image size: ");
            Serial.print(width);
            Serial.print('x');
            Serial.println(height);
            // BMP rows are padded (if needed) to 4-byte boundary
            uint32_t rowSize = (width * depth / 8 + 3) & ~3;
            if (depth < 8)
                rowSize = ((width * depth + 8 - depth) / 8 + 3) & ~3;
            if (height < 0) {
                height = -height;
                flip = false;
            }
            uint16_t w = width;
            uint16_t h = height;
            if ((x + w - 1) >= twatch->display.width())
                w = twatch->display.width() - x;
            if ((y + h - 1) >= twatch->display.height())
                h = twatch->display.height() - y;
            valid = true;
            uint8_t bitmask = 0xFF;
            uint8_t bitshift = 8 - depth;
            uint16_t red, green, blue;
            bool whitish, colored;
            if (depth == 1)
                with_color = false;
            if (depth <= 8) {
                if (depth < 8)
                    bitmask >>= depth;
                file.seek(54); //palette is always @ 54
                for (uint16_t pn = 0; pn < (1 << depth); pn++) {
                    blue = file.read();
                    green = file.read();
                    red = file.read();
                    file.read();
                    whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                    colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0));                                                  // reddish or yellowish?
                    if (0 == pn % 8)
                        mono_palette_buffer[pn / 8] = 0;
                    mono_palette_buffer[pn / 8] |= whitish << pn % 8;
                    if (0 == pn % 8)
                        color_palette_buffer[pn / 8] = 0;
                    color_palette_buffer[pn / 8] |= colored << pn % 8;
                }
            }
            twatch->display.fillScreen(GxEPD_WHITE);
            uint32_t rowPosition = flip ? imageOffset + (height - h) * rowSize : imageOffset;
            for (uint16_t row = 0; row < h; row++, rowPosition += rowSize) {
                // for each line
                uint32_t in_remain = rowSize;
                uint32_t in_idx = 0;
                uint32_t in_bytes = 0;
                uint8_t in_byte = 0; // for depth <= 8
                uint8_t in_bits = 0; // for depth <= 8
                uint16_t color = GxEPD_WHITE;
                file.seek(rowPosition);
                for (uint16_t col = 0; col < w; col++) {
                    // for each pixel
                    // Time to read more pixel data?
                    if (in_idx >= in_bytes) {
                        // ok, exact match for 24bit also (size IS multiple of 3)
                        in_bytes = file.read(input_buffer, in_remain > sizeof(input_buffer) ? sizeof(input_buffer) : in_remain);
                        in_remain -= in_bytes;
                        in_idx = 0;
                    }
                    switch (depth) {
                    case 24:
                        blue = input_buffer[in_idx++];
                        green = input_buffer[in_idx++];
                        red = input_buffer[in_idx++];
                        whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                        colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0));                                                  // reddish or yellowish?
                        break;
                    case 16: {
                        uint8_t lsb = input_buffer[in_idx++];
                        uint8_t msb = input_buffer[in_idx++];
                        if (format == 0) {
                            // 555
                            blue = (lsb & 0x1F) << 3;
                            green = ((msb & 0x03) << 6) | ((lsb & 0xE0) >> 2);
                            red = (msb & 0x7C) << 1;
                        } else {
                            // 565
                            blue = (lsb & 0x1F) << 3;
                            green = ((msb & 0x07) << 5) | ((lsb & 0xE0) >> 3);
                            red = (msb & 0xF8);
                        }
                        whitish = with_color ? ((red > 0x80) && (green > 0x80) && (blue > 0x80)) : ((red + green + blue) > 3 * 0x80); // whitish
                        colored = (red > 0xF0) || ((green > 0xF0) && (blue > 0xF0));                                                  // reddish or yellowish?
                    }
                    break;
                    case 1:
                    case 4:
                    case 8: {
                        if (0 == in_bits) {
                            in_byte = input_buffer[in_idx++];
                            in_bits = 8;
                        }
                        uint16_t pn = (in_byte >> bitshift) & bitmask;
                        whitish = mono_palette_buffer[pn / 8] & (0x1 << pn % 8);
                        colored = color_palette_buffer[pn / 8] & (0x1 << pn % 8);
                        in_byte <<= depth;
                        in_bits -= depth;
                    }
                    break;
                    }
                    if (whitish) {
                        color = GxEPD_WHITE;
                    } else if (colored && with_color) {
                        color = GxEPD_RED;
                    } else {
                        color = GxEPD_BLACK;
                    }
                    uint16_t yrow = y + (flip ? h - row - 1 : row);
                    twatch->display.drawPixel(x + col, yrow, color);
                } // end pixel
            }     // end line
            Serial.print("loaded in ");
            Serial.print(millis() - startTime);
            Serial.println(" ms");
        }
    }
    file.close();
    if (!valid) {
        Serial.println("bitmap format not handled.");
    }
}

static bool page = false;

void setup()
{
    Serial.begin(115200);

    // Reduce CPU frequency
    setCpuFrequencyMhz(80);

    if (!FILESYSTEM.begin()) {
        Serial.println("FILESYSTEM is not database");
        Serial.println("Please use Arduino ESP32 Sketch data Upload files");
        while (1) {
            delay(1000);
        }
    }

    // Get watch object
    twatch = TTGOClass::getWatch();

    twatch->begin();

#ifdef LILYGO_WATCH_HAS_BACKLIGHT
    // Turn on the backlight
    twatch->openBL();
#endif

    rtc = twatch->rtc;

    power = twatch->power;

    btn = twatch->button;

    twatch->display.setTextColor(GxEPD_BLACK);
    twatch->display.setFont(&DEFALUT_FONT);
    // Use compile time as RTC input time
    rtc->check();

    // Turn on power management button interrupt
    power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ | AXP202_PEK_LONGPRESS_IRQ, true);

    //Prohibit long-pressing the power button of the watch to shut down,
    //the shutdown action will be processed after long-pressing the power button
    power->setTimeOutShutdown(false);

    // Clear power interruption
    power->clearIRQ();

    // Set MPU6050 to sleep
    twatch->mpu->setSleepEnabled(true);

    // Set Pin to interrupt
    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        pwIRQ = true;
    }, FALLING);

    btn->setPressedHandler([]() {
        twatch->display.clearScreen();
        if (!page) {
            showQrPage();
        } else {
            showMianPage();
        }
        page = !page;
    });

#ifdef LILYGO_WATCH_HAS_BACKLIGHT
    // Adjust the backlight to reduce current consumption
    twatch->setBrightness(DEFAULT_BRIGHTNESS);
#endif

    if (!loadBadgeInfo(&info)) {
        loadDefaultInfo();
    }

    showMianPage();

    WebServerStart();



}

void loop()
{
#ifdef LILYGO_WATCH_HAS_TOUCH
    if (twatch->touched()) {
        twatch->display.clearScreen();
        if (!page) {
            showQrPage();
        } else {
            showMianPage();
        }
        page = !page;
    }
#endif
    if (pwIRQ) {
        pwIRQ = false;
        // Get interrupt status
        power->readIRQ();

        if (power->isPEKShortPressIRQ()) {
#ifdef LILYGO_WATCH_HAS_BACKLIGHT
            twatch->bl->isOn() ? twatch->bl->off() : twatch->bl->adjust(DEFAULT_BRIGHTNESS);
#endif
        } else if (power->isPEKLongPressIRQ()) {

            Serial.println("enter power off!!!!");
            /**
             * The long-press shutdown action is handled here.
             * Before turning off the power,need to set the e-paper to sleep,
             * otherwise the display of the e-pape will be distorted when the power is turned off.
             */
            twatch->display.hibernate();
            power->clearIRQ();
            delay(500);
            twatch->shutdown();
        }
        // After the interruption, you need to manually clear the interruption status
        power->clearIRQ();
    }
    btn->loop();

}