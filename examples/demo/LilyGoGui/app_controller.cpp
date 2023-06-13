#include "app_controller.h"
#include "Arduino.h"
#include "global_flags.h"
// #include "NimBLE/NimBLEDevice.h"

static struct {
  int32_t temp;
  int32_t humidity;
  lv_obj_t *temp_meter;
  lv_obj_t *hum_meter;
  lv_group_t *g;
  bool need_to_update;
  void *temp_msg_p;
  void *hum_msg_p;
} controller_param;

#define INDIC_COLOR_START lv_palette_main(LV_PALETTE_BLUE)
#define INDIC_COLOR_END   lv_color_hex(0xB84909)
#define INDIC_1_MIN       0
#define INDIC_1_MAX       100
#define INDIC_2_MIN       0
#define INDIC_2_MAX       100

extern QueueHandle_t led_setting_queue;
// extern NimBLEClient *pClient;
// extern NimBLERemoteCharacteristic *p2A6EChr;
// extern NimBLERemoteCharacteristic *p2A6FChr;

static void toggle_state_event_cb(lv_event_t *e);
static void change_meter_event_cb(lv_event_t *e);
static void label_event_cb(lv_event_t *e);
static void get_ble_data_event_cb(lv_event_t *e);

static void create_meter_1(lv_obj_t *cont);
static void create_meter_2(lv_obj_t *cont);

LV_IMG_DECLARE(img_knob);

typedef struct _meter_config_t {
  int32_t *value;
  int32_t value_min;
  int32_t value_max;
  lv_meter_indicator_t *indic;
  lv_meter_indicator_t *knob_indic;
  lv_obj_t *label;
  uint32_t msg_id;
} meter_config_t;

static meter_config_t met_conf1;
static meter_config_t met_conf2;

void app_controller_load(lv_obj_t *cont) {
  controller_param.g = lv_group_get_default();

  lv_obj_t *cont_row = lv_obj_create(cont);

  lv_obj_set_scrollbar_mode(cont_row, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_size(cont_row, LV_PCT(100), LV_PCT(100));
  lv_obj_align(cont_row, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
  lv_obj_set_scroll_snap_x(cont_row, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_flex_flow(cont_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_bg_opa(cont_row, LV_OPA_0, 0);

  create_meter_1(cont_row); // Create meter 1.
  create_meter_2(cont_row); // Create meter 2.

  lv_obj_update_snap(cont_row, LV_ANIM_ON);
  controller_param.need_to_update = true;
}

void exit_controller(lv_obj_t *cont) {
  controller_param.need_to_update = false;
  /* Not unsubscribing on return will keep sending updates causing a reset. */
  lv_msg_unsubscribe(controller_param.temp_msg_p);
  lv_msg_unsubscribe(controller_param.hum_msg_p);
  Serial.println(__FUNCTION__);
}

static void create_meter_1(lv_obj_t *cont) {
  lv_obj_t *t = controller_param.temp_meter = lv_meter_create(cont);
  /* Set the gauge style and shape. */
  lv_obj_set_size(t, 150, 150);
  lv_obj_clear_flag(t, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(t, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // Make the target focusable when scrolling
  lv_obj_remove_style(t, NULL, LV_PART_INDICATOR);

  lv_obj_set_style_outline_color(t, lv_color_white(), LV_STATE_FOCUS_KEY);
  lv_obj_set_style_outline_width(t, 3, LV_STATE_FOCUS_KEY);
  lv_obj_set_style_outline_opa(t, LV_OPA_50, LV_STATE_FOCUS_KEY);
  lv_obj_set_style_pad_all(t, 2, 0);

  /* Add meter rules. */
  lv_meter_scale_t *scale = lv_meter_add_scale(t);
  lv_meter_set_scale_ticks(t, scale, 41, 5, 10, lv_palette_main(LV_PALETTE_GREY));
  lv_meter_set_scale_range(t, scale, INDIC_1_MIN, INDIC_1_MAX, 270, 135);
  /* Add gauge needle 1. */
  lv_meter_indicator_t *indic = lv_meter_add_scale_lines(t, scale, INDIC_COLOR_START, INDIC_COLOR_END, false, 0);
  lv_meter_set_indicator_start_value(t, indic, 0);
  lv_meter_set_indicator_end_value(t, indic, controller_param.temp);
  /* Add gauge needle 2. */
  lv_meter_indicator_t *indic_knob = lv_meter_add_needle_img(t, scale, &img_knob, 60, 60);
  lv_meter_set_indicator_value(t, indic_knob, controller_param.temp);
  /* Add meter tags. */
  lv_obj_t *label = lv_label_create(t);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -5);
  lv_label_set_text_fmt(label, "%d°C", controller_param.temp);
  lv_obj_add_event_cb(label, label_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_LABEL_PARAM_CHANGE_1, label, (void *)"%d°C");

  lv_obj_t *title_label = lv_label_create(t);
  lv_obj_align(title_label, LV_ALIGN_CENTER, 0, 10);
  lv_label_set_text(title_label, "Temp");

  /* Add dial code change data callback event. */
  lv_group_add_obj(controller_param.g, t);
  lv_obj_add_event_cb(t, toggle_state_event_cb, LV_EVENT_CLICKED, NULL);
  met_conf1 = {
      .value = &controller_param.temp,
      .value_min = INDIC_1_MIN,
      .value_max = INDIC_1_MAX,
      .indic = indic,
      .knob_indic = indic_knob,
      .label = label,
      .msg_id = MSG_LABEL_PARAM_CHANGE_1,
  };
  lv_obj_add_event_cb(t, change_meter_event_cb, LV_EVENT_KEY, &met_conf1);
  /* Added bluetooth info callback event. */
  lv_obj_add_event_cb(t, get_ble_data_event_cb, LV_EVENT_MSG_RECEIVED, &met_conf1);
  controller_param.temp_msg_p = lv_msg_subsribe_obj(MSG_BLE_SEND_DATA_2, t, NULL);
}

static void create_meter_2(lv_obj_t *cont) {
  lv_obj_t *t = controller_param.temp_meter = lv_meter_create(cont);
  // Create a gauge frame
  lv_obj_set_size(t, 150, 150);
  lv_obj_clear_flag(t, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(t, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // Make the target focusable when scrolling
  lv_obj_remove_style(t, NULL, LV_PART_INDICATOR);

  lv_obj_set_style_outline_color(t, lv_color_white(), LV_STATE_FOCUS_KEY);
  lv_obj_set_style_outline_width(t, 3, LV_STATE_FOCUS_KEY);
  lv_obj_set_style_outline_opa(t, LV_OPA_50, LV_STATE_FOCUS_KEY);
  lv_obj_set_style_pad_all(t, 2, 0);
  // Add meter scale
  lv_meter_scale_t *scale = lv_meter_add_scale(t);
  lv_meter_set_scale_ticks(t, scale, 11, 2, 10, lv_palette_darken(LV_PALETTE_GREY, 4));
  lv_meter_set_scale_range(t, scale, INDIC_2_MIN, INDIC_2_MAX, 270, 135);
  // Add gauge pointer
  lv_meter_indicator_t *indic = lv_meter_add_arc(t, scale, 10, INDIC_COLOR_END, 0);
  lv_meter_set_indicator_start_value(t, indic, 0);
  lv_meter_set_indicator_end_value(t, indic, controller_param.humidity);

  lv_meter_indicator_t *indic_knob = lv_meter_add_needle_img(t, scale, &img_knob, 60, 60);
  lv_meter_set_indicator_value(t, indic_knob, controller_param.humidity);

  lv_obj_t *label = lv_label_create(t);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -5);
  lv_label_set_text_fmt(label, "%d %%", controller_param.humidity);
  lv_obj_add_event_cb(label, label_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_LABEL_PARAM_CHANGE_2, label, (void *)"%d %%");

  lv_obj_t *title_label = lv_label_create(t);
  lv_obj_align(title_label, LV_ALIGN_CENTER, 0, 10);
  lv_label_set_text(title_label, "Humidity");

  lv_group_add_obj(controller_param.g, t);
  lv_obj_add_event_cb(t, toggle_state_event_cb, LV_EVENT_CLICKED, NULL);
  met_conf2 = {
      .value = &controller_param.humidity,
      .value_min = INDIC_2_MIN,
      .value_max = INDIC_2_MAX,
      .indic = indic,
      .knob_indic = indic_knob,
      .label = label,
      .msg_id = MSG_LABEL_PARAM_CHANGE_2,
  };
  lv_obj_add_event_cb(t, change_meter_event_cb, LV_EVENT_KEY, &met_conf2);

  lv_obj_add_event_cb(t, get_ble_data_event_cb, LV_EVENT_MSG_RECEIVED, &met_conf2);
  controller_param.hum_msg_p = lv_msg_subsribe_obj(MSG_BLE_SEND_DATA_1, t, NULL);
}
static void toggle_state_event_cb(lv_event_t *e) {
  lv_obj_t *instance = lv_event_get_target(e);
  lv_event_code_t c = lv_event_get_code(e);
  if (c == LV_EVENT_CLICKED) {
    // toggle selected state
    if (lv_obj_has_flag(instance, LV_OBJ_FLAG_CHECKABLE)) {
      lv_obj_clear_flag(instance, LV_OBJ_FLAG_CHECKABLE);
      lv_group_set_editing(controller_param.g, false);
    } else {
      lv_obj_add_flag(instance, LV_OBJ_FLAG_CHECKABLE);
      lv_group_set_editing(controller_param.g, true);
    }
  }
}

static void change_meter_event_cb(lv_event_t *e) {
  /* get instance */
  lv_obj_t *instance = lv_event_get_target(e);
  lv_event_code_t c = lv_event_get_code(e);
  meter_config_t *u = (meter_config_t *)lv_event_get_user_data(e);

  if (c == LV_EVENT_KEY) {
    char s = *((char *)lv_event_get_param(e));
    lv_meter_indicator_t *indic = u->indic;
    int32_t v = indic->end_value; // Get the current pointer position.

    if ((s == LV_KEY_RIGHT || s == LV_KEY_UP) && v < indic->scale->max) {
      v++;
    } else if ((s == LV_KEY_LEFT || s == LV_KEY_DOWN) && v > indic->scale->min) {
      v--;
    }
    *u->value = v;                                            // Update global parameters
    lv_meter_set_indicator_end_value(instance, indic, v);     // Update the pointer in the gauge 1.
    lv_meter_set_indicator_value(instance, u->knob_indic, v); // Update the pointer in the gauge 2.
    lv_msg_send(u->msg_id, &v);                               // Send the message to update the label.
  }
}

static void label_event_cb(lv_event_t *e) {
  /* get instance */
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);

  const char *fmt = (const char *)lv_msg_get_user_data(m);
  const int32_t *v = (const int32_t *)lv_msg_get_payload(m);

  lv_label_set_text_fmt(label, fmt, *v); // Change the label text.
}

static void get_ble_data_event_cb(lv_event_t *e) {
  if (controller_param.need_to_update) {
    /* get instance */
    lv_obj_t *instance = lv_event_get_target(e);
    // lv_event_code_t c = lv_event_get_code(e);
    lv_msg_t *m = lv_event_get_msg(e);
    meter_config_t *u = (meter_config_t *)lv_event_get_user_data(e);
    const float *fv = (const float *)lv_msg_get_payload(m); // Get the data passed by BLE.
    int32_t v = *fv;                                        // Convert float to int.
    if (v >= INDIC_1_MIN && v <= INDIC_1_MAX) {
      *u->value = v;                                            // Update global parameters
      lv_meter_set_indicator_end_value(instance, u->indic, v);  // Update the pointer in the gauge 1.
      lv_meter_set_indicator_value(instance, u->knob_indic, v); // Update the pointer in the gauge 2.
      lv_msg_send(u->msg_id, &v);                               // Send the message to update the label.
    }
  }
}

app_t app_controller = {
    .setup_func_cb = app_controller_load,
    .exit_func_cb = exit_controller,
    .user_data = nullptr,
};
