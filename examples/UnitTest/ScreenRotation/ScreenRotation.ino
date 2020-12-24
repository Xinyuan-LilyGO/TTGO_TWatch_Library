/**
 * * Screen selection and touch coordinate test
 */

#include "config.h"

TTGOClass *ttgo;
bool irq = false;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->drawString("T-Watch Rotation Test", 62, 90);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);

    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ |
                           AXP202_VBUS_REMOVED_IRQ |
                           AXP202_VBUS_CONNECT_IRQ |
                           AXP202_CHARGING_IRQ, true);
    ttgo->power->clearIRQ();
}

void loop()
{
    int16_t x, y;

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {
            ttgo->tft->setRotation(ttgo->tft->getRotation() + 1 % 4);
            ttgo->tft->fillScreen(TFT_BLACK);
            ttgo->tft->setCursor(0, 0);
            ttgo->tft->print("Rotation");
            ttgo->tft->println(ttgo->tft->getRotation());
            ttgo->tft->drawString("T-Watch Rotation Test", 62, 90);

        }
        ttgo->power->clearIRQ();
    }
    if (ttgo->getTouch(x, y)) {
        ttgo->tft->fillRect(120, 130, 100, 40, TFT_BLACK);
        ttgo->tft->setCursor(120, 130);
        ttgo->tft->print("X:");
        ttgo->tft->println(x);
        ttgo->tft->setCursor(120, ttgo->tft->getCursorY());
        ttgo->tft->print("Y:");
        ttgo->tft->print(y);
        Serial.printf("x:%d y:%d\n", x, y);
    }


    delay(5);
}
