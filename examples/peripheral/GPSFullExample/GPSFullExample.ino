/**
 * @file      GPSFullExample.ino
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

    Serial.println(F("FullExample.ino"));
    Serial.println(F("An extensive example of many interesting TinyGPSPlus features"));
    Serial.print(F("Testing TinyGPSPlus library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println(F("by Mikal Hart"));
    Serial.println();
    Serial.println(F("Sats HDOP  Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum"));
    Serial.println(F("           (deg)      (deg)       Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail"));
    Serial.println(F("----------------------------------------------------------------------------------------------------------------------------------------"));

    label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
}

void loop()
{
    uint32_t  satellites = gps.satellites.isValid() ? gps.satellites.value() : 0;
    double hdop = gps.hdop.isValid() ? gps.hdop.hdop() : 0;
    double lat = gps.location.isValid() ? gps.location.lat() : 0;
    double lng = gps.location.isValid() ? gps.location.lng() : 0;
    uint32_t age = gps.location.isValid() ? gps.location.age() : 0;
    uint16_t year = gps.date.isValid() ? gps.date.year() : 0;
    uint8_t  month = gps.date.isValid() ? gps.date.month() : 0;
    uint8_t  day = gps.date.isValid() ? gps.date.day() : 0;
    uint8_t  hour = gps.time.isValid() ? gps.time.hour() : 0;
    uint8_t  minute = gps.time.isValid() ? gps.time.minute() : 0;
    uint8_t  second = gps.time.isValid() ? gps.time.second() : 0;
    double  meters = gps.altitude.isValid() ? gps.altitude.meters() : 0;
    double  kmph = gps.speed.isValid() ? gps.speed.kmph() : 0;
    lv_label_set_text_fmt(label, "Fix:%u\nSats:%u\nHDOP:%.1f\nLat:%.5f\nLon:%.5f \nDate:%d/%d/%d \nTime:%d/%d/%d\nAlt:%.2f m \nSpeed:%.2f\nRX:%u",
                          age, satellites, hdop, lat, lng,  year, month, day, hour, minute, second, meters, kmph, gps.charsProcessed());
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 5, 20);

    Serial.printf(
        "Fix:%u  Sats:%u  HDOP:%.1f  Lat:%.5f  Lon:%.5f   Date:%d/%d/%d   Time:%d/%d/%d  Alt:%.2f m   Speed:%.2f  RX:%u\n",
        age, satellites, hdop, lat, lng,  year, month, day, hour, minute, second, meters, kmph, gps.charsProcessed());

    smartDelay(1000);

    lv_task_handler();
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do {
        // read message from GPSSerial
        while (GPSSerial.available()) {
            int r = GPSSerial.read();
            gps.encode(r);
            Serial.write(r);
        }
    } while (millis() - start < ms);
}

static void printFloat(float val, bool valid, int len, int prec)
{
    if (!valid) {
        while (len-- > 1)
            Serial.print('*');
        Serial.print(' ');
    } else {
        Serial.print(val, prec);
        int vi = abs((int)val);
        int flen = prec + (val < 0.0 ? 2 : 1); // . and -
        flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
        for (int i = flen; i < len; ++i)
            Serial.print(' ');
    }
    smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len)
{
    char sz[32] = "*****************";
    if (valid)
        sprintf(sz, "%ld", val);
    sz[len] = 0;
    for (int i = strlen(sz); i < len; ++i)
        sz[i] = ' ';
    if (len > 0)
        sz[len - 1] = ' ';
    Serial.print(sz);
    smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
    if (!d.isValid()) {
        Serial.print(F("********** "));
    } else {
        char sz[32];
        sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
        Serial.print(sz);
    }

    if (!t.isValid()) {
        Serial.print(F("******** "));
    } else {
        char sz[32];
        sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
        Serial.print(sz);
    }

    printInt(d.age(), d.isValid(), 5);
    smartDelay(0);
}

static void printStr(const char *str, int len)
{
    int slen = strlen(str);
    for (int i = 0; i < len; ++i)
        Serial.print(i < slen ? str[i] : ' ');
    smartDelay(0);
}
