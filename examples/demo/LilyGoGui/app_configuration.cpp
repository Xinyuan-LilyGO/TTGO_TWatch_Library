#include "app_configuration.h"
#include "Arduino.h"
#include <LilyGoLib.h>

static void sleep_event_cb(lv_event_t *e);
extern LilyGoLib watch;

static void slider_event_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    lv_obj_t *slider_label =  (lv_obj_t *)lv_event_get_user_data(e);
    uint8_t level = (uint8_t)lv_slider_get_value(slider);
    int percentage = map(level, 0, 255, 0, 100);
    lv_label_set_text_fmt(slider_label, "%u%%", percentage);
    watch.setBrightness(level);
}

void app_configuration_load(lv_obj_t *cont) {
  lv_obj_t *btn = lv_btn_create(cont);
  lv_obj_set_size(btn, 100, 50);
  lv_obj_align(btn, LV_ALIGN_CENTER, 0, -40);
  lv_obj_set_style_outline_color(btn, lv_color_white(), LV_STATE_FOCUS_KEY);
  lv_obj_set_style_bg_color(btn, lv_color_white(), 0);

  lv_obj_t *label = lv_label_create(btn);
  lv_obj_center(label);
  lv_obj_set_style_text_color(label, lv_color_black(), 0);
  lv_label_set_text(label, "Deep Sleep");
  lv_obj_add_event_cb(btn, sleep_event_cb, LV_EVENT_CLICKED, NULL);

  static lv_style_t style_indicator;
  static lv_style_t style_knob;

  /*Create a transition*/
  static const lv_style_prop_t props[] = {LV_STYLE_BG_COLOR, LV_STYLE_PROP_INV};
  static lv_style_transition_dsc_t transition_dsc;
  lv_style_transition_dsc_init(&transition_dsc, props, lv_anim_path_linear, 300, 0, NULL);

  lv_style_init(&style_indicator);
  lv_style_set_bg_opa(&style_indicator, LV_OPA_COVER);
  lv_style_set_bg_color(&style_indicator, lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_radius(&style_indicator, LV_RADIUS_CIRCLE);
  lv_style_set_transition(&style_indicator, &transition_dsc);

  lv_style_init(&style_knob);
  lv_style_set_bg_opa(&style_knob, LV_OPA_COVER);
  lv_style_set_bg_color(&style_knob, lv_color_make(0xff, 0xff, 0xff));
  lv_style_set_border_color(&style_knob, lv_palette_darken(LV_PALETTE_YELLOW, 2));
  lv_style_set_border_width(&style_knob, 2);
  lv_style_set_radius(&style_knob, LV_RADIUS_CIRCLE);
  lv_style_set_pad_all(&style_knob, 6); /*Makes the knob larger*/
  lv_style_set_transition(&style_knob, &transition_dsc);
  /*Create a slider and add the style*/
  lv_obj_t *slider = lv_slider_create(cont);
  lv_obj_set_size(slider, 200, 30);
  lv_slider_set_range(slider, 5, 255);
  lv_obj_add_style(slider, &style_indicator, LV_PART_INDICATOR);
  lv_obj_add_style(slider, &style_knob, LV_PART_KNOB);
  lv_obj_align(slider, LV_ALIGN_CENTER, 0, 40);
  lv_slider_set_value(slider, watch.getBrightness(), LV_ANIM_OFF);

  /*Create a label below the slider*/
  lv_obj_t *slider_label = lv_label_create(slider);
  lv_label_set_text_fmt(slider_label, "%u%%", watch.getBrightness());
  lv_obj_set_style_text_color(slider_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, slider_label);
  lv_obj_set_style_text_color(slider_label, lv_color_black(), 0);
  lv_obj_center(slider_label);
}

void lowPowerEnergyHandler();
static void sleep_event_cb(lv_event_t *e) {
    lowPowerEnergyHandler();
}

app_t app_config = {
    .setup_func_cb = app_configuration_load,
    .exit_func_cb = nullptr,
    .user_data = nullptr,
};