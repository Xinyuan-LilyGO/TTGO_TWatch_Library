
#include <TTGO.h>

TTGOClass *ttgo;

lv_obj_t *pos = nullptr;
char buf[128];

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextFont(2);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->eTFT->drawString("T-Watch Touch Test", 62, 90);
}

void loop()
{
    if (ttgo->touch->touched()) {
        TP_Point p =  ttgo->touch->getPoint();
        sprintf(buf, "x:%03d  y:%03d", p.x, p.y);
        ttgo->eTFT->drawString(buf, 80, 118);
    }
    delay(5);
}
