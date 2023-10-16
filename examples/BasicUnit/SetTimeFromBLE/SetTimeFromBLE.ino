/*

SetTimeFromBLE.ino : Simply use BLE to set the T-Watch RTC time
Copyright 2020 Lewis he

1. Download LightBlue on Google Play or Apple store
2. Connect the Bluetooth device named <LilyGo-Watch>
3. Slide to the bottom of the page, the attribute UUID is <beb5483e-36e1-4688-b7f5-ea07361b26a8>, then click it
4. Select <UTF-8 String> in the Data format of the page
5. Click the READ AGAIN button, it will return to the format requirements set for the first time,
    the default format is YY, MM, DD, H, M, S
6. Under <WRITTEN VALUES>, fill in the time you need to set, such as: 2020,08,07,11,20,30
7. Click the <WRITE> button on the right, the time will be written into the watch

Android : https://play.google.com/store/apps/details?id=com.punchthrough.lightblueexplorer&hl=en_US
Apple  : https://apps.apple.com/us/app/lightblue/id557428110

*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft;
PCF8563_Class *rtc;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t interval = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define TFT_GREY            0x5AEB


void drawSTATUS(bool status);

bool setDateTimeFormBLE(const char *str)
{
    uint16_t year;
    uint8_t month,  day, hour,  min,  sec;
    String temp, data;
    int r1, r2;
    if (str == NULL)return false;

    data = str;

    r1 = data.indexOf(',');
    if (r1 < 0)return false;
    temp = data.substring(0, r1);
    year = (uint16_t)temp.toInt();

    r1 += 1;
    r2 = data.indexOf(',', r1);
    if (r2 < 0)return false;
    temp = data.substring(r1, r2);
    month = (uint16_t)temp.toInt();

    r1 = r2 + 1;
    r2 = data.indexOf(',', r1);
    if (r2 < 0)return false;
    temp = data.substring(r1, r2);
    day = (uint16_t)temp.toInt();

    r1 = r2 + 1;
    r2 = data.indexOf(',', r1);
    if (r2 < 0)return false;
    temp = data.substring(r1, r2);
    hour = (uint16_t)temp.toInt();

    r1 = r2 + 1;
    r2 = data.indexOf(',', r1);
    if (r2 < 0)return false;
    temp = data.substring(r1, r2);
    min = (uint16_t)temp.toInt();

    r1 = r2 + 1;
    temp = data.substring(r1);
    sec = (uint16_t)temp.toInt();

    // No parameter check, please set the correct time
    Serial.printf("SET:%u/%u/%u %u:%u:%u\n", year, month, day, hour, min, sec);
    rtc->setDateTime(year, month, day, hour, min, sec);

    return true;
}

class MyCallbacks: public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
        std::string value = pCharacteristic->getValue();

        if (value.length() > 0) {
            Serial.println("*********");
            Serial.print("New value: ");
            value.c_str();
            for (int i = 0; i < value.length(); i++)
                Serial.print(value[i]);
            Serial.println();
            Serial.println("*********");
        }

        if (value.length() <= 0) {
            return;
        }
        if (!setDateTimeFormBLE(value.c_str())) {
            Serial.println("DateTime format error ...");
        }
    }
};

class MyServerCallback : public BLEServerCallbacks
{
    void onConnect(BLEServer *pServer)
    {
        deviceConnected = true;
        Serial.println("onConnect");
    }

    void onDisconnect(BLEServer *pServer)
    {
        deviceConnected = false;
        Serial.println("onDisconnect");
    }
};

void setupBLE(void)
{

    BLEDevice::init("LilyGo-Watch");
    BLEServer *pServer = BLEDevice::createServer();

    pServer->setCallbacks(new MyServerCallback);

    BLEService *pService = pServer->createService(SERVICE_UUID);

    BLECharacteristic *pCharacteristic = pService->createCharacteristic(
            CHARACTERISTIC_UUID,
            BLECharacteristic::PROPERTY_READ |
            BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic->setCallbacks(new MyCallbacks());

    pCharacteristic->setValue("Format: YY,MM,DD,h,m,s");
    pService->start();

    BLEAdvertising *pAdvertising = pServer->getAdvertising();
    pAdvertising->start();
}

void setup()
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("1. Download <LightBlue> on Google Play");
    Serial.println("2. Connect the Bluetooth device named <LilyGo-Watch >");
    Serial.println("3. Slide to the bottom of the page, the attribute UUID is <beb5483e-36e1-4688-b7f5-ea07361b26a8>, then click it");
    Serial.println("4. Select < UTF-8 String > in the Data format of the page");
    Serial.println("5. Click the READ AGAIN button, it will return to the format requirements set for the first time,the default format is YY, MM, DD, H, M, S");
    Serial.println("6. Under <WRITTEN VALUES>, fill in the time you need to set, such as : 2020, 08, 07, 11, 20, 30");
    Serial.println("7. Click the <WRITE> button on the right, the time will be written into the watch");
    Serial.println();

    //Get watch instance
    ttgo = TTGOClass::getWatch();
    // Initialize the hardware
    ttgo->begin();
    // Turn on the backlight
    ttgo->openBL();
    //  Receive as a local variable for easy writing
    rtc = ttgo->rtc;
    tft = ttgo->tft;


    // Time check will be done, if the time is incorrect, it will be set to compile time
    rtc->check();

    // Some settings of BLE
    setupBLE();

    // Draw initial connection status
    drawSTATUS(false);
}

void drawSTATUS(bool status)
{
    String str = status ? "Connection" : "Disconnect";
    int16_t cW = tft->textWidth("Connection", 2);
    int16_t dW = tft->textWidth("Disconnect", 2);
    int16_t w = cW > dW ? cW : dW;
    w += 6;
    int16_t x = 160;
    int16_t y = 20;
    int16_t h = tft->fontHeight(2) + 4;
    uint16_t col = status ? TFT_GREEN : TFT_GREY;
    tft->fillRoundRect(x, y, w, h, 3, col);
    tft->setTextColor(TFT_BLACK, col);
    tft->setTextFont(2);
    tft->drawString(str, x + 2, y);
}


void loop()
{
    // disconnected
    if (!deviceConnected && oldDeviceConnected) {
        oldDeviceConnected = deviceConnected;
        Serial.println("Draw deviceDisconnected");
        drawSTATUS(false);
    }

    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
        Serial.println("Draw deviceConnected");
        drawSTATUS(true);
    }

    if (millis() - interval > 1000) {

        interval = millis();

        tft->setTextColor(TFT_YELLOW, TFT_BLACK);

        tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_DD_MM_YYYY), 50, 200, 4);

        tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_HMS), 5, 118, 7);
    }
}
