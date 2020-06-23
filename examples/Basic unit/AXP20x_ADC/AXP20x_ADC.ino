
#include <TTGO.h>

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->drawString("T-Watch AXP202",  25, 50, 4);
    ttgo->tft->setTextFont(4);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
}

void loop()
{
    float vbus_v = ttgo->power->getVbusVoltage();
    float vbus_c = ttgo->power->getVbusCurrent();
    float batt_v = ttgo->power->getBattVoltage();
    int per = ttgo->power->getBattPercentage();

    ttgo->tft->fillRect(85, 100, 145, 85, TFT_BLACK);
    ttgo->tft->setCursor(20, 100);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(20, 130);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_c); ttgo->tft->println(" mA");
    ttgo->tft->setCursor(20, 160);
    ttgo->tft->print("BATT: "); ttgo->tft->print(batt_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(20, 190);
    ttgo->tft->print("Per: "); ttgo->tft->print(per); ttgo->tft->println(" %");

    delay(1000);
}
