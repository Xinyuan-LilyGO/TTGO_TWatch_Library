/**
 * @file      DRV2605_Basic.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

lv_obj_t *slider;

void value_changed_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *label = (lv_obj_t *)lv_event_get_user_data(e);
    int16_t value = lv_slider_get_value(slider);
    lv_label_set_text_fmt(label, "Effect:%d", value);
}

void vibration_feedback_effect_handler(lv_timer_t *time)
{
    // get the effect value
    int16_t value = lv_slider_get_value(slider);
    // set the effect to play
    watch.setWaveform(0, value);  // play effect
    // play the effect!
    watch.run();
}

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    /*Create a slider in the center of the display*/
    slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, LV_PCT(80));
    lv_obj_set_height(slider, LV_PCT(10));
    lv_slider_set_range(slider, 0, 113);
    lv_obj_center(slider);

    /*Create a label below the slider*/
    lv_obj_t *slider_label = lv_label_create(lv_scr_act());
    lv_label_set_text(slider_label, "Effect:0");
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    lv_obj_add_event_cb(slider, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, slider_label);

    lv_timer_create(vibration_feedback_effect_handler, 1000, NULL);
}


void loop()
{
    lv_task_handler();
    delay(5);
}



