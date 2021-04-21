/**************************************************************************/
/*!
    @file     iso14443as_target.ino
    @original Adafruit Industries
    @modified Salvador Mendoza(@Netxing)
    @license  BSD (see license.txt)

    This example will attempt to mimic an ISO14443A smart card
    and retrieve some basic information from a PoS or terminal,
    this can be used to establish a communication process.

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
    ttgo->tft->println("iso14443as_target example!");
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
    uint8_t apdubuffer[255] = {}, apdulen;
    uint8_t ppse[] = {0x8E, 0x6F, 0x23, 0x84, 0x0E, 0x32, 0x50, 0x41, 0x59, 0x2E, 0x53, 0x59, 0x53, 0x2E, 0x44, 0x44, 0x46, 0x30, 0x31, 0xA5, 0x11, 0xBF, 0x0C, 0x0E, 0x61, 0x0C, 0x4F, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x03, 0x10, 0x10, 0x87, 0x01, 0x01, 0x90, 0x00};
    nfc->AsTarget();
    success = nfc->getDataTarget(apdubuffer, &apdulen); //Read initial APDU
    if (apdulen > 0) {
        for (uint8_t i = 0; i < apdulen; i++) {
            Serial.print(" 0x"); Serial.print(apdubuffer[i], HEX);
        }
        Serial.println("");
    }
    nfc->setDataTarget(ppse, sizeof(ppse));   //Mimic a smart card response with a PPSE APDU
    nfc->getDataTarget(apdubuffer, &apdulen); //Read respond from the PoS or Terminal
    if (apdulen > 0) {
        for (uint8_t i = 0; i < apdulen; i++) {
            Serial.print(" 0x"); Serial.print(apdubuffer[i], HEX);
        }
        Serial.println("");
    }
    delay(1000);
}
