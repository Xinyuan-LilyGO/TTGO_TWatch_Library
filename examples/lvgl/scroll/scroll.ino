/**
 * @file      scroll.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include "lv_example_scroll.h"

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Tips : Select a separate function to see the effect
    lv_example_scroll_1();
    // lv_example_scroll_2();
    // lv_example_scroll_3();
    // lv_example_scroll_4();
    // lv_example_scroll_5();
    // lv_example_scroll_6();
}


void loop()
{
    lv_task_handler();
    delay(5);
}
