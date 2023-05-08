/**
 * @file      BAM423_Accelerometer.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */

#include <LilyGoLib.h>
#include <LV_Helper.h>

uint32_t lastMillis;
lv_obj_t *label1;

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Default 4G ,200HZ , Avg4, Continuous mode
    watch.configAccelerometer();

    watch.enableAccelerometer();

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_obj_center(label1);
}


void loop()
{
    int16_t x, y, z;
    if (lastMillis < millis()) {
        lastMillis = millis() + 50;
        watch.getAccelerometer(x, y, z);
        Serial.print("X:");
        Serial.print(x); Serial.print(" ");
        Serial.print("Y:");
        Serial.print(y); Serial.print(" ");
        Serial.print("Z:");
        Serial.print(z);
        Serial.println();
        lv_label_set_text_fmt(label1, "X:%d \nY:%d \nZ:%d", x, y, z);
    }

    lv_task_handler();
    delay(5);
}



