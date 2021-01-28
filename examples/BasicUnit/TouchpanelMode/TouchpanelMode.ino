/*
These are three different modes for testing the touch panel.
Please note: Only TWATCH 2020-V2/2020-V3 can wake up from sleep,
and the others do not support wake up from sleep.
The only way to wake up is to restart the device.

Copyright 2021 Lewis he

*/


#include "config.h"


enum {
    TOUCH_ACTIVITY_MODE,
    TOUCH_MONITOR_MODE,
    TOUCH_SLEEP_MODE,
};

TTGOClass *watch = nullptr;
TFT_eSPI  *tft   = nullptr;

int16_t x = 0,
        y = 0;
char    buf[64];
bool    pmuIrq = false;
uint8_t status = TOUCH_ACTIVITY_MODE;

void setFlag(void)
{
    pmuIrq = true;
}

void setup()
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->openBL();

    tft = watch->tft;

    tft->fillScreen(TFT_BLACK);

    tft->setTextFont(2);

    tft->setCursor(0, 0);

    tft->println("Touch panel test ");

    watch->powerAttachInterrupt(setFlag);

    // Enable PEK button interrupt
    watch->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);

    //!Clear IRQ unprocessed  first
    watch->power->clearIRQ();

    // Use touch panel interrupt as digital input
    pinMode(TOUCH_INT, INPUT);

}

void loop()
{

    // When entering sleep mode, here will never be printed
    if (digitalRead(TOUCH_INT) == LOW) {
        Serial.println("pressed..");
    }

    // Print touch coordinates to the screen
    if (watch->getTouch(x, y)) {
        snprintf(buf, 64, "X:%03d Y:%03d", x, y);
        tft->drawCentreString(buf, 120, 120, 2);
    }

    if (pmuIrq) {
        pmuIrq = false;
        watch->power->readIRQ();

        // Press the PEK button to switch between different modes
        if (watch->power->isPEKShortPressIRQ()) {
            tft->fillScreen(TFT_BLACK);
            tft->setCursor(0, 0);
            switch (status) {
            case TOUCH_ACTIVITY_MODE:
                tft->println("set Touch in Monitor");
                watch->touchToMonitor();
                break;
            case TOUCH_MONITOR_MODE:
                tft->println("set Touch in Sleep");
#if !defined(LILYGO_WATCH_2020_V2) && !defined(LILYGO_WATCH_2020_V3)
                tft->println("If you use LilyPi/2019/2020-V1, the touch panel will not wake up, the only way to wake it up is to restart the device ");
#endif
                watch->touchToSleep();
                break;
#if defined(LILYGO_WATCH_2020_V2) || defined(LILYGO_WATCH_2020_V3)
            case TOUCH_SLEEP_MODE:
                tft->println("Wakeup Touch panel");
                watch->touchWakup();
                break;
#endif
            default:
                break;
            }
            status++;
            status %= 3;
        }
        watch->power->clearIRQ();
    }
    delay(5);
}
