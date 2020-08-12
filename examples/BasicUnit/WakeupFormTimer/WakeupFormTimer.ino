/*
 * WakeupFormTimer: Use esp32 internal timer to wake T-Watch regularly
 * Copyright 2020 Lewis he
 */

#include "config.h"

TTGOClass *watch;
TFT_eSPI *tft;
AXP20X_Class *power;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15          /* Time ESP32 will go to sleep (in seconds) */

void setup()
{
    // Get TTGOClass instance
    watch = TTGOClass::getWatch();

    // Initialize the hardware, the BMA423 sensor has been initialized internally
    watch->begin();

    // Turn on the backlight
    watch->openBL();

    //Receive objects for easy writing
    tft = watch->tft;
    power = watch->power;

    tft->setTextColor(TFT_GREEN);
    tft->println("Timer wake-up example");

    for (int i = 5; i > 0; i--) {
        tft->print("Go to sleep after ");
        tft->print(i);
        tft->println(" seconds");
        delay(1000);
    }
    tft->println("Sleep now ...");
    delay(1000);

    // Set screen and touch to sleep mode
    watch->displaySleep();

    /*
    When using T - Watch2020V1, you can directly call power->powerOff(),
    if you use the 2019 version of TWatch, choose to turn off
    according to the power you need to turn off
    */
#ifdef LILYGO_WATCH_2020_V1
    watch->powerOff();
    // LDO2 is used to power the display, and LDO2 can be turned off if needed
    // power->setPowerOutPut(AXP202_LDO2, false);
#else
    power->setPowerOutPut(AXP202_LDO3, false);
    power->setPowerOutPut(AXP202_LDO4, false);
    power->setPowerOutPut(AXP202_LDO2, false);
    // The following power channels are not used
    power->setPowerOutPut(AXP202_EXTEN, false);
    power->setPowerOutPut(AXP202_DCDC2, false);
#endif


    esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);

    esp_deep_sleep_start();
}

void loop()
{
}



