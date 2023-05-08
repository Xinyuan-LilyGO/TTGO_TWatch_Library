/**
 * @file      grid.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include "lv_example_grid.h"

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Tips : Select a separate function to see the effect
    lv_example_grid_1();
    // lv_example_grid_2();
    // lv_example_grid_3();
    // lv_example_grid_4();
    // lv_example_grid_5();
    // lv_example_grid_6();
}


void loop()
{
    lv_task_handler();
    delay(5);
}
