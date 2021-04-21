#include "config.h"

TTGOClass *ttgo;

char buf[128];
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
    ttgo->tft->drawString("T-Watch Touch Test", 62, 90);

    pinMode(AXP202_INT, INPUT_PULLUP);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);
    //!Clear IRQ unprocessed  first
    ttgo->power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ, true);
    ttgo->power->clearIRQ();

    pinMode(TOUCH_INT, INPUT);
}

void loop()
{
    int16_t x, y;
    if (digitalRead(TOUCH_INT) == LOW) {
        Serial.println("PRESSED");
        if (ttgo->getTouch(x, y)) {
            sprintf(buf, "x:%03d  y:%03d", x, y);
            ttgo->tft->drawString(buf, 80, 118);
        }
    }

    if (irq) {
        irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isPEKShortPressIRQ()) {
            // Clean power chip irq status
            ttgo->power->clearIRQ();

            // Set  touchscreen sleep
            ttgo->displaySleep();

            /*
            In TWatch2019/ Twatch2020V1, touch reset is not connected to ESP32,
            so it cannot be used. Set the touch to sleep,
            otherwise it will not be able to wake up.
            Only by turning off the power and powering on the touch again will the touch be working mode
            // ttgo->displayOff();
            */

            ttgo->powerOff();

            //Set all channel power off
            ttgo->power->setPowerOutPut(AXP202_LDO3, false);
            ttgo->power->setPowerOutPut(AXP202_LDO4, false);
            ttgo->power->setPowerOutPut(AXP202_LDO2, false);
            ttgo->power->setPowerOutPut(AXP202_EXTEN, false);
            ttgo->power->setPowerOutPut(AXP202_DCDC2, false);

            // TOUCH SCREEN  Wakeup source
            esp_sleep_enable_ext1_wakeup(GPIO_SEL_38, ESP_EXT1_WAKEUP_ALL_LOW);
            // PEK KEY  Wakeup source
            // esp_sleep_enable_ext1_wakeup(GPIO_SEL_35, ESP_EXT1_WAKEUP_ALL_LOW);
            esp_deep_sleep_start();
        }
        ttgo->power->clearIRQ();
    }
}
