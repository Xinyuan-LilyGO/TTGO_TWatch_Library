
#include "config.h"

TTGOClass *ttgo;

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    ttgo->lvgl_begin();

    lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);

    // Obtain the specified file from the file system, TWATCH 2020 V1 obtains it from SPIFFS by default
    lv_img_set_src(img, "P:ttgo.bin");
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

}

void loop()
{
    lv_task_handler();
    delay(5);
}
