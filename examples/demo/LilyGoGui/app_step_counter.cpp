#include "app_step_counter.h"
#include "Arduino.h"

LV_FONT_DECLARE(AlimamaShuHeiTi_Bold_54);
LV_IMG_DECLARE(step_img);
lv_obj_t * step_counter_label = NULL;

extern lv_obj_t *setupGUI();

void app_step_counter_load(lv_obj_t *cont) {
    lv_obj_t * obj = lv_obj_create(cont);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(obj, 0, 0);  
    lv_obj_set_style_pad_all(obj, 0, 0); 

    lv_obj_t * img = lv_img_create(obj);
    lv_obj_set_style_shadow_opa(img, LV_OPA_0, 0); 
    lv_obj_set_size(img, 60, 60);  
    lv_img_set_src(img, &step_img);
    lv_obj_align(img, LV_ALIGN_CENTER, -60, -10);

    step_counter_label = lv_label_create(obj);
    lv_obj_set_style_text_color(step_counter_label, lv_color_white(), 0);
    lv_obj_align(step_counter_label, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(step_counter_label, "0");
    lv_obj_set_style_text_font(step_counter_label, &AlimamaShuHeiTi_Bold_54, 0);
}


app_t app_step_counter = {
    .setup_func_cb = app_step_counter_load,
    .exit_func_cb = nullptr,
    .user_data= nullptr,
};