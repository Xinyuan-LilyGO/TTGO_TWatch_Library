#include "config.h"
#include "lv_demo_widgets.h"

TTGOClass *ttgo;

void relayTurnOn(void)
{
#ifdef LILYGO_LILYPI_V1
    ttgo->turnOffRelay();
#endif
}

void relayTurnOff(void)
{
#ifdef LILYGO_LILYPI_V1
    ttgo->turnOnRelay();
#endif
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

    if (!ttgo->sdcard_begin()) {
        // if sd card mount fail . show it
        Serial.println("SDCARD MOUNT FAILED");
        ttgo->tft->setTextFont(4);
        ttgo->tft->setRotation(3);
        ttgo->tft->setCursor(0, 0);
        ttgo->tft->setTextColor(TFT_RED);
        ttgo->tft->println("SDCARD MOUNT FAILED");
        delay(5000);
    } else {
        Serial.println("SDCARD MOUNT SUCCESS");
    }

    ttgo->lvgl_begin();

    ttgo->lvgl_whirling(3);



    lv_demo_widgets();

}

void loop()
{
    lv_task_handler();
    delay(5);
}


