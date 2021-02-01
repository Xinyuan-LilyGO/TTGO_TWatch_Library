#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft;
Adafruit_DRV2605 *drv;
uint8_t effect = 1;

void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    // Turn on the backlight
    ttgo->openBL();

    //  Receive as a local variable for easy writing
    tft = ttgo->tft;
    drv = ttgo->drv;

    // Some display setting
    tft->setCursor(0, 0);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->println("DRV2605 complex example");

    //Enable DRV2605
    ttgo->enableDrv2650();

    // I2C trigger by sending 'go' command
    drv->setMode(DRV2605_MODE_INTTRIG); // default, internal trigger when sending GO command

    drv->selectLibrary(1);
    drv->setWaveform(0, 84);  // ramp up medium 1, see datasheet part 11.2
    drv->setWaveform(1, 1);  // strong click 100%, see datasheet part 11.2
    drv->setWaveform(2, 0);  // end of waveforms
}

void loop()
{
    drv->go();
    delay(1000);
}

