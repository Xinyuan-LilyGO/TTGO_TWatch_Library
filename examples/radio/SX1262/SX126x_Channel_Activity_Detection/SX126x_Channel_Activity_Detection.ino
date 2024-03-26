/*
   RadioLib SX126x Channel Activity Detection Example

   This example uses SX1262 to scan the current LoRa
   channel and detect ongoing LoRa transmissions.
   Unlike SX127x CAD, SX126x can detect any part
   of LoRa transmission, not just the preamble.

   Other modules from SX126x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

#include <LilyGoLib.h>
#include <LV_Helper.h>

SX1262 radio = newModule();

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    // initialize SX1262 with default settings
    Serial.print(F("[SX1262] Initializing ... "));
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

    // set carrier frequency to 433.5 MHz
    if (radio.setFrequency(433.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

}

void loop()
{
    Serial.print(F("[SX1262] Scanning channel for LoRa transmission ... "));

    // start scanning current channel
    int state = radio.scanChannel();

    if (state == RADIOLIB_LORA_DETECTED) {
        // LoRa preamble was detected
        Serial.println(F("detected!"));

    } else if (state == RADIOLIB_CHANNEL_FREE) {
        // no preamble was detected, channel is free
        Serial.println(F("channel is free!"));

    } else {
        // some other error occurred
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    // wait 100 ms before new scan
    lv_task_handler();
    delay(5);
}
