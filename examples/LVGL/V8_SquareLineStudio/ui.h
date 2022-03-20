// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.4
// LVGL VERSION: 8.2
// PROJECT: TWatch2020

#ifndef _TWATCH2020_UI_H
#define _TWATCH2020_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

extern lv_obj_t * ui_Main;
extern lv_obj_t * ui_imgKitty;
extern lv_obj_t * ui_flower0;
extern lv_obj_t * ui_flower45;
extern lv_obj_t * ui_flower90;
extern lv_obj_t * ui_flower135;
extern lv_obj_t * ui_flower180;
extern lv_obj_t * ui_flower225;
extern lv_obj_t * ui_flower270;
extern lv_obj_t * ui_flower315;

void on_kitty_longpressed(lv_event_t * e);
void on_kitty_released(lv_event_t * e);
void on_flower0_released(lv_event_t * e);
void on_flower45_released(lv_event_t * e);
void on_flower90_released(lv_event_t * e);
void on_flower135_released(lv_event_t * e);
void on_flower180_released(lv_event_t * e);
void on_flower225_released(lv_event_t * e);
void on_flower270_released(lv_event_t * e);
void on_flower315_released(lv_event_t * e);

LV_IMG_DECLARE(ui_img_kitty_png);    // assets\kitty.png
LV_IMG_DECLARE(ui_img_flower_0_png);    // assets\flower_0.png
LV_IMG_DECLARE(ui_img_flower_45_png);    // assets\flower_45.png
LV_IMG_DECLARE(ui_img_flower_90_png);    // assets\flower_90.png
LV_IMG_DECLARE(ui_img_flower_135_png);    // assets\flower_135.png
LV_IMG_DECLARE(ui_img_flower_180_png);    // assets\flower_180.png
LV_IMG_DECLARE(ui_img_flower_225_png);    // assets\flower_225.png
LV_IMG_DECLARE(ui_img_flower_270_png);    // assets\flower_270.png
LV_IMG_DECLARE(ui_img_flower_315_png);    // assets\flower_315.png




void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
