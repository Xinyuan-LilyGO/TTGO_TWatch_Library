/**
 * @file      main.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2022  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2022-11-11
 *
 */

#include "config.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SPIFFS.h>
#include <FS.h>
#include <HTTPClient.h>

TTGOClass *ttgo = nullptr;
const int httpsPort = 443;

const char *httpsHost = "raw.githubusercontent.com";
const char *webPath =  "/Xinyuan-LilyGO/TTGO_TWatch_Library/master/examples/BasicUnit/HttpsGetPhoto/";

unsigned long requestTimeout = 8000; //mean time between  requests
unsigned long requestTime = 0;
uint8_t fileCount = 0;

//---------------------------------------//
// edit this for your network
#define wifi_ssid       "WIFI SSID"
#define wifi_password   "WIFI PASSWORD"
//---------------------------------------//

const char *rootCACertificate =
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIEvjCCA6agAwIBAgIQBtjZBNVYQ0b2ii+nVCJ+xDANBgkqhkiG9w0BAQsFADBh\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
    "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
    "QTAeFw0yMTA0MTQwMDAwMDBaFw0zMTA0MTMyMzU5NTlaME8xCzAJBgNVBAYTAlVT\n" \
    "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxKTAnBgNVBAMTIERpZ2lDZXJ0IFRMUyBS\n" \
    "U0EgU0hBMjU2IDIwMjAgQ0ExMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKC\n" \
    "AQEAwUuzZUdwvN1PWNvsnO3DZuUfMRNUrUpmRh8sCuxkB+Uu3Ny5CiDt3+PE0J6a\n" \
    "qXodgojlEVbbHp9YwlHnLDQNLtKS4VbL8Xlfs7uHyiUDe5pSQWYQYE9XE0nw6Ddn\n" \
    "g9/n00tnTCJRpt8OmRDtV1F0JuJ9x8piLhMbfyOIJVNvwTRYAIuE//i+p1hJInuW\n" \
    "raKImxW8oHzf6VGo1bDtN+I2tIJLYrVJmuzHZ9bjPvXj1hJeRPG/cUJ9WIQDgLGB\n" \
    "Afr5yjK7tI4nhyfFK3TUqNaX3sNk+crOU6JWvHgXjkkDKa77SU+kFbnO8lwZV21r\n" \
    "eacroicgE7XQPUDTITAHk+qZ9QIDAQABo4IBgjCCAX4wEgYDVR0TAQH/BAgwBgEB\n" \
    "/wIBADAdBgNVHQ4EFgQUt2ui6qiqhIx56rTaD5iyxZV2ufQwHwYDVR0jBBgwFoAU\n" \
    "A95QNVbRTLtm8KPiGxvDl7I90VUwDgYDVR0PAQH/BAQDAgGGMB0GA1UdJQQWMBQG\n" \
    "CCsGAQUFBwMBBggrBgEFBQcDAjB2BggrBgEFBQcBAQRqMGgwJAYIKwYBBQUHMAGG\n" \
    "GGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTBABggrBgEFBQcwAoY0aHR0cDovL2Nh\n" \
    "Y2VydHMuZGlnaWNlcnQuY29tL0RpZ2lDZXJ0R2xvYmFsUm9vdENBLmNydDBCBgNV\n" \
    "HR8EOzA5MDegNaAzhjFodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vRGlnaUNlcnRH\n" \
    "bG9iYWxSb290Q0EuY3JsMD0GA1UdIAQ2MDQwCwYJYIZIAYb9bAIBMAcGBWeBDAEB\n" \
    "MAgGBmeBDAECATAIBgZngQwBAgIwCAYGZ4EMAQIDMA0GCSqGSIb3DQEBCwUAA4IB\n" \
    "AQCAMs5eC91uWg0Kr+HWhMvAjvqFcO3aXbMM9yt1QP6FCvrzMXi3cEsaiVi6gL3z\n" \
    "ax3pfs8LulicWdSQ0/1s/dCYbbdxglvPbQtaCdB73sRD2Cqk3p5BJl+7j5nL3a7h\n" \
    "qG+fh/50tx8bIKuxT8b1Z11dmzzp/2n3YWzW2fP9NsarA4h20ksudYbj/NhVfSbC\n" \
    "EXffPgK2fPOre3qGNm+499iTcc+G33Mw+nur7SpZyEKEOxEXGlLzyQ4UfaJbcme6\n" \
    "ce1XR2bFuAJKZTRei9AqPCCcUZlM51Ke92sRKw2Sfh3oius2FkOH6ipjv3U/697E\n" \
    "A7sKPPcw7+uvTPyLNhBzPvOk\n" \
    "-----END CERTIFICATE-----\n";


void drawBitmapFromHTTPS(const char *host, const char *path, const char *filename)
{
    static lv_obj_t *img = NULL;

    WiFiClientSecure client;
    bool connection_ok = false;

    Serial.print("connecting to ");
    Serial.println(host);

    // Set Server Certificate
    client.setCACert(rootCACertificate);

    String files = String("/") + filename;


    // Delete the files inside spiffs every time and download them again
    if (SPIFFS.exists(files)) {
        SPIFFS.remove(files);
    }

    // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
    HTTPClient https;
    String url =  "https://" + String(host) + path + filename;
    Serial.print("Get ");
    Serial.println(url);
    Serial.print("[HTTPS] begin...\n");
    if (https.begin(client, url)) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {

                // Save the acquired data in spiffs for storage
                fs::File f = SPIFFS.open(files, "w", true);
                WiFiClient stream = https.getStream();
                Serial.printf("SIZE:%d\n", https.getSize());

                https.writeToStream(&f);

                Serial.println();
                Serial.print("FileWrite :"); Serial.println(f.getWriteError());
                Serial.print("FileSize :"); Serial.println(f.size());
                f.close();
            }
        } else {
            Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }

        https.end();
    } else {
        Serial.printf("[HTTPS] Unable to connect\n");
    }

    // If the file exists, it will try to decode it using lvgl, and then display
    if (SPIFFS.exists(files)) {
        Serial.println("files is exist..");
        String fspath = String("P:/") + filename;
        if (img) {
            lv_obj_del(img);
            img = NULL;
        }
        img = lv_img_create(lv_scr_act(), NULL);
        lv_img_set_src(img, fspath.c_str());
        lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

    } else {
        Serial.println("files is not exist..");
    }

    client.stop();

}

void setup()
{

    Serial.begin(115200);

    Serial.println(F("Get https photo example"));

    // Get Watch Instance
    ttgo = TTGOClass::getWatch();

    // Initialize watch
    ttgo->begin();

    // Register lvgl
    ttgo->lvgl_begin();

    // Turn on the backlight
    ttgo->openBL();

    // Register wifi events
    WiFi.mode(WIFI_STA);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("."); delay(50);
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}


void loop()
{
    const char *filename[] = {
        "logo.png", "logoColor1.png", "logoColor2.png", "ttgo.png",
        "product0.png", "product1.png", "product2.png", "product3.png",
        "product4.png", "product5.png"
    };

    unsigned long timeNow = millis();
    if ((timeNow > requestTime))  {
        drawBitmapFromHTTPS(httpsHost, webPath, filename[fileCount++]);
        fileCount %= sizeof(filename) / sizeof(filename[0]);
        requestTime = timeNow + requestTimeout;
    }
    lv_task_handler(); delay(5);
}
