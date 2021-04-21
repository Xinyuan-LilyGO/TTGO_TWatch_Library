
// There is no User Button in Twatch-2020

#include "config.h"

TTGOClass *ttgo;

void pressed()
{
    uint16_t color = random(0xFFFF);
    ttgo->tft->fillScreen(color);
    ttgo->tft->setTextColor(color, TFT_WHITE);
    ttgo->tft->drawString("User Button pressed",  5, 100, 4);
}

void released()
{
    uint16_t color = random(0xFFFF);
    ttgo->tft->fillScreen(color);
    ttgo->tft->setTextColor(color, TFT_WHITE);
    ttgo->tft->drawString("User Button released",  5, 100, 4);
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextFont(4);
    ttgo->tft->drawString("User Button released",  5, 100, 4);

    ttgo->button->setPressedHandler(pressed);
    ttgo->button->setReleasedHandler(released);

#ifdef LILYGO_WATCH_2019_NO_TOUCH
    pinMode(TOUCH_INT, INPUT);
#endif
}

void loop()
{
#ifdef LILYGO_WATCH_2019_NO_TOUCH
    if (digitalRead(TOUCH_INT)) {
        Serial.println("TWatchN touch pressed");
    }
#endif
    ttgo->button->loop();
}
