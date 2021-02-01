#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft;
Adafruit_DRV2605 *drv;

uint8_t rtp_index = 0;
uint8_t rtp[] = {
    0x30, 100, 0x32, 100,
    0x34, 100, 0x36, 100,
    0x38, 100, 0x3A, 100,
    0x00, 100,
    0x40, 200, 0x00, 100,
    0x40, 200, 0x00, 100,
    0x40, 200, 0x00, 100
};

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
    tft->println("DRV2605 realtime example");

    //Enable DRV2605
    ttgo->enableDrv2650();

    // Set Real-Time Playback mode
    drv->setMode(DRV2605_MODE_REALTIME);
}

void loop()
{
    if (rtp_index < sizeof(rtp) / sizeof(rtp[0])) {
        drv->setRealtimeValue(rtp[rtp_index]);
        rtp_index++;
        delay(rtp[rtp_index]);
        rtp_index++;
    } else {
        drv->setRealtimeValue(0x00);
        delay(1000);
        rtp_index = 0;
    }
}

