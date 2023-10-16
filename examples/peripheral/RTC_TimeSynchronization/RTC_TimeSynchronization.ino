/**
 * @file      RTC_TimeSynchronization.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <time.h>
#include <WiFi.h>
#include <sntp.h>

const char *ssid       = "YOUR_SSID";
const char *password   = "YOUR_PASS";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

const char *time_zone = "CST-8";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

uint32_t lastMillis;

lv_obj_t *label1;
lv_obj_t *label2;
lv_obj_t *label3;
char buf[64];
// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP, Write the hardware clock");

    // Write synchronization time to hardware
    watch.hwClockWrite();
}

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    // set notification call-back function
    sntp_set_time_sync_notification_cb( timeavailable );

    /**
     * NTP server address could be aquired via DHCP,
     *
     * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
     * otherwise SNTP option 42 would be rejected by default.
     * NOTE: configTime() function call if made AFTER DHCP-client run
     * will OVERRIDE aquired NTP server address
     */
    sntp_servermode_dhcp(1);    // (optional)

    /**
     * This will set configured ntp servers and constant TimeZone/daylightOffset
     * should be OK if your time zone does not need to adjust daylightOffset twice a year,
     * in such a case time adjustment won't be handled automagicaly.
     */
    // configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

    /**
     * A more convenient approach to handle TimeZones with daylightOffset
     * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
     * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
     */
    configTzTime(time_zone, ntpServer1, ntpServer2);


    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);

    label1 = lv_label_create(cont);
    lv_obj_set_width(label1, LV_PCT(90));  /*Set smaller width to make the lines wrap*/
    label2 = lv_label_create(cont);
    lv_obj_set_width(label2, LV_PCT(90));  /*Set smaller width to make the lines wrap*/
    label3 = lv_label_create(cont);
    lv_obj_set_width(label3, LV_PCT(90));  /*Set smaller width to make the lines wrap*/


    //connect to WiFi
    lv_label_set_text_fmt(label1, "Connecting to %s", ssid);
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(5);
        lv_task_handler();
    }
    Serial.println(" CONNECTED");
    lv_label_set_text_fmt(label1, "%s Connected", ssid);
}

void loop()
{
    if (millis() - lastMillis > 1000) {

        lastMillis = millis();

        // hardware clock
        struct tm hwTimeinfo;
        watch.getDateTime(&hwTimeinfo);
        Serial.print("Hardware clock :");
        Serial.println(&hwTimeinfo, "%A, %B %d %Y %H:%M:%S");
        strftime(buf, 64, "%A, %B %d %Y %H:%M:%S", &hwTimeinfo);
        lv_label_set_text_fmt(label2, "HW Clock:%s", buf);

        // system clock
        struct tm timeinfo;
        if (!getLocalTime(&timeinfo)) {
            Serial.println("No time available (yet)");
            lv_label_set_text(label3, "No time available (yet)");
            return;
        }

        Serial.print("System   clock :");
        Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
        Serial.println();
        strftime(buf, 64, "%A, %B %d %Y %H:%M:%S", &timeinfo);
        lv_label_set_text_fmt(label3, "SYS Clock:%s", buf);

    }
    lv_task_handler();
    delay(5);
}



