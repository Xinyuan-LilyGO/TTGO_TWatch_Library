#include <TTGO.h>

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextColor(random(0xFFFF));
    if (!ttgo->bma->begin()) {
        ttgo->eTFT->drawString("BMA423 Init FAIL",  25, 50, 4);
        while (1);
    }

    ttgo->bma->enableAccel();

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->drawString("T-Watch BMA423",  25, 50, 4);
    ttgo->eTFT->setTextFont(4);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
}

void loop()
{
    Accel acc;
    bool res = ttgo->bma->getAccel(acc);
    if (res == false) {
        Serial.println("getAccel FAIL");
    } else {
        ttgo->eTFT->fillRect(98, 100, 70, 85, TFT_BLACK);
        ttgo->eTFT->setCursor(80, 100);
        ttgo->eTFT->print("X:"); ttgo->eTFT->println(acc.x);
        ttgo->eTFT->setCursor(80, 130);
        ttgo->eTFT->print("Y:"); ttgo->eTFT->println(acc.y);
        ttgo->eTFT->setCursor(80, 160);
        ttgo->eTFT->print("Z:"); ttgo->eTFT->println(acc.z);
    }
    delay(100);
}
