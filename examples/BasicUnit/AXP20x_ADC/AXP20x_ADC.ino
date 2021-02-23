#include "config.h"

TTGOClass *ttgo;
AXP20X_Class *power;


#if  !defined(LILYGO_WATCH_BLOCK) || defined(LILYGO_GC9A01A_MODULE)

TFT_eSPI *tft =  nullptr;

void display_setup()
{
    // Turn on the backlight
    ttgo->openBL();

    //  Receive as a local variable for easy writing
    tft = ttgo->tft;
    // Some display setting
    tft->setTextFont(2);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
}

void display_show()
{
    // A simple clear screen will flash a bit
    tft->fillRect(0, 0, 210, 130, TFT_BLACK);
    tft->setCursor(0, 0);
    tft->print("VBUS STATUS: ");
    // You can use isVBUSPlug to check whether the USB connection is normal
    if (power->isVBUSPlug()) {
        tft->println("CONNECT");
        // Get USB voltage
        tft->print("VBUS Voltage:");
        tft->print(power->getVbusVoltage());
        tft->println(" mV");
        // Get USB current
        tft->print("VBUS Current: ");
        tft->print(power->getVbusCurrent());
        tft->println(" mA");
    } else {

        tft->setTextColor(TFT_RED, TFT_BLACK);
        tft->println("DISCONNECT");
        tft->setTextColor(TFT_GREEN, TFT_BLACK);
    }

    tft->println();

    tft->print("BATTERY STATUS: ");
    // You can use isBatteryConnect() to check whether the battery is connected properly
    if (power->isBatteryConnect()) {
        tft->println("CONNECT");

        // Get battery voltage
        tft->print("BAT Voltage:");
        tft->print(power->getBattVoltage());
        tft->println(" mV");

        // To display the charging status, you must first discharge the battery,
        // and it is impossible to read the full charge when it is fully charged
        if (power->isChargeing()) {
            tft->print("Charge:");
            tft->print(power->getBattChargeCurrent());
            tft->println(" mA");
        } else {
            // Show current consumption
            tft->print("Discharge:");
            tft->print(power->getBattDischargeCurrent());
            tft->println(" mA");
            tft->print("Per: ");
            tft->print(power->getBattPercentage());
            tft->println(" %");

        }
    } else {
        tft->setTextColor(TFT_RED, TFT_BLACK);
        tft->println("DISCONNECT");
        tft->setTextColor(TFT_GREEN, TFT_BLACK);
    }
}


#else

GxEPD_Class *ePaper = nullptr;

void display_setup()
{
    ePaper = ttgo->ePaper;
    ePaper->setFont(&FreeSans9pt7b);
}

void display_show()
{
    ePaper->fillScreen(GxEPD_WHITE);
    ePaper->setCursor(0, 15);
    ePaper->print("VBUS STATUS: ");
    // You can use isVBUSPlug to check whether the USB connection is normal
    if (power->isVBUSPlug()) {
        ePaper->println("CONNECT");
        // Get USB voltage
        ePaper->print("VBUS Voltage:");
        ePaper->print(power->getVbusVoltage());
        ePaper->println(" mV");
        // Get USB current
        ePaper->print("VBUS Current: ");
        ePaper->print(power->getVbusCurrent());
        ePaper->println(" mA");
    } else {
        ePaper->println("DISCONNECT");
    }


    ePaper->print("BATTERY STATUS: ");
    // You can use isBatteryConnect() to check whether the battery is connected properly
    if (power->isBatteryConnect()) {
        ePaper->println("CONNECT");

        // Get battery voltage
        ePaper->print("BAT Voltage:");
        ePaper->print(power->getBattVoltage());
        ePaper->println(" mV");

        // To display the charging status, you must first discharge the battery,
        // and it is impossible to read the full charge when it is fully charged
        if (power->isChargeing()) {
            ePaper->print("Charge:");
            ePaper->print(power->getBattChargeCurrent());
            ePaper->println(" mA");
        } else {
            // Show current consumption
            ePaper->print("Discharge:");
            ePaper->print(power->getBattDischargeCurrent());
            ePaper->println(" mA");
            ePaper->print("Per: ");
            ePaper->print(power->getBattPercentage());
            ePaper->println(" %");
        }
    } else {
        ePaper->println("DISCONNECT");
    }
    ePaper->update();
}
#endif


void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    power = ttgo->power;

    display_setup();

    // ADC monitoring must be enabled to use the AXP202 monitoring function
    power->adc1Enable(
        AXP202_VBUS_VOL_ADC1 |
        AXP202_VBUS_CUR_ADC1 |
        AXP202_BATT_CUR_ADC1 |
        AXP202_BATT_VOL_ADC1,
        true);

}

void loop()
{
    display_show();
    delay(1000);
}



