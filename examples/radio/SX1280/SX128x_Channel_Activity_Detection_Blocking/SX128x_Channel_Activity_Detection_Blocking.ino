/*
  RadioLib SX128x Blocking Channel Activity Detection Example

  This example uses SX1280 to scan the current LoRa
  channel and detect ongoing LoRa transmissions.

  Other modules from SX128x family can also be used.

  Using blocking CAD is not recommended, as it will lead
  to significant amount of timeouts, inefficient use of processor
  time and can some miss packets!
  Instead, interrupt CAD is recommended.

  For default module settings, see the wiki page
  https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

  For full API reference, see the GitHub Pages
  https://jgromes.github.io/RadioLib/
*/

#include <LilyGoLib.h>
#include <LV_Helper.h>

SX1280 radio = newModule();

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
}

void loop()
{
    Serial.print(F("[SX1280] Scanning channel for LoRa transmission ... "));

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
    delay(100);
}
