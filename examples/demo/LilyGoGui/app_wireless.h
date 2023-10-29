#pragma once

#include "app_typedef.h"
#include "lvgl.h"

extern app_t app_wireless;

typedef struct {
  lv_obj_t *(*obj_create)(lv_obj_t *parent);
  lv_align_t align;
  lv_coord_t x_ofs;
  lv_coord_t y_ofs;
  const char *text;
} textarea_config_t;

void app_wireless_load(lv_obj_t *cont);
