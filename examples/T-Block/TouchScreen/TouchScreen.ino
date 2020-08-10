#include "config.h"
#include "lv_demo_widgets.h"

TTGOClass *ttgo;
TFT_eSPI *tft;

void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    //  Receive as a local variable for easy writing
    tft = ttgo->tft;

    ttgo->lvgl_begin();

    // Turn on the backlight
    ttgo->openBL();

    lv_demo_widgets();

}

void loop()
{
    lv_task_handler();
    delay(5);
}


