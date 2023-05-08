/**
 * @file      DisplayRotation.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

lv_obj_t *label1;

bool isPmuIRQ = false;
uint8_t rotation = 0;

void setRotation()
{
    watch.setRotation(++rotation);
    rotation %= 4;
    updateLvglHelper();
}


static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        setRotation();
    }
}

void setup(void)
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    watch.attachPMU([]() {
        isPmuIRQ = true;
    });

    watch.setRotation(rotation);

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
    lv_label_set_text(label1, "0000");
    lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *btn1 = lv_btn_create(lv_scr_act());
#if LV_VERSION_CHECK(9,0,0)
    lv_obj_add_event(btn1, event_handler, LV_EVENT_CLICKED, NULL);
#else
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_CLICKED, NULL);
#endif
    lv_obj_set_width(btn1,  LV_PCT(80));
    lv_obj_align(btn1, LV_ALIGN_CENTER, 0, 45);

    lv_obj_t *label;
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Set Rotation");
    lv_obj_center(label);

}

void loop()
{
    if (isPmuIRQ) {
        isPmuIRQ = false;
        watch.readPMU();
        if (watch.isPekeyShortPressIrq()) {
            setRotation();
        }
        watch.clearPMU();
    }

    lv_point_t point;
    lv_indev_t *indev = lv_indev_get_next(NULL);
    if (indev) {
#if LV_VERSION_CHECK(9,0,0)
        if (lv_indev_get_state(indev) == LV_INDEV_STATE_PRESSED) {
#else
        if (indev->proc.state == LV_INDEV_STATE_PRESSED) {
#endif
            lv_indev_get_point(indev, &point);
            lv_label_set_text_fmt(label1, "Rotation %d \n X:%d Y:%d",
                                  watch.getRotation(),
                                  point.x, point.y);
        }
    }
    lv_task_handler();
    delay(5);
}

