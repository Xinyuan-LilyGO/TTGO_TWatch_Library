/**************************************************************************/
/*!
    @file     readMifareClassic.ino
    @author   Adafruit Industries
    @license  BSD (see license.txt)

    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.

    If the card has a 4-byte UID it is probably a Mifare
    Classic card, and the following steps are taken:

    Reads the 4 byte (32 bit) ID of a MiFare Classic card.
    Since the classic cards have only 32 bit identifiers you can stick
    them in a single variable and use that to compare card ID's as a
    number. This doesn't work for ultralight cards that have longer 7
    byte IDs!

    Note that you need the baud rate to be 115200 because we need to
    print out the data and read from the card at the same time!

This is an example sketch for the Adafruit PN532 NFC/RFID breakout boards
This library works with the Adafruit NFC breakout
  ----> https://www.adafruit.com/products/364

Check out the links above for our tutorials and wiring diagrams
These chips use SPI to communicate, 4 required to interface

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
    ttgo->tft->println("readMifareClassic example!");
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
    uint8_t success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
    uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

    if (success) {
        // Display some basic information about the card
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("  UID Value: ");
        nfc->PrintHex(uid, uidLength);

        if (uidLength == 4) {
            // We probably have a Mifare Classic card ...
            uint32_t cardid = uid[0];
            cardid <<= 8;
            cardid |= uid[1];
            cardid <<= 8;
            cardid |= uid[2];
            cardid <<= 8;
            cardid |= uid[3];
            Serial.print("Seems to be a Mifare Classic card #");
            Serial.println(cardid);
        }
        Serial.println("");
    }
}

