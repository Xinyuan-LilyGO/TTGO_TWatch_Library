/**
 * @file      gif.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-05-01
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

#if !LV_USE_GIF
#error "lvgl gid decoder library is not enable!"
#endif

LV_IMG_DECLARE(ellipse80);

void setup()
{
    watch.begin();

    beginLvglHelper();

    lv_obj_t *img = lv_gif_create(lv_scr_act());
    lv_gif_set_src(img, &ellipse80);
    lv_obj_center(img);
}



void loop()
{
    lv_task_handler();
    delay(5);
}

