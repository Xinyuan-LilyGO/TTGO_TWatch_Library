/*
 * GPSDisplay.ino: Example of displaying  GPS information on the screen
 * Copyright 2020 Lewis he
 */
// Only supports 2020 V2 version, other versions do not support
// Only supports 2020 V2 version, other versions do not support
// Only supports 2020 V2 version, other versions do not support

#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

#define     REQUEST_URL_FORMART     "http://api.map.baidu.com/staticimage/v2?ak=%s&width=240&height=240&center=&markers=%f,%f&zoom=13&markerStyles=m,A,0xff0000"
#define     DEFAULT_IMG_FILENAME    "/tmp.png"

const char *ssid     = "WIFI SSID";
const char *password = "WIFI PASSWORD";

//BaiDu map doc: http://lbsyun.baidu.com/index.php?title=static
const char *baidu_ak = "YOUR BAIDU AK";

LV_IMG_DECLARE(img_gps);
LV_IMG_DECLARE(img_gps1);

TTGOClass *ttgo = nullptr;
TFT_eSPI *tft = nullptr;
TinyGPSPlus *gps = nullptr;

uint32_t lastUpdate = 0;
uint32_t updateMillis = 0;
lv_obj_t  *wifi_label = nullptr;
lv_obj_t  *gps_label = nullptr;
lv_obj_t  *img   = nullptr;
int i = 0;
const lv_img_dsc_t *imgs[] = {&img_gps, &img_gps1};

void img_loop_cb(lv_task_t *t)
{
    if (gps->location.isUpdated()) {
        lv_task_del(t);
    }
    lv_img_set_src(img, imgs[i++]);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, - 10);
    i %= 2;
}

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->lvgl_begin();

    ttgo->openBL();

    //Open gps power
    ttgo->trunOnGPS();

    ttgo->gps_begin();

    gps = ttgo->gps;

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());


    wifi_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text_fmt(wifi_label, "Connect:%s \nIP:%s \nRSSI:%d \n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.RSSI());
    lv_obj_align(wifi_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img, &img_gps);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, - 10);

    gps_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(gps_label, "Starting positioning...");
    lv_obj_align(gps_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    lv_task_create(img_loop_cb, 1000, LV_TASK_PRIO_LOW, NULL);
}

bool requestLoaction(const char *ak, double lat, double lng)
{
    char url_buf[512];
    while (1) {
        // wait for WiFi connection
        if (WiFi.isConnected()) {

            HTTPClient http;

            Serial.print("[HTTP] begin...\n");

            // configure traged server and url
            snprintf(url_buf, sizeof(url_buf), REQUEST_URL_FORMART, ak, lng, lat);
            // Serial.printf("REQE:%s\n", url_buf);

            http.begin(url_buf); //HTTP

            Serial.print("[HTTP] GET...\n");
            // start connection and send HTTP header
            int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                Serial.printf("[HTTP] GET... code: %d\n", httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK) {
                    File f = SPIFFS.open(DEFAULT_IMG_FILENAME, "w");
                    if (!f) {
                        http.end();
                        return false;
                    }
                    uint8_t buff[512] = { 0 };
                    int total = http.getSize();
                    int len = total;
                    WiFiClient *stream = http.getStreamPtr();
                    while (http.connected() && (len > 0 || len == -1)) {
                        size_t size = stream->available();
                        if (size) {
                            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
                            f.write(buff, c);
                            if (len > 0) {
                                len -= c;
                            }
                        }
                        delay(1);
                    }
                    Serial.println("done.");
                    Serial.printf("Size:%u\n", f.size());
                    f.close();
                    http.end();
                    return true;
                }
            } else {
                Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }
            http.end(); return false;
        }
    }
}


void loop(void)
{
    ttgo->gpsHandler();

    if (gps->location.isValid()) {
        if (millis() - lastUpdate > 60000) {
            if (requestLoaction(baidu_ak, gps->location.lat(), gps->location.lng())) {
                lv_img_set_src(img, "P:/tmp.png");
                lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
            }
            lastUpdate = millis();
        }
    }

    if (millis() - updateMillis > 1000) {
        char buffer[512];
        if (gps->charsProcessed() < 50) {
            Serial.println(F("WARNING: No GPS data.  Check wiring."));
            lv_label_set_text(gps_label, "WARNING: No GPS data.\n  Check wiring.");
        } else {
            snprintf(buffer, sizeof(buffer), "Lat=%.06f Long=%.06f\n DATETIME:%d/%d/%d %d:%d:%d\nAltitude:%.2f/M SATELLITES:%d",
                     gps->location.isUpdated() ?  gps->location.lat() : 0,
                     gps->location.isUpdated() ?  gps->location.lng() : 0,
                     gps->date.isUpdated() ? gps->date.year() : 0,
                     gps->date.isUpdated() ? gps->date.month() : 0,
                     gps->date.isUpdated() ? gps->date.day() : 0,
                     gps->time.isUpdated() ? gps->time.hour() : 0,
                     gps->time.isUpdated() ? gps->time.minute() : 0,
                     gps->time.isUpdated() ? gps->time.second() : 0,
                     gps->altitude.isUpdated() ? gps->altitude.meters() : 0,
                     gps->satellites.isUpdated() ? gps->satellites.value() : 0);
            lv_label_set_text(gps_label, buffer);
        }
        lv_obj_align(gps_label, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

        lv_label_set_text_fmt(wifi_label, "Connect:%s \nIP:%s \nRSSI:%d \n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), WiFi.RSSI());
        lv_obj_align(wifi_label, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

        updateMillis = millis();
    }

    lv_task_handler();

}
