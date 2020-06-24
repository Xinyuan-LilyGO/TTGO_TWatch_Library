#pragma mark - Depend SparkFun_MAX3010x_Sensor_Library
/*
cd ~/Arduino/libraries
git clone https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library
*/

#include "config.h"
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
#include "spo2_algorithm.h"


TTGOClass *ttgo;
MAX30105 particleSensor;

const uint8_t RATE_SIZE = 20; //Increase this for more averaging.
uint8_t rates[RATE_SIZE]; //Array of heart rates
uint8_t rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
int prevBPM = 0;


void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing...");
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextFont(4);
    ttgo->tft->setCursor(0, 0);

    // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) { //Use default I2C port, 400kHz speed
        ttgo->tft->setCursor(0, 0);
        ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
        ttgo->tft->println("Sensor was not found.");
        Serial.println("Sensor was not found. Please check wiring/power. ");
        while (1);
    }
    ttgo->tft->drawCentreString("T-Watch HeartRate",  120, 60, 2);
    particleSensor.setup();
    particleSensor.setPulseAmplitudeRed(0xFF);
    particleSensor.setPulseAmplitudeGreen(0);
    Serial.println("Place your index finger on the sensor with steady pressure.");
}

void loop()
{
    particleSensor.check(); //Check the sensor, read up to 3 samples
    while (particleSensor.available()) { //do we have new data?
        long irValue  = particleSensor.getFIFOIR();

        if (irValue < 50000) {
            return;
        }

        if (checkForBeat(irValue) == true) {
            //We sensed a beat!
            long delta = millis() - lastBeat;
            lastBeat = millis();

            beatsPerMinute = 60 / (delta / 1000.0);

            if (beatsPerMinute < 255 && beatsPerMinute > 20) {
                rates[rateSpot++] = (uint8_t)beatsPerMinute; //Store this reading in the array
                rateSpot %= RATE_SIZE; //Wrap variable

                //Take average of readings
                beatAvg = 0;
                for (uint8_t x = 0 ; x < RATE_SIZE ; x++)
                    beatAvg += rates[x];
                beatAvg /= RATE_SIZE;
            }
        }

        if (prevBPM != beatAvg) {
            prevBPM = beatAvg;
            ttgo->tft->setTextColor(TFT_GREEN);
            static char buffer[256];
            ttgo->tft->fillRect(0, 120, 240, 30, TFT_RED);
            snprintf(buffer, sizeof(buffer), "Avg BPM:%d", beatAvg);
            ttgo->tft->drawCentreString(buffer, 240 / 2, 120, 2);
        }
        particleSensor.nextSample(); //We're finished with this sample so move to next sample
    }
}

