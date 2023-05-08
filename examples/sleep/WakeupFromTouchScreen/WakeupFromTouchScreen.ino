/**
 * @file      WakeupFormTouchScreen.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-05-03
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>


RTC_DATA_ATTR int bootCount = 0;

bool  pmu_flag = false;
lv_obj_t *label1;


const char *get_wakeup_reason()
{
    switch (esp_sleep_get_wakeup_cause()) {
    case ESP_SLEEP_WAKEUP_EXT0 : return ("Wakeup caused by external signal using RTC_IO");
    case ESP_SLEEP_WAKEUP_EXT1 : return ("Wakeup caused by external signal using RTC_CNTL");
    case ESP_SLEEP_WAKEUP_TIMER : return ("Wakeup caused by timer");
    case ESP_SLEEP_WAKEUP_TOUCHPAD : return ("Wakeup caused by touchpad");
    case ESP_SLEEP_WAKEUP_ULP : return ("Wakeup caused by ULP program");
    default : return ("Wakeup was not caused");
    }
}

void setFlag(void)
{
    pmu_flag = true;
}

void setup()
{

    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    lv_obj_t *label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(label, LV_PCT(90));
    lv_label_set_text_fmt(label, "Boot counter: %d", ++bootCount);
    lv_obj_align(label, LV_ALIGN_CENTER, 0, -64);

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(label1, LV_PCT(90));
    lv_label_set_text(label1, "Waiting to press the crown to go to sleep...");
    lv_obj_center(label1);

    lv_obj_t *label2 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL);
    lv_obj_set_width(label2, LV_PCT(90));
    lv_label_set_text(label2, get_wakeup_reason());
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, -32);

    // Set the interrupt handler of the PMU
    watch.attachPMU(setFlag);

    // Waiting to press the crown to go to sleep
    while (!pmu_flag) {
        lv_task_handler();
        delay(5);
    }

    /*
    *   After the PMU interrupt is triggered, the interrupt status must be cleared,
    *   otherwise the next interrupt will not be triggered
    */
    watch.clearPMU();


    for (int i = 5; i > 0; i--) {
        lv_label_set_text_fmt(label1, "Go to sleep after %d seconds", i);
        lv_task_handler();
        delay(1000);
    }

    lv_label_set_text(label1, "Sleep now ...");
    lv_task_handler();
    delay(1000);

    //Set to wake up by touching the touchscreen
    watch.setSleepMode(TOUCH_WAKEUP);

    // go to sleep
    watch.sleep();

    Serial.println("This will never be printed");
}

void loop()
{

}



