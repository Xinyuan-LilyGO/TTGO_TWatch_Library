/**
 * @file      GPSFactory.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2024  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2024-06-16
 * @note      The T-WATCH-S3 watch does not have GPS function and requires an external GPS module to use
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <TinyGPSPlus.h>


// The TinyGPSPlus object
TinyGPSPlus gps;
lv_obj_t *label;


void setup()
{
    Serial.begin(115200);

    watch.begin();

    // Enable debug message output
    // Serial.setDebugOutput(true);

    beginLvglHelper();

    label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_label_set_text(label, "GPS Factory example");
    lv_obj_set_width(label, lv_pct(90));
    lv_label_set_long_mode(label, LV_LABEL_LONG_WRAP);
    lv_obj_center(label);
    lv_task_handler();

    uint32_t baud[] = {38400, 57600, 115200, 9600};
    bool result = 0;
    int retry = 4;
    int i = 0;
    do {
        GPSSerial.updateBaudRate(baud[i]);
        Serial.printf("Try use %u baud\n", baud[i]);
        delay(10);
        result = watch.gpsProbe();
        i++;
        if (i >= sizeof(baud) / sizeof(baud[0])) {
            retry--;
            i = 0;
        }
        if (!retry)
            break;
    } while (!result);

    if (result) {
        Serial.println("UBlox GPS init succeeded");
    } else {
        Serial.println("Warning: Failed to find UBlox GPS Module");
        lv_label_set_text(label, "Failed to find UBlox GPS Module");
        return;
    }
    if (watch.factoryGPS()) {
        Serial.println("UBlox GPS factory succeeded");
        lv_label_set_text(label, "UBlox GPS factory succeeded");
    } else {
        Serial.println("Warning: Failed to factory UBlox GPS");
        lv_label_set_text(label, "Failed to factory UBlox GPS");
        return;
    }
}

void loop()
{
    while (GPSSerial.available()) {
        Serial.write(GPSSerial.read());
    }
    while (Serial.available()) {
        GPSSerial.write(Serial.read());
    }
    lv_task_handler();
    delay(5);
}
