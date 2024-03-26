/*
   RadioLib SX126x Transmit with Interrupts Example

   This example transmits LoRa packets with one second delays
   between them. Each packet contains up to 256 bytes
   of data, in the form of:
    - Arduino String
    - null-terminated char array (C-string)
    - arbitrary binary data (byte array)

   Other modules from SX126x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx126x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/

#include <LilyGoLib.h>
#include <LV_Helper.h>

SX1262 radio = newModule();

// save transmission state between loops
int transmissionState = RADIOLIB_ERR_NONE;
lv_obj_t *label1;

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
    // when packet transmission is finished
    radio.setDio1Action(setFlag);

    // start transmitting the first packet
    Serial.print(F("[SX1262] Sending first packet ... "));

    // you can transmit C-string or Arduino string up to
    // 256 characters long
    transmissionState = radio.startTransmit("Hello World!");

    // you can also transmit byte array up to 256 bytes long
    /*
      byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                        0x89, 0xAB, 0xCD, 0xEF};
      state = radio.startTransmit(byteArr, 8);
    */
    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "SX126x_Transmit_Interrupt");
    lv_obj_center(label1);
}

// flag to indicate that a packet was sent
volatile bool transmittedFlag = false;

// this function is called when a complete packet
// is transmitted by the module
// IMPORTANT: this function MUST be 'void' type
//            and MUST NOT have any arguments!
ICACHE_RAM_ATTR void setFlag(void)
{
    // we sent a packet, set the flag
    transmittedFlag = true;
}

void loop()
{
    // check if the previous transmission finished
    if (transmittedFlag) {
        // reset flag
        transmittedFlag = false;

        if (transmissionState == RADIOLIB_ERR_NONE) {
            // packet was successfully sent
            Serial.println(F("transmission finished!"));

            // NOTE: when using interrupt-driven transmit method,
            //       it is not possible to automatically measure
            //       transmission data rate using getDataRate()
            lv_label_set_text_fmt(label1, "[%u]Transmit PASS\n", millis() / 1000);

        } else {
            Serial.print(F("failed, code "));
            Serial.println(transmissionState);
            lv_label_set_text_fmt(label1, "Transmit failed,code:%d", transmissionState);

        }

        // clean up after transmission is finished
        // this will ensure transmitter is disabled,
        // RF switch is powered down etc.
        radio.finishTransmit();

        // wait a second before transmitting again
        delay(1000);

        // send another one
        Serial.print(F("[SX1262] Sending another packet ... "));

        // you can transmit C-string or Arduino string up to
        // 256 characters long
        transmissionState = radio.startTransmit("Hello World!");

        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x67,
                            0x89, 0xAB, 0xCD, 0xEF};
          int state = radio.startTransmit(byteArr, 8);
        */
    }

    lv_task_handler();
    delay(5);
}
