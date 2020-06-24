
#include "config.h"

TTGOClass *ttgo;

void setup()
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

    lv_obj_t *text = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(text, "T-Watch");
    lv_obj_align(text, NULL, LV_ALIGN_CENTER, 0, 0);
}

void loop()
{
    lv_task_handler();
    delay(5);
}
