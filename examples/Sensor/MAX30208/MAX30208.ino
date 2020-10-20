#pragma mark - Depend MAX30208_Library
/*
cd ~/Arduino/libraries
git clone https://github.com/lewisxhe/MAX30208_Library.git
*/

#include "config.h"
#include "MAX30208.h"

TTGOClass *ttgo;
TFT_eSPI *tft ;
MAX30208_Class sensor;

#define MAX30208_INT_PIN            4
#define MAX30208_SLAVE_ADDRESS      0x50

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    tft = ttgo->tft;

    ttgo->openBL();

    /*
    * The default address is 0x51, which conflicts with the TWATCH RTC chip.
    * Set MAX30208 INT to LOW and change the address to 0x50.
    * * */
    pinMode(MAX30208_INT_PIN, OUTPUT);
    digitalWrite(MAX30208_INT_PIN, LOW);

    if (sensor.begin(Wire, MAX30208_SLAVE_ADDRESS) == false) {
        Serial.println("Unable to communicate with MAX30208.");
        tft->setTextFont(2);
        tft->setTextColor(TFT_RED, TFT_BLACK);
        tft->setCursor(0, 0);
        tft->println("Unable to communicate with Sensor.");
        while (1) ;
    }

    // Turn on temperature conversion complete interrupt
    sensor.enableDATARDY();

    // Start the conversion
    sensor.startConvert();

    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextFont(4);
}


void loop(void)
{
    uint8_t mask = sensor.getINT();
    if (mask & MAX30208_INT_STATUS_TEMP_RDY) {
        sensor.check();
        float t = sensor.readTemperature();
        tft->fillRect(30, 90, 120, 80, TFT_BLACK);
        tft->setCursor(30, 90);
        tft->println("Temp: " + String(t) + "*C");
        Serial.printf("Temp:%.2f\n", t);

        //Start the next conversion
        sensor.startConvert();
    }
    delay(1000);
}




