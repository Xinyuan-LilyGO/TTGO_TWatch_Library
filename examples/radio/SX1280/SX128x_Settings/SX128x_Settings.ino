/*
   RadioLib SX128x Settings Example

   This example shows how to change all the properties of LoRa transmission.
   RadioLib currently supports the following settings:
    - pins (SPI slave select, DIO1, DIO2, BUSY pin)
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - output power during transmission
    - CRC
    - preamble length

   Other modules from SX128x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

#include <LilyGoLib.h>
#include <LV_Helper.h>

SX1281 radio = newModule();

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    // initialize SX1280 with default settings
    Serial.print(F("[SX1280] Initializing ... "));
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }


    // you can also change the settings at runtime
    // and check if the configuration was changed successfully

    // set carrier frequency to 2410.5 MHz
    if (radio.setFrequency(2410.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

    // set bandwidth to 203.125 kHz
    if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true);
    }

    // set spreading factor to 10
    if (radio.setSpreadingFactor(10) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true);
    }

    // set coding rate to 6
    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true);
    }

    // set output power to -2 dBm
    if (radio.setOutputPower(-2) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
        while (true);
    }

    // set LoRa preamble length to 16 symbols (accepted range is 2 - 65535)
    if (radio.setPreambleLength(16) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.println(F("Selected preamble length is invalid for this module!"));
        while (true);
    }

    // disable CRC
    if (radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.println(F("Selected CRC is invalid for this module!"));
        while (true);
    }

    Serial.println(F("All settings succesfully changed!"));
}

void loop()
{
    // nothing here
}
