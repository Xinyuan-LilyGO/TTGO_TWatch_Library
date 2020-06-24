/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

#include "config.h"

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



