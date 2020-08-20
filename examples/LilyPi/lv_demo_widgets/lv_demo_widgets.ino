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

    ttgo->lvgl_begin();

    ttgo->lvgl_whirling(3);

    // Turn on the backlight
    ttgo->openBL();

    lv_demo_widgets();

}

void loop()
{
    lv_task_handler();
    delay(5);
}


