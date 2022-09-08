
#define SDA 21
#define SCL 22
#define I2C_DEV_ADDR 0x55

#include <String>
#include "Wire.h"
#include "Arduino.h"
#include "config.h"
#include <WiFi.h>

TTGOClass *ttgo = nullptr;
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

    Serial.setDebugOutput(true);
    Wire.begin(SDA, SCL);

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

int last = 0;
int i = 0;
char key_ch = ' ';
bool isOn = false;
void loop()
{

    //Read  bytes from the slave
    Wire.requestFrom(I2C_DEV_ADDR, 1);
    while (Wire.available() > 0) {
        key_ch = Wire.read();
        if (key_ch == (char)0x00) {
            return;
        }
        Serial.print("c:");
        Serial.println((int)key_ch);
    }

    if (key_ch == 0x0D) {
        if (keyValue != "") {
            if (keyValue == "scan") {
                tft->println();
                wifi_scan();
            } else if (keyValue == "help") {
                tft->println();
                tft->println("1.scan");
                tft->println("2.connect [index] [password]");
                tft->println("[Alt + C]:Clear Screen");
                tft->println("[Alt + B]:Turn ON/OFF backlight");
                keyValue.clear();
                printTitle();

            } else if (keyValue.startsWith("connect")) {
                String password = "";
                String ssid = "";
                tft->println();
                tft->print(keyValue);
                int index = keyValue.indexOf(' ');
                int indexend = keyValue.lastIndexOf(' ');

                if (index >= 0) {
                    ssid = keyValue.substring(index + 1, indexend);

                } else {
                    tft->setTextColor(TFT_RED);
                    tft->println("Invaild SSID");
                    tft->setTextColor(TFT_WHITE);
                    keyValue.clear();
                    return;
                }
                if (indexend >= 0) {
                    password = keyValue.substring(indexend + 1);
                }

                if (index >= 0 && password != "") {
                    // ssid = WiFi.SSID(index);
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
                    WiFi.mode(WIFI_STA);
                    WiFi.disconnect();
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
    }

    else if (key_ch == 0x08) {
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
    }

    else if (key_ch == 0x0C) {
        tft->fillScreen(TFT_BLACK);
        tft->setCursor(0, 0);
        printTitle();
        keyValue.clear();
    } else {
        int16_t x = tft->getCursorX();
        int16_t w = tft->textWidth(String(key_ch));
        if (x + w > 239) {
            // return;
            tft->println();
        }
        keyValue +=  key_ch;
        Serial.print("keyValue:");
        Serial.println(keyValue);
        tft->print(key_ch);
    }


}

