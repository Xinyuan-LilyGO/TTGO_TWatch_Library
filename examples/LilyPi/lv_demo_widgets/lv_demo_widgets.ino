#include "config.h"
#include "lv_demo_widgets.h"

TTGOClass *ttgo;

void relayTurnOn(void)
{
    ttgo->turnOnRelay();
}

void relayTurnOff(void)
{
    ttgo->turnOffRelay();
}

void setBrightness(uint8_t level)
{
    ttgo->setBrightness(level);
}

void turnOnUSB()
{
    ttgo->turnOnUSB();
}

void turnOffUSB()
{
    ttgo->turnOffUSB();
}

float getVoltage()
{
    return ttgo->getVoltage();
}

void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    // Turn on the backlight
    ttgo->openBL();

    ttgo->lvgl_begin();

    ttgo->lvgl_whirling(1);

    if (!ttgo->sdcard_begin()) {
        // if sd card mount fail . show it
        Serial.println("SDCARD MOUNT FAILED");
        ttgo->tft->setTextFont(4);
        ttgo->tft->setCursor(0, 0);
        ttgo->tft->setTextColor(TFT_RED);
        ttgo->tft->println("SDCARD MOUNT FAILED");
        delay(5000);
    } else {
        Serial.println("SDCARD MOUNT SUCCESS");
    }

    //Check if RTC is online
    if (!ttgo->deviceProbe(0x51)) {
        Serial.println("RTC CHECK FAILED");
        ttgo->tft->fillScreen(TFT_BLACK);
        ttgo->tft->setTextFont(4);
        ttgo->tft->setCursor(0, 0);
        ttgo->tft->setTextColor(TFT_RED);
        ttgo->tft->println("RTC CHECK FAILED");
        delay(5000);
    }

    lv_demo_widgets();

}

void loop()
{
    lv_task_handler();
    delay(5);
}


