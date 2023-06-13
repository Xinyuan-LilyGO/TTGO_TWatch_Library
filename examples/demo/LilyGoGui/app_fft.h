#pragma once
#include "app_typedef.h"
#include "arduinoFFT.h"
#include "global_flags.h"
#include "lvgl.h"

extern app_t app_fft;


typedef struct {
  double vReal[SAMPLES];
  double vImag[SAMPLES];
  //   double vTemp[HALSAMPLES];
} fft_t;

void app_fft_load(lv_obj_t *cont);
