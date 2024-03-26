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

// include the library
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

    // set the function that will be called
    // when LoRa packet or timeout is detected
    radio.setDio1Action(setFlag);

    // start scanning the channel
    Serial.print(F("[SX1262] Starting scan for LoRa preamble ... "));
    state = radio.startChannelScan();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }
}

// flag to indicate that a packet was detected or CAD timed out
volatile bool scanFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
ICACHE_RAM_ATTR void setFlag(void)
{
    // something happened, set the flag
    scanFlag = true;
}

void loop()
{
    // check if the flag is set
    if (scanFlag) {
        // reset flag
        scanFlag = false;

        // check CAD result
        int state = radio.getChannelScanResult();

        if (state == RADIOLIB_LORA_DETECTED) {
            // LoRa packet was detected
            Serial.println(F("[SX1262] Packet detected!"));

        } else if (state == RADIOLIB_CHANNEL_FREE) {
            // channel is free
            Serial.println(F("[SX1262] Channel is free!"));

        } else {
            // some other error occurred
            Serial.print(F("[SX1262] Failed, code "));
            Serial.println(state);

        }

        // start scanning the channel again
        Serial.print(F("[SX1262] Starting scan for LoRa preamble ... "));
        state = radio.startChannelScan();
        if (state == RADIOLIB_ERR_NONE) {
            Serial.println(F("success!"));
        } else {
            Serial.print(F("failed, code "));
            Serial.println(state);
        }
    }
    lv_task_handler();
    delay(5);
}
