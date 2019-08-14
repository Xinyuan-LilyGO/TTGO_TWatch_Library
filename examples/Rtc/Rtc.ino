
#include <TTGO.h>

TTGOClass *ttgo;

char buf[128];
bool rtcIrq = false;

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    
    ttgo->eTFT->fillScreen(TFT_BLACK);

    pinMode(RTC_INT, INPUT_PULLUP);
    attachInterrupt(RTC_INT, [] {
        rtcIrq = 1;
    }, FALLING);

    ttgo->rtc->disableAlarm();

    ttgo->rtc->setDateTime(2019, 8, 12, 15, 0, 53);

    ttgo->rtc->setAlarmByMinutes(1);

    ttgo->rtc->enableAlarm();
}

void loop()
{
    ttgo->eTFT->setTextColor(random(0xFFFF));
    ttgo->eTFT->drawString("T-Watch RTC",  50, 50, 4);

    ttgo->eTFT->setTextColor(TFT_YELLOW, TFT_BLACK);
    snprintf(buf, sizeof(buf), "%s", ttgo->rtc->formatDateTime());
    ttgo->eTFT->drawString(buf, 5, 118, 7);
    if (rtcIrq) {
        rtcIrq = 0;
        detachInterrupt(RTC_INT);
        ttgo->rtc->resetAlarm();
        int i = 3;
        while (i--) {
            ttgo->eTFT->fillScreen(TFT_RED);
            ttgo->eTFT->setTextColor(TFT_WHITE, TFT_RED);
            ttgo->eTFT->drawString("RTC Alarm", 60, 118, 4);
            delay(500);
            ttgo->eTFT->fillScreen(TFT_BLACK);
            ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
            ttgo->eTFT->drawString("RTC Alarm", 60, 118, 4);
            delay(500);
        }
    }
    delay(1000);
}
