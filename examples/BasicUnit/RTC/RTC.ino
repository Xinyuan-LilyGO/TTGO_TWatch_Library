
#include "config.h"

TTGOClass *ttgo;

char buf[128];
bool rtcIrq = false;

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    pinMode(RTC_INT_PIN, INPUT_PULLUP);
    attachInterrupt(RTC_INT_PIN, [] {
        rtcIrq = 1;
    }, FALLING);

    ttgo->rtc->disableAlarm();

    ttgo->rtc->setDateTime(2019, 8, 12, 15, 0, 53);

    ttgo->rtc->setAlarmByMinutes(1);

    ttgo->rtc->enableAlarm();
}

void loop()
{
    ttgo->tft->setTextColor(random(0xFFFF));
    ttgo->tft->drawString("T-Watch RTC",  50, 50, 4);

    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->tft->drawString(buf, 5, 118, 7);
    if (rtcIrq) {
        rtcIrq = 0;
        detachInterrupt(RTC_INT_PIN);
        ttgo->rtc->resetAlarm();
        int i = 3;
        while (i--) {
            ttgo->tft->fillScreen(TFT_RED);
            ttgo->tft->setTextColor(TFT_WHITE, TFT_RED);
            ttgo->tft->drawString("RTC Alarm", 60, 118, 4);
            delay(500);
            ttgo->tft->fillScreen(TFT_BLACK);
            ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
            ttgo->tft->drawString("RTC Alarm", 60, 118, 4);
            delay(500);
        }
    }
    delay(1000);
}
