
#include <WiFi.h>
#include "time.h"
#include "config.h"


TTGOClass *ttgo;
PCF8563_Class *rtc;
TFT_eSPI *tft;


const char *ssid            = "YOUR_SSID";
const char *password        = "YOUR_PASS";

const char *ntpServer       = "pool.ntp.org";
const long  gmtOffset_sec   = 3600;
const int   daylightOffset_sec = 3600;

bool rtcIrq = false;

void setup()
{
    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    ttgo->setBrightness(128);       // 0~255

    rtc = ttgo->rtc;

    tft = ttgo->tft;

    tft->setTextColor(TFT_GREEN, TFT_BLACK);

    //connect to WiFi
    tft->print("Connecting to ");
    tft->println(ssid);
    tft->println();

    Serial.printf("Connecting to %s \n", ssid);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        tft->print(".");
    }
    tft->println(" CONNECTED\n");
    Serial.println(" CONNECTED");

    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        tft->println("Failed to obtain time, Restart in 3 seconds");
        Serial.println("Failed to obtain time, Restart in 3 seconds");
        delay(3000);
        esp_restart();
        while (1);
    }
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    tft->println("Time synchronization succeeded");
    // Sync local time to external RTC
    rtc->syncToRtc();
    tft->setFreeFont(&FreeMonoOblique9pt7b);
}

void loop()
{
    tft->drawString(rtc->formatDateTime(), 15, 80, 7);
    tft->drawString(rtc->formatDateTime(PCF_TIMEFORMAT_MM_DD_YYYY), 60, 160);
    delay(1000);
}
