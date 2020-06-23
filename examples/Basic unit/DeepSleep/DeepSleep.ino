
// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH       // To use T-Watch2019 Not touchscreen , please uncomment this line
//#define LILYGO_WATCH_2020_V1              // To use T-Watch2020 , please uncomment this line
// #define LILYGO_WATCH_BLOCK               // To use T-Watch Block , please uncomment this line

#include <lilyGoWatch.h>

TTGOClass *ttgo;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15          /* Time ESP32 will go to sleep (in seconds) */

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->fillScreen(rand() % 0xFFFF);
    delay(3000);
    ttgo->power->setPowerOutPut(0xFF, false);
    ttgo->displaySleep();
    esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);
    esp_deep_sleep_start();
}

void loop()
{
}



