#pragma once

#include "lvgl.h"

typedef void (*app_func_t)(lv_obj_t *parent);

typedef struct {
  app_func_t setup_func_cb;
  app_func_t exit_func_cb;
  void *user_data;
} app_t;