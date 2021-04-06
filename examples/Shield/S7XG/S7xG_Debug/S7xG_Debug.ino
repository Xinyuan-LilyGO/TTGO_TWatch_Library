#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft ;

#define S7XGPort Serial1

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    //! Create a new pointer to save the display object
    tft = ttgo->tft;

    tft->setTextFont(2);
    tft->println(" S7XG Debug Test");

    ttgo->enableLDO4();     //S7XG GPS VDD
    ttgo->enableLDO3();     //S7XG VDD

    S7XGPort.begin(115200, SERIAL_8N1, GPS_RX, GPS_TX);

}

// Command datasheet: https://github.com/Xinyuan-LilyGO/LilyGo-HAL/tree/master/ACSIP_S7678G

void loop(void)
{
    while (S7XGPort.available()) {
        Serial.write(S7XGPort.read());
    }
    while (Serial.available()) {
        S7XGPort.write(Serial.read());
    }
}


