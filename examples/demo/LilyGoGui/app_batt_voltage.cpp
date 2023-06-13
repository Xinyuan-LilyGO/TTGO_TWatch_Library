#include "app_batt_voltage.h"
#include "Arduino.h"

lv_obj_t * batt_voltage_label = NULL;

LV_FONT_DECLARE(robot_ightItalic_16);

extern lv_obj_t *setupGUI();

void app_batt_voltage_load(lv_obj_t *cont) {

    lv_obj_t * obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(obj, 0, 0);  
    lv_obj_set_style_pad_all(obj, 0, 0); 

    batt_voltage_label = lv_label_create(obj);
    lv_label_set_recolor(batt_voltage_label, true);    /*Enable re-coloring by commands in the text*/
    lv_obj_center(batt_voltage_label);
    lv_obj_set_style_text_color(batt_voltage_label, lv_color_white(), 0);
}


app_t app_batt_voltage = {
    .setup_func_cb = app_batt_voltage_load,
    .exit_func_cb = nullptr,
    .user_data= nullptr,
};