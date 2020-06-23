// #define LILYGO_TWATCH_2020_V1        // If you are using T-Watch-2020 version, please open this macro definition

#include <TTGO.h>

TTGOClass *ttgo;

lv_obj_t *pos = nullptr;
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
    if (ttgo->touch->touched()) {
        TP_Point p =  ttgo->touch->getPoint();
        sprintf(buf, "x:%03d  y:%03d", p.x, p.y);
        ttgo->tft->drawString(buf, 80, 118);
    }
    delay(5);
}
