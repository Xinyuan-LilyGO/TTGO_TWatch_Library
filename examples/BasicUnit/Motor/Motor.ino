
#include "config.h"

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    //! begin motor attach to 33 pin , In TWatch-2020 it is IO4
    ttgo->motor_begin();

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->drawString("Please touch screen",  50, 100, 2);

    //attach touch screen interrupt pin
    pinMode(TP_INT, INPUT);
}

void loop()
{
    if (digitalRead(TP_INT) == LOW) {
        ttgo->motor->onec();
        delay(200);
    }
}
