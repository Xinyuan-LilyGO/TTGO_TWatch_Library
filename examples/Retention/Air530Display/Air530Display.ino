/*
 * Air530Display.ino: Example of displaying Air530 GPS information on the screen
 * Copyright 2020 Lewis he
 */
// Only supports 2020 V2 version, other versions do not support
// Only supports 2020 V2 version, other versions do not support
// Only supports 2020 V2 version, other versions do not support

#include "config.h"

TTGOClass *ttgo = nullptr;
TFT_eSPI *tft = nullptr;
Air530 *gps = nullptr;

TFT_eSprite *eSpLoaction = nullptr;
TFT_eSprite *eSpDate = nullptr;
TFT_eSprite *eSpTime = nullptr;
TFT_eSprite *eSpSpeed = nullptr;
TFT_eSprite *eSpSatellites = nullptr;

uint32_t last = 0;
uint32_t updateTimeout = 0;

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();
    //Create a new pointer to save the display object
    tft = ttgo->tft;

    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(2);
    tft->println("Begin GPS Module...");

    //Open gps power
    ttgo->trunOnGPS();

    gps = ttgo->gps_begin();

    // Display on the screen, latitude and longitude, number of satellites, and date and time

    eSpLoaction   = new TFT_eSprite(tft); // Sprite object for eSpLoaction
    eSpDate   = new TFT_eSprite(tft); // Sprite object for eSpDate
    eSpTime   = new TFT_eSprite(tft); // Sprite object for eSpTime
    eSpSatellites   = new TFT_eSprite(tft); // Sprite object for eSpSatellites

    eSpLoaction->createSprite(240, 48);
    eSpLoaction->setTextFont(2);

    eSpDate->createSprite(240, 48);
    eSpDate->setTextFont(2);

    eSpTime->createSprite(240, 48);
    eSpTime->setTextFont(2);

    eSpSatellites->createSprite(240, 48);
    eSpSatellites->setTextFont(2);

    last = millis();
}


void loop(void)
{
    gps->process();

    if (gps->location.isUpdated()) {
        updateTimeout = millis();
        eSpLoaction->fillSprite(TFT_BLACK);
        eSpLoaction->setTextColor(TFT_GREEN, TFT_BLACK);
        eSpLoaction->setCursor(0, 0);
        eSpLoaction->print("LOCATION ");
        eSpLoaction->print("Fix Age=");
        eSpLoaction->println(gps->location.age());
        eSpLoaction->print("Lat= ");
        eSpLoaction->print(gps->location.lat(), 6);
        eSpLoaction->print(" Long= ");
        eSpLoaction->print(gps->location.lng(), 6);
        eSpLoaction->pushSprite(0, 0);

        Serial.print(F("LOCATION   Fix Age="));
        Serial.print(gps->location.age());
        Serial.print(F("ms Raw Lat="));
        Serial.print(gps->location.rawLat().negative ? "-" : "+");
        Serial.print(gps->location.rawLat().deg);
        Serial.print("[+");
        Serial.print(gps->location.rawLat().billionths);
        Serial.print(F(" billionths],  Raw Long="));
        Serial.print(gps->location.rawLng().negative ? "-" : "+");
        Serial.print(gps->location.rawLng().deg);
        Serial.print("[+");
        Serial.print(gps->location.rawLng().billionths);
        Serial.print(F(" billionths],  Lat="));
        Serial.print(gps->location.lat(), 6);
        Serial.print(F(" Long="));
        Serial.println(gps->location.lng(), 6);
    } else {
        if (millis() - updateTimeout > 3000) {
            updateTimeout = millis();

            eSpLoaction->fillSprite(TFT_BLACK);
            eSpLoaction->setTextColor(TFT_GREEN);
            eSpLoaction->setCursor(0, 0);
            eSpLoaction->print("LOCATION ");
            eSpLoaction->setTextColor(TFT_RED);
            eSpLoaction->print("INVAILD");
            eSpLoaction->pushSprite(0, 0);
        }
    }



    if (gps->date.isUpdated()) {
        eSpDate->setTextColor(TFT_GREEN, TFT_BLACK);
        eSpDate->fillSprite(TFT_BLACK);
        eSpDate->setCursor(0, 0);
        eSpDate->print("DATE Fix Age= ");
        eSpDate->println(gps->date.age());
        eSpDate->print("Year=");
        eSpDate->print(gps->date.year());
        eSpDate->print(" Month=");
        eSpDate->print(gps->date.month());
        eSpDate->print(" Day=");
        eSpDate->println(gps->date.day());
        eSpDate->pushSprite(0, 49);

        Serial.print(F("DATE Fix Age="));
        Serial.print(gps->date.age());
        Serial.print(F("ms Raw="));
        Serial.print(gps->date.value());
        Serial.print(F(" Year="));
        Serial.print(gps->date.year());
        Serial.print(F(" Month="));
        Serial.print(gps->date.month());
        Serial.print(F(" Day="));
        Serial.println(gps->date.day());
    }


    if (gps->time.isUpdated()) {
        eSpTime->setTextColor(TFT_GREEN, TFT_BLACK);
        eSpTime->fillSprite(TFT_BLACK);
        eSpTime->setCursor(0, 0);
        eSpTime->print("TIME Fix Age= ");
        eSpTime->println(gps->time.age());
        eSpTime->print("Hour=");
        eSpTime->print(gps->time.hour());
        eSpTime->print(" Minute=");
        eSpTime->print(gps->time.minute());
        eSpTime->print(" Second=");
        eSpTime->println(gps->time.second());
        eSpTime->pushSprite(0, 49 * 2);

        Serial.print(F("TIME Fix Age="));
        Serial.print(gps->time.age());
        Serial.print(F("ms Raw="));
        Serial.print(gps->time.value());
        Serial.print(F(" Hour="));
        Serial.print(gps->time.hour());
        Serial.print(F(" Minute="));
        Serial.print(gps->time.minute());
        Serial.print(F(" Second="));
        Serial.print(gps->time.second());
        Serial.print(F(" Hundredths="));
        Serial.println(gps->time.centisecond());
    }

    if (gps->speed.isUpdated()) {
        Serial.print(F("SPEED      Fix Age="));
        Serial.print(gps->speed.age());
        Serial.print(F("ms Raw="));
        Serial.print(gps->speed.value());
        Serial.print(F(" Knots="));
        Serial.print(gps->speed.knots());
        Serial.print(F(" MPH="));
        Serial.print(gps->speed.mph());
        Serial.print(F(" m/s="));
        Serial.print(gps->speed.mps());
        Serial.print(F(" km/h="));
        Serial.println(gps->speed.kmph());
    }

    if (gps->altitude.isUpdated()) {
        Serial.print(F("ALTITUDE   Fix Age="));
        Serial.print(gps->altitude.age());
        Serial.print(F("ms Raw="));
        Serial.print(gps->altitude.value());
        Serial.print(F(" Meters="));
        Serial.print(gps->altitude.meters());
        Serial.print(F(" Miles="));
        Serial.print(gps->altitude.miles());
        Serial.print(F(" KM="));
        Serial.print(gps->altitude.kilometers());
        Serial.print(F(" Feet="));
        Serial.println(gps->altitude.feet());
    }


    if (gps->satellites.isUpdated()) {
        eSpSatellites->setTextColor(TFT_GREEN, TFT_BLACK);
        eSpSatellites->fillSprite(TFT_BLACK);
        eSpSatellites->setCursor(0, 0);
        eSpSatellites->print("SATELLITES Fix Age= ");
        eSpSatellites->print(gps->satellites.age());
        eSpSatellites->print("ms Value=");
        eSpSatellites->println(gps->satellites.value());

        Serial.print(F("SATELLITES Fix Age="));
        Serial.print(gps->satellites.age());
        Serial.print(F("ms Value="));
        Serial.println(gps->satellites.value());

        eSpSatellites->pushSprite(0, 49 * 3);
    }

    if (gps->hdop.isUpdated()) {
        Serial.print(F("HDOP       Fix Age="));
        Serial.print(gps->hdop.age());
        Serial.print(F("ms Value="));
        Serial.println(gps->hdop.value());
    }

    if (millis() - last > 5000) {
        Serial.println();
        if (gps->location.isValid()) {
            static const double LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
            double distanceToLondon =
                TinyGPSPlus::distanceBetween(
                    gps->location.lat(),
                    gps->location.lng(),
                    LONDON_LAT,
                    LONDON_LON);
            double courseToLondon =
                TinyGPSPlus::courseTo(
                    gps->location.lat(),
                    gps->location.lng(),
                    LONDON_LAT,
                    LONDON_LON);

            Serial.print(F("LONDON     Distance="));
            Serial.print(distanceToLondon / 1000, 6);
            Serial.print(F(" km Course-to="));
            Serial.print(courseToLondon, 6);
            Serial.print(F(" degrees ["));
            Serial.print(TinyGPSPlus::cardinal(courseToLondon));
            Serial.println(F("]"));
        }

        Serial.print(F("DIAGS      Chars="));
        Serial.print(gps->charsProcessed());
        Serial.print(F(" Sentences-with-Fix="));
        Serial.print(gps->sentencesWithFix());
        Serial.print(F(" Failed-checksum="));
        Serial.print(gps->failedChecksum());
        Serial.print(F(" Passed-checksum="));
        Serial.println(gps->passedChecksum());

        if (gps->charsProcessed() < 10) {
            Serial.println(F("WARNING: No GPS data.  Check wiring."));
            tft->fillScreen(TFT_BLACK);
            tft->setTextColor(TFT_RED, TFT_BLACK);
            tft->println("ERROR: No GPS data.  Check wiring.");
            while (1);
        }

        last = millis();
        Serial.println();
    }
}
