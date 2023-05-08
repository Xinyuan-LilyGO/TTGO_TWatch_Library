/**
 * @file      RTC_AlarmByUnits.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */


#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <time.h>

lv_obj_t *label1;
lv_obj_t *datetime_label;
uint32_t lastMillis = 0;
uint8_t nextHour = 22;
uint8_t nextMonth = 1;
uint8_t nextDay = 1;
uint8_t nextMinute = 59;
uint8_t nextSecond = 55;


void setup()
{
    Serial.begin(115200);
    while (!Serial);

    watch.begin();

    beginLvglHelper();

    // Setting test datetime
    watch.setDateTime(2023, nextMonth, nextDay, nextHour, nextMinute, nextSecond);

    // Set the check minute to trigger RTC interrupt
    watch.setAlarmByMinutes(0);

    // Enable rtc alarm interrupt
    watch.enableAlarm();


    label1 = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_text(label1, "RTC_AlarmByUnits_Example");
    lv_obj_set_width(label1, LV_PCT(90));  /*Set smaller width to make the lines wrap*/
    lv_obj_center(label1);

    datetime_label = lv_label_create(lv_scr_act());
    lv_label_set_long_mode(datetime_label, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
    lv_label_set_text(datetime_label, "00:00:00");
    lv_obj_set_width(datetime_label, LV_PCT(90));  /*Set smaller width to make the lines wrap*/
    lv_obj_align_to(datetime_label, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

void printDateTime()
{
    if (millis() - lastMillis > 1000) {
        /**
        /// Format output time*
        Option:
            DATETIME_FORMAT_HM
            DATETIME_FORMAT_HMS
            DATETIME_FORMAT_YYYY_MM_DD
            DATETIME_FORMAT_MM_DD_YYYY
            DATETIME_FORMAT_DD_MM_YYYY
            DATETIME_FORMAT_YYYY_MM_DD_H_M_S
        default:   DATETIME_FORMAT_YYYY_MM_DD_H_M_S_WEEK
        */
        const char *str = watch.strftime();
        Serial.println(str);


        lv_label_set_text_fmt(datetime_label, "%s", str);
        lv_obj_align_to(datetime_label, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
        lastMillis = millis();
    }
    lv_task_handler();
    delay(5);
}

// Test minute timing
void  testAlarmMinute()
{
    lv_label_set_text(label1, "RTC_testAlarmMinute");
    lv_obj_center(label1);
    while (1) {
        //Get RTC interrupt status,if rtc interrupt active ,will return true
        if (watch.readRTC()) {
            Serial.println("testAlarmMinute Interrupt .... ");
            if (watch.isAlarmActive()) {
                Serial.println("Alarm active");
                watch.resetAlarm();
                watch.setDateTime(2022, nextMonth, nextDay, nextHour, nextMinute, nextSecond);
                nextHour++;
                if (nextHour >= 24) {
                    nextHour = 23;
                    nextDay = 25;
                    watch.setAlarmByHours(0);
                    Serial.println("setAlarmByHours");
                    return;
                }
            }
        }
        printDateTime();
    }
}

// Test hour timing
void  testAlarmHour()
{
    lv_label_set_text(label1, "RTC_testAlarmHour");
    lv_obj_center(label1);
    while (1) {
        //Get RTC interrupt status,if rtc interrupt active ,will return true
        if (watch.readRTC()) {
            Serial.println("testAlarmHour Interrupt .... ");
            if (watch.isAlarmActive()) {
                Serial.println("Alarm active");
                watch.resetAlarm();
                watch.setDateTime(2022, nextMonth, nextDay, nextHour, nextMinute, nextSecond);
                nextDay++;
                if (nextDay >= 30) {
                    nextMonth = 1;
                    nextHour = 23;
                    nextMinute = 59;
                    nextSecond = 55;
                    nextDay = watch.getDaysInMonth(nextMonth, 2022);
                    watch.setDateTime(2022, nextMonth, nextDay, nextHour, nextMinute, nextSecond);
                    watch.setAlarmByDays(1);
                    Serial.println("setAlarmByDays");
                    return;
                }
            }
        }
        printDateTime();
    }
}

// Test day timing
void  testAlarmDay()
{
    lv_label_set_text(label1, "RTC_testAlarmDay");
    lv_obj_center(label1);
    while (1) {
        //Get RTC interrupt status,if rtc interrupt active ,will return true
        if (watch.readRTC()) {
            Serial.println("testAlarmDay Interrupt .... ");
            if (watch.isAlarmActive()) {
                Serial.println("Alarm active");
                watch.resetAlarm();
                nextDay = watch.getDaysInMonth(nextMonth, 2022);
                watch.setDateTime(2022, nextMonth, nextDay, nextHour, nextMinute, nextSecond);
                nextMonth++;
                if (nextMonth >= 12) {
                    return;
                }
            }
        }
        printDateTime();
    }
}

void loop()
{
    testAlarmMinute();
    testAlarmHour();
    testAlarmDay();

    Serial.println("Test done ...");
    lv_label_set_text(label1, "RTC_Test done ...");
    while (1) {
        printDateTime();
    }
}



