/**************************************************************************/
/*!
    @file     iso14443a_uid.ino
    @author   Adafruit Industries
    @license  BSD (see license.txt)

    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.

    Note that you need the baud rate to be 115200 because we need to print
    out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout
  ----> https://www.adafruit.com/products/364

Check out the links above for our tutorials and wiring diagrams
These chips use SPI or I2C to communicate.

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

*/
/**************************************************************************/

#include "config.h"

TTGOClass       *ttgo;
Adafruit_PN532  *nfc;




void setup(void)
{
    Serial.begin(115200);
    while (!Serial) delay(10); // for Leonardo/Micro/Zero

    Serial.println("Looking for PN532...");

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

#ifdef LILYGO_WATCH_2019_WITH_TOUCH
    ttgo->openBL();
    ttgo->tft->println("iso14443a_uid example!");
    ttgo->tft->println("All messages will be output to the serial port, please open the terminal to view ");
#endif

    ttgo->enableLDO3();

    ttgo->nfc_begin();

    nfc = ttgo->nfc;

    uint32_t versiondata = nfc->getFirmwareVersion();
    if (! versiondata) {
        Serial.print("Didn't find PN53x board");
        while (1); // halt
    }

    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    // configure board to read RFID tags
    nfc->SAMConfig();

}


void loop(void)
{
    boolean success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

    if (success) {
        Serial.println("Found a card!");
        Serial.print("UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("UID Value: ");
        for (uint8_t i = 0; i < uidLength; i++) {
            Serial.print(" 0x"); Serial.print(uid[i], HEX);
        }
        Serial.println("");
        // Wait 1 second before continuing
        delay(1000);
    } else {
        // PN532 probably timed out waiting for a card
        Serial.println("Timed out waiting for a card");
    }
}

