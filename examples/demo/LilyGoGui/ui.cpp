#include "ui.h"
#include "Arduino.h"
#include "app_typedef.h"
#include "global_flags.h"
#include "lvgl.h"
#include "app_return.h"
#include "app_calendar.h"
#include "app_step_counter.h"
#include "app_batt_voltage.h"
#include "app_radio.h"


LV_FONT_DECLARE(alibaba_font);
LV_IMG_DECLARE(lilygo2_gif);

LV_IMG_DECLARE(home_img80);
LV_IMG_DECLARE(calendar_img80);
LV_IMG_DECLARE(img_led);
LV_IMG_DECLARE(img_mic);
LV_IMG_DECLARE(img_controller);
LV_IMG_DECLARE(img_music);
LV_IMG_DECLARE(img_wifi);
LV_IMG_DECLARE(img_configuration);
LV_IMG_DECLARE(img_batt_voltage);
LV_IMG_DECLARE(img_step_counter);
LV_IMG_DECLARE(radio_img);
LV_IMG_DECLARE(img_radio);


static lv_style_t style_frameless;
static lv_obj_t *main_screen;
static lv_group_t *menu_g, *app_g;
static lv_indev_t *indev;
void menu_name_label_event_cb(lv_event_t *e);

void ui_boot_anim() {
  lv_obj_t *logo_img = lv_gif_create(lv_scr_act());
  lv_obj_center(logo_img);
  lv_gif_set_src(logo_img, &lilygo2_gif);
  LV_DELAY(500);
  lv_obj_del(logo_img);
}
extern lv_obj_t *chart;
extern lv_obj_t * step_counter_label;
extern lv_obj_t * batt_voltage_label;
extern lv_timer_t *transmitTask;
extern void suspend_vadTaskHandler(void);

static void create_app(lv_obj_t *parent,const char *name, const lv_img_dsc_t *img, app_t *app_fun) {
  /* Create an interactive button named after the app. */
  lv_obj_t *btn = lv_btn_create(parent);
  lv_obj_set_size(btn, 150, lv_pct(100));
  lv_obj_set_style_bg_opa(btn, LV_OPA_0, 0);
  lv_obj_set_style_outline_color(btn, lv_color_black(), 0);
  lv_obj_align(btn, LV_ALIGN_TOP_RIGHT, 0, 0);

  if (img != nullptr) {
    lv_obj_t *icon = lv_img_create(btn);
    lv_img_set_src(icon, img);
    lv_obj_center(icon);
  }
  /* Text change event callback */
  lv_obj_add_event_cb(
      btn,
      [](lv_event_t *e) {
        lv_event_code_t c = lv_event_get_code(e);
        char *text = (char *)lv_event_get_user_data(e);
        if (c == LV_EVENT_FOCUSED) {
          lv_msg_send(MSG_MENU_NAME_CHANGED, text);
        }
      },
      LV_EVENT_FOCUSED, (void*)name);
  /* Click to select event callback */
  lv_obj_add_event_cb(
      btn,
      [](lv_event_t *e) {
        lv_event_code_t c = lv_event_get_code(e);
        app_t *func_cb = (app_t *)lv_event_get_user_data(e);

        lv_obj_t *parent = lv_obj_get_child(main_screen, 1);
        if (c == LV_EVENT_CLICKED) {
          lv_group_set_default(app_g);
          lv_indev_set_group(indev, app_g);
          /* Execute app control generation */
          // assert(app_func_cb);
          if (func_cb->setup_func_cb != nullptr) {
            (*func_cb->setup_func_cb)(parent);
          }
          lv_obj_t *return_btn = lv_btn_create(parent);
          lv_obj_align(return_btn, LV_ALIGN_TOP_LEFT, 3, 3);
          lv_obj_set_style_bg_opa(return_btn, LV_OPA_0, 0);
          lv_obj_set_style_outline_color(return_btn, lv_color_white(), LV_STATE_FOCUS_KEY);
          lv_obj_set_size(return_btn, 40, 40);
          lv_obj_t *text = lv_label_create(return_btn);
          lv_obj_center(text);
          lv_label_set_text(text, LV_SYMBOL_LEFT);
          /* Clean up useless controls and return */
          lv_obj_add_event_cb(
              return_btn,
              [](lv_event_t *e) {
                lv_obj_t *parent = lv_obj_get_parent(lv_event_get_target(e));
                app_t *exit_func_cb = (app_t *)lv_event_get_user_data(e);
                if (exit_func_cb->exit_func_cb != nullptr) { /* Execute callback on exit */
                  (*exit_func_cb->exit_func_cb)(parent);
                }
                lv_group_set_default(menu_g);
                lv_indev_set_group(indev, menu_g);
                lv_obj_clean(parent);
                lv_obj_set_tile_id(main_screen, 0, 0, LV_ANIM_ON);
                if(chart != NULL)
                {
                    chart = NULL;
                    suspend_vadTaskHandler();
                }
                
                step_counter_label = NULL;
                batt_voltage_label = NULL;
                if (!transmitTask->paused) {
                lv_timer_pause(transmitTask);
                Serial.println("lv_timer_pause transmitTask");
                }

              },
              LV_EVENT_CLICKED, func_cb);

          lv_obj_set_tile_id(main_screen, 0, 1, LV_ANIM_ON);
        }
      },
      LV_EVENT_CLICKED, app_fun);
}

void style_init() {
  lv_style_init(&style_frameless);
  lv_style_set_radius(&style_frameless, 0);
  lv_style_set_border_width(&style_frameless, 0);
  lv_style_set_bg_color(&style_frameless, lv_color_white());
}

void ui_init(void) {
  menu_g = lv_group_create();
  app_g = lv_group_create();
  indev = lv_indev_get_next(NULL);
  lv_indev_set_group(indev, menu_g);
  lv_group_set_default(menu_g);

  style_init();
  /* opening animation */
  main_screen = lv_tileview_create(lv_scr_act());
  lv_obj_align(main_screen, LV_ALIGN_TOP_RIGHT, 0, 0);
  lv_obj_set_size(main_screen, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_bg_color(main_screen, lv_color_hex(0x000000), LV_PART_MAIN);

  /* Create two views for switching menus and app UI */
  lv_obj_t *menu_panel = lv_tileview_add_tile(main_screen, 0, 0, LV_DIR_HOR);
  lv_obj_set_style_bg_color(menu_panel, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_t *app_panel = lv_tileview_add_tile(main_screen, 0, 1, LV_DIR_HOR);
  if(app_panel == NULL)
    return;

  lv_obj_clear_flag(menu_panel, LV_OBJ_FLAG_SCROLLABLE);

  /* Initialize the menu view */
  lv_obj_t *panel = lv_obj_create(menu_panel);
  lv_obj_set_size(panel, 240, 130);
  lv_obj_set_scroll_snap_x(panel, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_flex_flow(panel, LV_FLEX_FLOW_ROW);
  lv_obj_align(panel, LV_ALIGN_TOP_MID, 0, 10);
  lv_obj_add_style(panel, &style_frameless, 0);

  /* Add application */
  create_app(panel, "Return", &home_img80, &app_return);
  create_app(panel, "Calendar", &calendar_img80, &app_calendar);
  create_app(panel, "Wireless", &img_wifi, &app_wireless);
  create_app(panel, "Music", &img_music, &app_music);
  create_app(panel, "Mic FFT", &img_mic, &app_fft);
  create_app(panel, "Radio", &img_radio, &app_radio);
  create_app(panel, "Configuration", &img_configuration, &app_config);
  create_app(panel, "Step Counter", &img_step_counter, &app_step_counter);
  create_app(panel, "Batt Voltage", &img_batt_voltage, &app_batt_voltage);

  /* Initialize the label */
  lv_obj_t *desc_label = lv_label_create(menu_panel);
  lv_obj_set_width(desc_label, LV_PCT(100));
  lv_obj_align(desc_label, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_set_style_text_align(desc_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_font(desc_label, &alibaba_font, 0);
  lv_obj_set_style_bg_color(desc_label, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_text_color(desc_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_label_set_long_mode(desc_label, LV_LABEL_LONG_SCROLL_CIRCULAR);
  lv_obj_add_event_cb(desc_label, menu_name_label_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
  lv_msg_subsribe_obj(MSG_MENU_NAME_CHANGED, desc_label, NULL);

  lv_event_send(lv_obj_get_child(panel, 0), LV_EVENT_FOCUSED, NULL);
  lv_obj_update_snap(panel, LV_ANIM_ON);
}

void menu_name_label_event_cb(lv_event_t *e) {
  lv_obj_t *label = lv_event_get_target(e);
  lv_msg_t *m = lv_event_get_msg(e);

  const char *v = (const char *)lv_msg_get_payload(m);
  lv_label_set_text_fmt(label, v);
}

void ui_send_msg(char *str, uint32_t delay) {
  lv_obj_t *msg_obj = lv_obj_create(main_screen);
  lv_obj_remove_style(msg_obj, NULL, LV_PART_SCROLLBAR);
  lv_obj_align(msg_obj, LV_ALIGN_TOP_MID, 0, 0);
  lv_obj_set_size(msg_obj, 200, 40);

  lv_obj_t *label = lv_label_create(msg_obj);
  lv_obj_center(label);
  lv_label_set_text(label, str);
  lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);

  lv_anim_t a;
  lv_anim_init(&a);
  lv_anim_set_exec_cb(&a, [](void *obj, int32_t v) { lv_obj_align((lv_obj_t *)obj, LV_ALIGN_TOP_MID, 0, v); });
  lv_anim_set_var(&a, msg_obj);
  lv_anim_set_time(&a, 500);
  lv_anim_set_values(&a, -40, 0);
  lv_anim_set_playback_time(&a, 500);
  lv_anim_set_playback_delay(&a, delay);
  lv_anim_set_repeat_count(&a, 1);
  lv_anim_set_deleted_cb(&a, [](lv_anim_t *a) {
    lv_obj_t *obj = (lv_obj_t *)a->var;
    lv_obj_del(obj);
  });
  lv_anim_start(&a);
}