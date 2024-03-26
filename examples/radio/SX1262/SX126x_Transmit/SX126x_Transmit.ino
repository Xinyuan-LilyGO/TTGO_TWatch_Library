/*
   RadioLib SX126x Transmit Example

   This example transmits packets using SX1262 LoRa watch module.
   Each packet contains up to 256 bytes of data, in the form of:
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

uint32_t interval = 0;
lv_obj_t *label1;

void setup()
{
    Serial.begin(115200);

    watch.begin();

    // set carrier frequency to 433.5 MHz
    if (radio.setFrequency(433.5) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
        while (true);
    }

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

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "SX126x_Transmit");
    lv_obj_center(label1);
}

void loop()
{
    if (millis() > interval) {
        interval = millis() + 1000;
        Serial.print(F("[SX1262] Transmitting packet ... "));

        // you can transmit C-string or Arduino string up to
        // 256 characters long
        // NOTE: transmit() is a blocking method!
        //       See example SX126x_Transmit_Interrupt for details
        //       on non-blocking transmission method.
        int state = radio.transmit("Hello World!");

        // you can also transmit byte array up to 256 bytes long
        /*
          byte byteArr[] = {0x01, 0x23, 0x45, 0x56, 0x78, 0xAB, 0xCD, 0xEF};
          int state = radio.transmit(byteArr, 8);
        */

        if (state == RADIOLIB_ERR_NONE) {
            // the packet was successfully transmitted
            Serial.println(F("success!"));

            // print measured data rate
            Serial.print(F("[SX1262] Datarate:\t"));
            Serial.print(radio.getDataRate());
            Serial.println(F(" bps"));

            lv_label_set_text_fmt(label1, "[%u]Transmit PASS\nDatarate:%.2f bps", millis() / 1000, radio.getDataRate());

        } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
            // the supplied packet was longer than 256 bytes
            Serial.println(F("too long!"));

        } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
            // timeout occured while transmitting packet
            Serial.println(F("timeout!"));

        } else {
            lv_label_set_text_fmt(label1, "Transmit failed,code:%d", state);
            // some other error occurred
            Serial.print(F("failed, code "));
            Serial.println(state);
        }
    }

    lv_task_handler();
}
