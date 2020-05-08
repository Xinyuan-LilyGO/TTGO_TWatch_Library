
// #define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition
#include <TTGO.h>

TTGOClass *ttgo;

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  15          /* Time ESP32 will go to sleep (in seconds) */

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->eTFT->fillScreen(rand() % 0xFFFF);
    delay(3000);
    ttgo->power->setPowerOutPut(0xFF, false);
    ttgo->displaySleep();
    esp_sleep_enable_timer_wakeup(uS_TO_S_FACTOR * TIME_TO_SLEEP);
    esp_deep_sleep_start();
}

void loop()
{
}



