// An Arduino based framework for the Lilygo T-Watch 2020
// Much of the code is based on the sample apps for the
// T-watch that were written and copyrighted by Lewis He.
//(Copyright (c) 2019 lewis he)



// https://www.instructables.com/id/Lilygo-T-Watch-2020-Arduino-Framework/
// By DanGeiger


#include "config.h"
#include <soc/rtc.h>

TTGOClass *ttgo;

uint32_t targetTime = 0;       // for next 1 second display update
// uint32_t clockUpTime = 0;      // track the time the clock is displayed

uint8_t hh, mm, ss, mmonth, dday; // H, M, S variables
uint16_t yyear; // Year is 16 bit int

void setup()
{
    //initSetup();
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->tft->setTextFont(1);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
    //LVGL is not used, this line is not needed
    // ttgo->lvgl_begin();

    //Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    //Synchronize time to system time
    ttgo->rtc->syncToSystem();

    displayTime(true); // Our GUI to show the time
    ttgo->openBL(); // Turn on the backlight

}

void loop()
{

    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        displayTime(ss == 0); // Call every second but only update time every minute
    }

    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        while (ttgo->getTouch(x, y)) {} // wait for user to release

        // This is where the app selected from the menu is launched
        // If you add an app, follow the variable update instructions
        // at the beginning of the menu code and then add a case
        // statement on to this switch to call your paticular
        // app routine.

        switch (modeMenu()) { // Call modeMenu. The return is the desired app number
        case 0: // Zero is the clock, just exit the switch
            break;
        case 1:
            jSats();
            break;
        case 2:
            appAccel();
            break;
        case 3:
            appBattery();
            break;
        case 4:
            appTouch();
            break;
        case 5:
            appSetTime();
            break;
        }
        displayTime(true);
    }
}
