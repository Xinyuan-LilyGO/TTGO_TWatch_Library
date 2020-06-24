#include "config.h"

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
