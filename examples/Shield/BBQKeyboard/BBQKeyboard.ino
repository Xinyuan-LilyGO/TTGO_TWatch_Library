#include "config.h"
#include <BBQ10Keyboard.h>      //Base on arturo182 arduino_bbq10kbd library https://github.com/arturo182/arduino_bbq10kbd
#include <WiFi.h>

TTGOClass *ttgo = nullptr;
BBQ10Keyboard keyboard;
TFT_eSPI *tft;
String title = "@LilyGo:";
String keyValue;
char buff[64];

uint8_t readBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t len)
{
    ttgo->readBytes(devAddress, regAddress, data, len);
    return 0;
}

uint8_t writeBytes(uint8_t devAddress, uint8_t regAddress, uint8_t *data, uint8_t len)
{
    ttgo->writeBytes(devAddress, regAddress, data, len);
    return 0;
}

void printTitle()
{
    tft->setTextColor(TFT_GREENYELLOW);
    tft->print(title);
    tft->setTextColor(TFT_WHITE);
}

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    tft = ttgo->tft;
    tft->fillScreen(TFT_BLACK);
    ttgo->openBL();
    tft->setTextFont(2);
    tft->setRotation(1);

    if (!ttgo->deviceProbe(BBQ10KEYBOARD_DEFAULT_ADDR)) {
        tft->setTextColor(TFT_RED);
        tft->println("Keyboard device not found");
        while (1);
    }

    //set keyboard read/write callback
    keyboard.begin(readBytes, writeBytes);

    keyboard.setBacklight(0.5f);
    printTitle();
}

void wifi_scan()
{
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int16_t n = WiFi.scanNetworks();
    tft->fillScreen(TFT_BLACK);
    if (n == 0) {
        tft->println("no networks found");
    } else {
        tft->setTextDatum(TL_DATUM);
        tft->setCursor(0, 0);
        for (int i = 0; i < n; ++i) {
            sprintf(buff,
                    "[%d]:%s(%d)",
                    i + 1,
                    WiFi.SSID(i).c_str(),
                    WiFi.RSSI(i));
            Serial.println(buff);
            tft->println(buff);
        }
    }
}


void loop()
{
    const int keyCount = keyboard.keyCount();
    if (keyCount == 0)
        return;

    const BBQ10Keyboard::KeyEvent key = keyboard.keyEvent();
    /*
    String state = "pressed";
    if (key.state == BBQ10Keyboard::StateLongPress)
        state = "held down";
    else if (key.state == BBQ10Keyboard::StateRelease)
        state = "released";
    */

    if (key.state == BBQ10Keyboard::StatePress) {
        static bool isOn = true;

        // pressing 'B' turns off the backlight and pressing Shift+b turns it on
        if (key.key == 'B') {
            if (isOn) {
                keyboard.setBacklight(0);
                isOn = false;
            } else {
                isOn = true;
                keyboard.setBacklight(1.0);
            }
        } else if (key.key == '\n') {
            if (keyValue != "") {
                // Serial.print("run -> "); Serial.println(keyValue);
                if (keyValue == "scan") {
                    tft->println();
                    wifi_scan();
                } else if (keyValue == "help") {
                    tft->println();
                    tft->println("1.scan");
                    tft->println("2.connect [index] [password]");
                    tft->println("[Shift + C]:Clear Screen");
                    tft->println("[Shift + B]:Turn ON/OFF backlight");
                    keyValue.clear();
                    printTitle();

                } else if (keyValue.startsWith("connect")) {
                    tft->println();
                    tft->print(keyValue);
                    int index = keyValue.indexOf(' ');
                    int indexend = keyValue.lastIndexOf(' ');

                    if (index >= 0) {
                        index = keyValue[index + 1];
                        index -= 49;
                    } else {
                        tft->setTextColor(TFT_RED);
                        tft->println("Invaild SSID");
                        tft->setTextColor(TFT_WHITE);
                        keyValue.clear();
                        return;
                    }

                    String password = "";
                    String ssid = "";
                    if (indexend >= 0) {
                        password = keyValue.substring(indexend + 1);
                    }

                    if (index >= 0 && password != "") {
                        ssid = WiFi.SSID(index);
                        if (ssid == "") {
                            tft->setTextColor(TFT_RED);
                            tft->println("SSID Missing ...");
                            tft->setTextColor(TFT_WHITE);
                            keyValue.clear();
                            return;
                        }
                        tft->print("Connect to ");
                        tft->println(ssid);
                        tft->println("Wait for result...");
                        WiFi.begin(ssid.c_str(), password.c_str());
                        if (WiFi.waitForConnectResult() != WL_CONNECTED) {
                            tft->setTextColor(TFT_RED);
                            tft->println("Connect Failed ...");
                            tft->setTextColor(TFT_WHITE);
                            keyValue.clear();
                            return;
                        }

                        tft->setTextColor(TFT_GREEN);
                        tft->println();
                        tft->print("Connect success,IP:");
                        tft->println(WiFi.localIP().toString());
                        tft->setTextColor(TFT_WHITE);
                    }
                } else {
                    tft->println();
                    tft->setTextColor(TFT_RED);
                    tft->print("error: ");
                    tft->setTextColor(TFT_WHITE);
                    tft->println("Unknow Command");
                    printTitle();
                }
                keyValue.clear();
            } else {
                tft->println();
                printTitle();
            }
        } else if (key.key == 0x08) {
            String ch ;
            if (keyValue.isEmpty() ) {
                return;
            }
            int index = keyValue.length() - 1;
            ch = keyValue[index];
            keyValue.remove(index);

            int16_t x = tft->getCursorX();
            int16_t y = tft->getCursorY();
            int16_t w = tft->textWidth(ch);
            int16_t h = tft->fontHeight();
            tft->setCursor(x - w, y);
            tft->fillRect(x - w, y, w, h, TFT_BLACK);
        } else {

            int16_t x = tft->getCursorX();
            int16_t w = tft->textWidth(String(key.key));
            if (x + w > 239) {
                return;
            }
            keyValue += key.key;
            tft->print(key.key);
        }
    } else if (key.state == BBQ10Keyboard::StateLongPress) {
        if (key.key == 'C') {
            tft->fillScreen(TFT_BLACK);
            tft->setCursor(0, 0);
            printTitle();
            keyValue.clear();
        }
    }
}
