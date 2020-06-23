/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

// => Hardware select
// #define LILYGO_WATCH_2019_WITH_TOUCH     // To use T-Watch2019 with touchscreen, please uncomment this line
// #define LILYGO_WATCH_2019_NO_TOUCH          // To use T-Watch2019 Not touchscreen , please uncomment this line
// #define LILYGO_WATCH_2020_V1             //To use T-Watch2020, please uncomment this line

#include <LilyGoWatch.h>

TTGOClass *ttgo;

void setup(void)
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
}

void loop()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    // Draw some random filled elipses
    for (int i = 0; i < 20; i++) {
        int rx = random(40);
        int ry = random(40);
        int x = rx + random(160 - rx - rx);
        int y = ry + random(128 - ry - ry);
        ttgo->tft->fillEllipse(x, y, rx, ry, random(0xFFFF));
    }

    delay(2000);
    ttgo->tft->fillScreen(TFT_BLACK);
    // Draw some random outline elipses
    for (int i = 0; i < 20; i++) {
        int rx = random(40);
        int ry = random(40);
        int x = rx + random(160 - rx - rx);
        int y = ry + random(128 - ry - ry);
        ttgo->tft->drawEllipse(x, y, rx, ry, random(0xFFFF));
    }
    delay(2000);
}



