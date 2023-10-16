/**
 * @file      switch.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

extern "C" {
    void lv_example_switch_1(void);
};


void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    lv_example_switch_1();
}


void loop()
{
    lv_task_handler();
    delay(5);
}
