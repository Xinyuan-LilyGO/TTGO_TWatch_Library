/*
   This example is using lvgl v8.2. GUI is exported from SquareLine Studio (https://squareline.io), 
     LVGL’s official UI editor using the "Export > Export File" functionality.

   Following files are exported from SquareLine Studio:
     
     - ui.c
     - ui.h
     - ui_events.c
     - ui_helpers.c
     - ui_helpers.h
     - ui_img_flower_0_png.c
     - ui_img_flower_135_png.c
     - ui_img_flower_180_png.c
     - ui_img_flower_225_png.c
     - ui_img_flower_270_png.c
     - ui_img_flower_315_png.c
     - ui_img_flower_45_png.c
     - ui_img_flower_90_png.c
     - ui_img_kitty_png.c
   
   Feel free to replace them with your exported design.

   In current example:
    * Tap on flowers at top of the screen and watch a few of them fall.
    * Long press on the cat to enable automatic spawning of random colored flowers.
*/

#include "config.h"
#include "ui.h"

TTGOClass *ttgo;

void setup()
{
  ttgo = TTGOClass::getWatch();
  ttgo->begin();
  ttgo->openBL();
  ttgo->lvgl_begin();

  ui_init();

  setCpuFrequencyMhz(240);
}

void loop()
{
  lv_timer_handler();
}
