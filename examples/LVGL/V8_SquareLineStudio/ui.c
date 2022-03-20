// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.4
// LVGL VERSION: 8.2
// PROJECT: TWatch2020

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Main;
lv_obj_t * ui_imgKitty;
lv_obj_t * ui_flower0;
lv_obj_t * ui_flower45;
lv_obj_t * ui_flower90;
lv_obj_t * ui_flower135;
lv_obj_t * ui_flower180;
lv_obj_t * ui_flower225;
lv_obj_t * ui_flower270;
lv_obj_t * ui_flower315;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
#error "#error LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////
static void ui_event_imgKitty(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_LONG_PRESSED) {
    _ui_opacity_set(ui_imgKitty, 127);
    on_kitty_longpressed(e);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_imgKitty, 255);
    on_kitty_released(e);
  }
}
static void ui_event_flower0(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower0, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower0, 255);
    on_flower0_released(e);
  }
}
static void ui_event_flower45(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower45, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower45, 255);
    on_flower45_released(e);
  }
}
static void ui_event_flower90(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower90, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower90, 255);
    on_flower90_released(e);
  }
}
static void ui_event_flower135(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower135, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower135, 255);
    on_flower135_released(e);
  }
}
static void ui_event_flower180(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower180, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower180, 255);
    on_flower180_released(e);
  }
}
static void ui_event_flower225(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower225, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower225, 255);
    on_flower225_released(e);
  }
}
static void ui_event_flower270(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower270, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower270, 255);
    on_flower270_released(e);
  }
}
static void ui_event_flower315(lv_event_t * e)
{
  lv_event_code_t event = lv_event_get_code(e);
  lv_obj_t * ta = lv_event_get_target(e);
  if (event == LV_EVENT_PRESSED) {
    _ui_opacity_set(ui_flower315, 127);
  }
  if (event == LV_EVENT_RELEASED) {
    _ui_opacity_set(ui_flower315, 255);
    on_flower315_released(e);
  }
}

///////////////////// SCREENS ////////////////////
void ui_Main_screen_init(void)
{

  // ui_Main

  ui_Main = lv_obj_create(NULL);

  lv_obj_clear_flag(ui_Main, LV_OBJ_FLAG_SCROLLABLE);

  lv_obj_set_style_bg_color(ui_Main, lv_color_hex(0xC8C8C8), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_Main, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // ui_imgKitty

  ui_imgKitty = lv_img_create(ui_Main);
  lv_img_set_src(ui_imgKitty, &ui_img_kitty_png);

  lv_obj_set_width(ui_imgKitty, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_imgKitty, LV_SIZE_CONTENT);

  lv_obj_set_x(ui_imgKitty, 18);
  lv_obj_set_y(ui_imgKitty, 39);

  lv_obj_set_align(ui_imgKitty, LV_ALIGN_CENTER);

  lv_obj_add_flag(ui_imgKitty, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_add_event_cb(ui_imgKitty, ui_event_imgKitty, LV_EVENT_ALL, NULL);

  // ui_flower0

  ui_flower0 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower0, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_0_png, NULL);
  lv_imgbtn_set_src(ui_flower0, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_0_png, NULL);

  lv_obj_set_width(ui_flower0, 32);
  lv_obj_set_height(ui_flower0, 32);

  lv_obj_set_x(ui_flower0, -100);
  lv_obj_set_y(ui_flower0, 0);

  lv_obj_set_align(ui_flower0, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower0, ui_event_flower0, LV_EVENT_ALL, NULL);

  // ui_flower45

  ui_flower45 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower45, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_45_png, NULL);
  lv_imgbtn_set_src(ui_flower45, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_45_png, NULL);

  lv_obj_set_width(ui_flower45, 32);
  lv_obj_set_height(ui_flower45, 32);

  lv_obj_set_x(ui_flower45, -90);
  lv_obj_set_y(ui_flower45, -43);

  lv_obj_set_align(ui_flower45, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower45, ui_event_flower45, LV_EVENT_ALL, NULL);

  // ui_flower90

  ui_flower90 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower90, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_90_png, NULL);
  lv_imgbtn_set_src(ui_flower90, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_90_png, NULL);

  lv_obj_set_width(ui_flower90, 32);
  lv_obj_set_height(ui_flower90, 32);

  lv_obj_set_x(ui_flower90, -62);
  lv_obj_set_y(ui_flower90, -78);

  lv_obj_set_align(ui_flower90, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower90, ui_event_flower90, LV_EVENT_ALL, NULL);

  // ui_flower135

  ui_flower135 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower135, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_135_png, NULL);
  lv_imgbtn_set_src(ui_flower135, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_135_png, NULL);

  lv_obj_set_width(ui_flower135, 32);
  lv_obj_set_height(ui_flower135, 32);

  lv_obj_set_x(ui_flower135, -22);
  lv_obj_set_y(ui_flower135, -97);

  lv_obj_set_align(ui_flower135, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower135, ui_event_flower135, LV_EVENT_ALL, NULL);

  // ui_flower180

  ui_flower180 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower180, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_180_png, NULL);
  lv_imgbtn_set_src(ui_flower180, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_180_png, NULL);

  lv_obj_set_width(ui_flower180, 32);
  lv_obj_set_height(ui_flower180, 32);

  lv_obj_set_x(ui_flower180, 22);
  lv_obj_set_y(ui_flower180, -97);

  lv_obj_set_align(ui_flower180, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower180, ui_event_flower180, LV_EVENT_ALL, NULL);

  // ui_flower225

  ui_flower225 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower225, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_225_png, NULL);
  lv_imgbtn_set_src(ui_flower225, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_225_png, NULL);

  lv_obj_set_width(ui_flower225, 32);
  lv_obj_set_height(ui_flower225, 32);

  lv_obj_set_x(ui_flower225, 62);
  lv_obj_set_y(ui_flower225, -78);

  lv_obj_set_align(ui_flower225, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower225, ui_event_flower225, LV_EVENT_ALL, NULL);

  // ui_flower270

  ui_flower270 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower270, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_270_png, NULL);
  lv_imgbtn_set_src(ui_flower270, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_270_png, NULL);

  lv_obj_set_width(ui_flower270, 32);
  lv_obj_set_height(ui_flower270, 32);

  lv_obj_set_x(ui_flower270, 90);
  lv_obj_set_y(ui_flower270, -43);

  lv_obj_set_align(ui_flower270, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower270, ui_event_flower270, LV_EVENT_ALL, NULL);

  // ui_flower315

  ui_flower315 = lv_imgbtn_create(ui_Main);
  lv_imgbtn_set_src(ui_flower315, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_flower_315_png, NULL);
  lv_imgbtn_set_src(ui_flower315, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_flower_315_png, NULL);

  lv_obj_set_width(ui_flower315, 32);
  lv_obj_set_height(ui_flower315, 32);

  lv_obj_set_x(ui_flower315, 100);
  lv_obj_set_y(ui_flower315, 0);

  lv_obj_set_align(ui_flower315, LV_ALIGN_CENTER);

  lv_obj_add_event_cb(ui_flower315, ui_event_flower315, LV_EVENT_ALL, NULL);

}

void ui_init(void)
{
  ui_Main_screen_init();
  lv_disp_load_scr(ui_Main);
}
