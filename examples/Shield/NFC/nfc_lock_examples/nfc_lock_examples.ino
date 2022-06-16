#include "config.h"
#include <Esp.h>

//! buzzer port to be controlled
#define BUZZER_PIN   13


TTGOClass *ttgo;
Adafruit_PN532 *nfc;
TFT_eSPI *tft ;

void setup(void)
{
    Serial.begin(115200);
    Serial.println("Hello!");

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    //! Create a new pointer to save the display object
    tft = ttgo->tft;

    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(2);
    tft->println("Begin PN532 Module...");

    //! Open nfc chip power
    ttgo->enableLDO3();

    pinMode(BUZZER_PIN, OUTPUT);
    ledcAttachPin(BUZZER_PIN, 7);
    ledcSetup(7, 1000, 16);

    ledcWriteTone(7, 1000);
    delay(200);
    ledcWriteTone(7, 0);


    //! Initialize the nfc module
    ttgo->nfc_begin();
    //! Create a new pointer to save the nfc object
    nfc = ttgo->nfc;

    uint32_t versiondata = nfc->getFirmwareVersion();
    if (! versiondata) {
        Serial.print("Didn't find PN53x board");
        tft->println("Didn't find PN53x board");
        int sec = 5;
        while (1) {
            if (sec < 0)esp_restart();
            tft->print("Will restart after ");
            tft->print(sec--);
            tft->println(" second");
            delay(1000);
        }
    }

    tft->println("Found chip PN5");
    // Got ok data, print it out!
    Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
    Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
    Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

    // configure board to read RFID tags
    nfc->SAMConfig();

    Serial.println("Waiting for an ISO14443A Card ...");
    tft->println("Waiting for an ISO14443A Card ...");

}

void PrintHexChar(const byte *data, const uint32_t numBytes)
{
    uint32_t szPos;
    for (szPos = 0; szPos < numBytes; szPos++) {
        // Append leading 0 for small values
        if (data[szPos] <= 0xF)
            tft->print(F("0"));
        tft->print(data[szPos], HEX);
        if ((numBytes > 1) && (szPos != numBytes - 1)) {
            tft->print(F(" "));
        }
    }
    tft->print(F("  "));
    for (szPos = 0; szPos < numBytes; szPos++) {
        if (data[szPos] <= 0x1F)
            tft->print(F("."));
        else
            tft->print((char)data[szPos]);
    }
    tft->println();
}


void PrintHex(const byte *data, const uint32_t numBytes)
{
    uint32_t szPos;
    for (szPos = 0; szPos < numBytes; szPos++) {
        tft->print(F("0x"));
        // Append leading 0 for small values
        if (data[szPos] <= 0xF)
            tft->print(F("0"));
        tft->print(data[szPos] & 0xff, HEX);
        if ((numBytes > 1) && (szPos != numBytes - 1)) {
            tft->print(F(" "));
        }
    }
    tft->println();
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

    tft->setCursor(0, 0);
    tft->fillScreen(TFT_BLACK);
    if (success) {
        tft->println("Found an ISO14443A card");
        tft->print("  UID Length: "); tft->print(uidLength, DEC); tft->println(" bytes");
        tft->print("  UID Value: ");

        // Display some basic information about the card
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: "); Serial.print(uidLength, DEC); Serial.println(" bytes");
        Serial.print("  UID Value: ");
        PrintHex(uid, uidLength);
        Serial.println("");

        if (uidLength == 4) {
            // We probably have a Mifare Classic card ...
            Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
            tft->println("Seems to be a Mifare Classic card (4 byte UID)");

            // Now we need to try to authenticate it for read/write access
            // Try with the factory default KeyA: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
            Serial.println("Trying to authenticate block 4 with default KEYA value");
            tft->println("Trying to authenticate block 4 with default KEYA value");
            uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
            // uint8_t keya[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };
            // Start with block 4 (the first block of sector 1) since sector 0
            // contains the manufacturer data and it's probably better just
            // to leave it alone unless you know what you're doing
            success = nfc->mifareclassic_AuthenticateBlock(uid, uidLength, 4, 0, keya);

            if (success) {

                ledcWriteTone(7, 1000);
                delay(200);
                ledcWriteTone(7, 0);


                Serial.println("Sector 1 (Blocks 4..7) has been authenticated");
                tft->println("Sector 1 (Blocks 4..7) has been authenticated");
                uint8_t data[16];

                // If you want to write something to block 4 to test with, uncomment
                // the following line and this text should be read back in a minute
                // memcpy(data, (const uint8_t[]) {
                //     'c', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0
                // }, sizeof data);
                // success = nfc->mifareclassic_WriteDataBlock (4, data);

                memset(data, 0, sizeof(data));

                // Try to read the contents of block 4
                success = nfc->mifareclassic_ReadDataBlock(4, data);

                //Compare information read
                int ret = memcmp(data, (const uint8_t[]) {
                    'a', 'd', 'a', 'f', 'r', 'u', 'i', 't', '.', 'c', 'o', 'm', 0, 0, 0, 0
                }, sizeof data);

                if (ret == 0) {
                    ledcWriteTone(7, 1000);
                    delay(200);
                    ledcWriteTone(7, 0);
                    ledcWriteTone(7, 1000);
                    delay(200);
                    ledcWriteTone(7, 0);
                } else {
                    Serial.println("vrefiy failed");
                }

                if (success) {
                    // Data seems to have been read ... spit it out
                    Serial.println("Reading Block 4:");
                    PrintHexChar(data, 16);
                    Serial.println("");

                    tft->println("Reading Block 4:");
                    tft->println("");
                    // Wait a bit before reading the card again
                    delay(1000);
                } else {
                    tft->setTextColor(TFT_RED);
                    Serial.println("Ooops ... unable to read the requested block.  Try another key?");
                    tft->println("Ooops ... unable to read the requested block.  Try another key?");
                    tft->setTextColor(TFT_GREEN);
                    delay(1000);
                }
            } else {
                // tft->setCursor(0, 0);
                // tft->fillScreen(TFT_BLACK);
                tft->setTextColor(TFT_RED);
                Serial.println("Ooops ... authentication failed: Try another key?");
                tft->println("Ooops ... authentication failed: Try another key?");
                tft->setTextColor(TFT_GREEN);
                delay(1000);
            }
        }

        if (uidLength == 7) {
            // We probably have a Mifare Ultralight card ...
            Serial.println("Seems to be a Mifare Ultralight tag (7 byte UID)");
            tft->println("Seems to be a Mifare Ultralight tag (7 byte UID)");

            // Try to read the first general-purpose user page (#4)
            Serial.println("Reading page 4");
            tft->println("Reading page 4");
            uint8_t data[32];
            success = nfc->mifareultralight_ReadPage (4, data);
            if (success) {
                // Data seems to have been read ... spit it out
                PrintHexChar(data, 4);
                Serial.println("");

                // Wait a bit before reading the card again
                delay(1000);
            } else {
                tft->setTextColor(TFT_RED);
                tft->println("Ooops ... unable to read the requested page!?");
                Serial.println("Ooops ... unable to read the requested page!?");
                tft->setTextColor(TFT_GREEN);
                delay(1000);
            }
        }
    }
}

