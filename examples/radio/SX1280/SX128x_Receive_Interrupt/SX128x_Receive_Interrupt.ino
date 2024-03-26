/*
   RadioLib SX128x Receive with Interrupts Example

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   Other modules from SX128x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx128x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

#include <LilyGoLib.h>
#include <LV_Helper.h>

SX1280 radio = newModule();
lv_obj_t *label1;

void setup()
{
    setCpuFrequencyMhz(80);

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

    // set carrier frequency to 2410.5 MHz
    if (radio.setFrequency(2492.0) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

    // set bandwidth to 203.125 kHz
    if (radio.setBandwidth(203.125) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
        while (true);
    }

    // set spreading factor to 5
    if (radio.setSpreadingFactor(5) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
        while (true);
    }

    // set coding rate to 6
    if (radio.setCodingRate(6) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
        while (true);
    }

    // set output power to 13 dBm
    if (radio.setOutputPower(13) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
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

    // set the function that will be called
    // when new packet is received
    radio.setPacketReceivedAction(setFlag);

    // start listening for LoRa packets
    Serial.print(F("[SX1280] Starting to listen ... "));
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        while (true);
    }

    // if needed, 'listen' mode can be disabled by calling
    // any of the following methods:
    //
    // radio.standby()
    // radio.sleep()
    // radio.transmit();
    // radio.receive();
    // radio.readData();
    // radio.scanChannel();
    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "SX128x_Receive_Interrupt");
    lv_obj_set_height(label1, LV_PCT(50));
    lv_obj_center(label1);
}

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

// this function is called when a complete packet
// is received by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
#if defined(ESP8266) || defined(ESP32)
ICACHE_RAM_ATTR
#endif
void setFlag(void)
{
    // we got a packet, set the flag
    receivedFlag = true;
}

void loop()
{
    // check if the flag is set
    if (receivedFlag) {
        // reset flag
        receivedFlag = false;

        // you can read received data as an Arduino String
        String str;
        int state = radio.readData(str);

        // you can also read received data as byte array
        /*
          byte byteArr[8];
          int numBytes = radio.getPacketLength();
          int state = radio.readData(byteArr, numBytes);
        */

        if (state == RADIOLIB_ERR_NONE) {
            // packet was successfully received
            Serial.println(F("[SX1280] Received packet!"));

            // print data of the packet
            Serial.print(F("[SX1280] Data:\t\t"));
            Serial.println(str);

            // print RSSI (Received Signal Strength Indicator)
            Serial.print(F("[SX1280] RSSI:\t\t"));
            Serial.print(radio.getRSSI());
            Serial.println(F(" dBm"));

            // print SNR (Signal-to-Noise Ratio)
            Serial.print(F("[SX1280] SNR:\t\t"));
            Serial.print(radio.getSNR());
            Serial.println(F(" dB"));

            // print the Frequency Error
            // of the last received packet
            Serial.print(F("[SX1280] Frequency Error:\t"));
            Serial.print(radio.getFrequencyError());
            Serial.println(F(" Hz"));

            lv_label_set_text_fmt(label1, "[%u]Received:%s\nRSSI:%.2f\nSNR:%.2f\n",
                                  millis() / 1000,
                                  str.c_str(), radio.getRSSI(),
                                  radio.getSNR()
                                 );


        } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
            // packet was received, but is malformed
            Serial.println(F("CRC error!"));
            lv_label_set_text_fmt(label1, "[%u]Received CRC failed", millis() / 1000);
        } else {
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
            lv_label_set_text_fmt(label1, "[%u]Received %d failed", millis() / 1000, state);

        }

        // put module back to listen mode
        radio.startReceive();
    }
    lv_task_handler();
    delay(5);
}
