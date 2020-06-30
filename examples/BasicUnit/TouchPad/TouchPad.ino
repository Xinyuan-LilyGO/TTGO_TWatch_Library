#include "config.h"

TTGOClass *ttgo;

char buf[128];

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->drawString("T-Watch Touch Test", 62, 90);
}

void loop()
{
    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        sprintf(buf, "x:%03d  y:%03d", x, y);
        ttgo->tft->drawString(buf, 80, 118);
    }
    delay(5);
}
