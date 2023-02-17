/**
 * @file      StaticIPAddress.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-02-17
 *
 */

#include "config.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <SPIFFS.h>
#include <FS.h>
#include <HTTPClient.h>

TTGOClass *ttgo = nullptr;

const char *ssid = "yourNetworkName";
const char *password =  "yourNetworkPass";

//IPAddress
IPAddress staticIP(192, 168, 0, 113);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);

void setup()
{
    Serial.begin(115200);

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

    WiFi.begin(ssid, password);

    if (WiFi.config(staticIP, gateway, subnet, dns, dns) == false) {
        Serial.println("Configuration failed.");
    }

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("."); delay(50);
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS 1: ");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: ");
    Serial.println(WiFi.dnsIP(1));

}


void loop()
{

}
