
#include <TTGO.h>

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->drawString("T-Watch AXP202",  25, 50, 4);
    ttgo->eTFT->setTextFont(4);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
}

void loop()
{
    float vbus_v = ttgo->power->getVbusVoltage();
    float vbus_c = ttgo->power->getVbusCurrent();
    float batt_v = ttgo->power->getBattVoltage();
    ttgo->eTFT->fillRect(85, 100, 145, 85, TFT_BLACK);
    ttgo->eTFT->setCursor(20, 100);
    ttgo->eTFT->print("Vbus: "); ttgo->eTFT->print(vbus_v); ttgo->eTFT->println(" mV");
    ttgo->eTFT->setCursor(20, 130);
    ttgo->eTFT->print("Vbus: "); ttgo->eTFT->print(vbus_c); ttgo->eTFT->println(" mA");
    ttgo->eTFT->setCursor(20, 160);
    ttgo->eTFT->print("BATT: "); ttgo->eTFT->print(batt_v); ttgo->eTFT->println(" mV");
    delay(1000);
}
