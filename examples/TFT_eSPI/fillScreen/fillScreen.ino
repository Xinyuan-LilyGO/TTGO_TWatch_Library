// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH          // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_2020_V1             //To use T-Watch2020, please uncomment this line


#include <LilyGoWatch.h>

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
}

void loop()
{
    Serial.println("R");
    ttgo->tft->fillScreen(TFT_RED);
    delay(1000);
    Serial.println("G");
    ttgo->tft->fillScreen(TFT_GREEN);
    delay(1000);
    Serial.println("B");
    ttgo->tft->fillScreen(TFT_BLUE);
    delay(1000);
}
