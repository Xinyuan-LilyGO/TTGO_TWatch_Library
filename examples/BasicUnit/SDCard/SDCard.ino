
#include "config.h"

TTGOClass *ttgo;

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextDatum(TL_DATUM);
    ttgo->tft->setCursor(0, 0);

    ttgo->tft->println("Listing directory:" + String(dirname));

    File root = fs.open(dirname);
    if (!root) {
        ttgo->tft->println("- failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        ttgo->tft->println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            ttgo->tft->print("  DIR : ");
            ttgo->tft->println(file.name());
            if (levels) {
                listDir(fs, file.name(), levels - 1);
            }
        } else {
            ttgo->tft->print("  FILE: ");
            ttgo->tft->print(file.name());
            ttgo->tft->print("  SIZE: ");
            ttgo->tft->println(file.size());
        }
        file = root.openNextFile();
    }
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();

    while (1) {
        if (ttgo->sdcard_begin()) {
            Serial.println("sd begin pass");
            break;
        }
        Serial.println("sd begin fail,wait 1 sec");
        delay(1000);
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
        Serial.println("No SD card attached");
        return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
        Serial.println("MMC");
    } else if (cardType == CARD_SD) {
        Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);

    ttgo->tft->fillScreen(TFT_BLACK);
    listDir(SD, "/", 2);
}





void loop()
{
    delay(1000);
}
