#include <TTGO.h>

TTGOClass *ttgo;

char buf[128];
bool irq = false;

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    pinMode(BMA423_INT1, INPUT);
    attachInterrupt(BMA423_INT1, [] {
        irq = 1;
    }, RISING);

    ttgo->bma->begin();
    ttgo->bma->attachInterrupt();

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextColor(random(0xFF10));
    ttgo->eTFT->drawString("T-Watch BMA423",  25, 50, 4);


}

void loop()
{
    if (irq) {
        irq = 0;
        bool  rlst;
        do {
            rlst =  ttgo->bma->readInterrupt();
        } while (!rlst);

        if (ttgo->bma->isStepCounter()) {
            Serial.println(ttgo->bma->getCounter());
            ttgo->eTFT->setTextColor(random(0xFFFF), TFT_BLACK);
            snprintf(buf, sizeof(buf), "StepCount: %u", ttgo->bma->getCounter());
            ttgo->eTFT->drawString(buf, 45, 118, 4);
        }
    }
    delay(20);
}
