/**
 * @file      BAM423_Temperature.ino
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
    // To monitor data, the USB_CDC_BOOT must be set to enable in the Arduino IDE
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_obj_center(label1);
}


void loop()
{
    if (lastMillis < millis()) {
        lastMillis = millis() + 1000;
        //Obtain the temperature value on the accelerometer
        float accel_celsius = watch.readAccelTemp();
        // Obtain the temperature value of esp32
        float core_celsius = watch.readCoreTemp();
        lv_label_set_text_fmt(label1, "Accel: %.2f°C \nESP32: %.2f°C", accel_celsius,  core_celsius);
    }
    lv_task_handler();
    delay(5);
}



