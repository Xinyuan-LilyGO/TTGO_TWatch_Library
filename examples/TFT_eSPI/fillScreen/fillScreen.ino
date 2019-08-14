
#include <TTGO.h>

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
    ttgo->eTFT->fillScreen(TFT_RED);
    delay(1000);
    Serial.println("G");
    ttgo->eTFT->fillScreen(TFT_GREEN);
    delay(1000);
    Serial.println("B");
    ttgo->eTFT->fillScreen(TFT_BLUE);
    delay(1000);
}
