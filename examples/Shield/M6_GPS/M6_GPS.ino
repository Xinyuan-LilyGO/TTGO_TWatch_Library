//! This example is an M8/M6 GPS backplane, not an S7XG backplane.

#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft ;
TinyGPSPlus *gps;

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    //! Create a new pointer to save the display object
    tft = ttgo->tft;

    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(2);
    tft->println("Begin ublox M8/M6 GPS Module...");

    //! Open gps power
    ttgo->enableLDO3();

    ttgo->gps_begin();
    //! Create a new pointer to save the gps object
    gps = ttgo->gps;

}

void displayInfo();

void loop(void)
{
    if (ttgo->gpsHandler()) {
        displayInfo();
        delay(1000);
    }
    if (millis() > 5000 && gps->charsProcessed() < 10) {
        Serial.println(F("No GPS detected: check wiring."));
        while (true);
    }
}

void displayInfo()
{
    tft->setCursor(0, 0);
    tft->fillRect(0, 0, 240, 30, TFT_BLACK);
    tft->print(F("Location: "));
    if (gps->location.isValid()) {
        tft->print(gps->location.lat(), 6);
        tft->print(F(","));
        tft->print(gps->location.lng(), 6);
    } else {
        tft->print(F("INVALID"));
    }
    tft->println();
    tft->print(F("Date/Time: "));
    if (gps->date.isValid()) {
        tft->print(gps->date.month());
        tft->print(F("/"));
        tft->print(gps->date.day());
        tft->print(F("/"));
        tft->print(gps->date.year());
    } else {
        tft->print(F("INVALID"));
    }

    tft->print(F(" "));
    if (gps->time.isValid()) {
        if (gps->time.hour() < 10) tft->print(F("0"));
        tft->print(gps->time.hour());
        tft->print(F(":"));
        if (gps->time.minute() < 10) tft->print(F("0"));
        tft->print(gps->time.minute());
        tft->print(F(":"));
        if (gps->time.second() < 10) tft->print(F("0"));
        tft->print(gps->time.second());
        tft->print(F("."));
        if (gps->time.centisecond() < 10) tft->print(F("0"));
        tft->print(gps->time.centisecond());
    } else {
        tft->print(F("INVALID"));
    }
    tft->println();
}