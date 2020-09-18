#pragma mark - Depend Arduino-IRremote library
/*
cd ~/Arduino/libraries
git clone https://github.com/lewisxhe/Arduino-IRremote.git
*/

#include "config.h"
#include <IRremote.h>       //Depend  https://github.com/lewisxhe/Arduino-IRremote.git
#include <WiFi.h>
#include <WiFiMulti.h>

LV_IMG_DECLARE(img_background);
LV_IMG_DECLARE(img_unit);
LV_IMG_DECLARE(img_plus);
LV_IMG_DECLARE(img_minus);
LV_IMG_DECLARE(img_power);

LV_FONT_DECLARE(font_outaeg_128);

// #define ENABLE_ULTRASONIC_MODULE     //Ultrasonic module
#define WIFI_SSID           "WIFI_SSID"
#define WIFI_PASSWD         "WIFI_PASSWD"


//! Use Arduino-IRremote/examples/IRrecvDumpV2 to generate RAW
//! Use Arduino-IRremote/examples/IRrecvDumpV2 to generate RAW
//! Use Arduino-IRremote/examples/IRrecvDumpV2 to generate RAW
const unsigned int  airOFF[211] = {9050, 4400, 600, 1550, 650, 1550, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 700}; // NEC C3E90000
const unsigned int  airLevel30[211] = {9000, 4400, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 1550, 600, 1550, 650, 400, 600, 1600, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 550, 450, 650, 400, 650, 1550, 600, 450, 600, 1550, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 600, 450, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 400, 650, 400, 700, 350, 700, 350, 650, 400, 650, 400, 700, 350, 650, 400, 650, 400, 650, 1500, 650, 400, 650, 1550, 650, 1500, 650, 400, 650, 1550, 650, 400, 650}; // NEC C3ED0000
const unsigned int  airLevel29[211] = {9000, 4400, 650, 1550, 600, 1550, 650, 400, 650, 400, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600, 1600, 600, 400, 650, 1550, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600}; // NEC C3F50000
const unsigned int  airLevel28[211] = {9050, 4400, 650, 1550, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 1550, 600, 1600, 600, 1550, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 1600, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 1550, 650, 400, 650}; // NEC C3E50000
const unsigned int  airLevel27[211] = {9000, 4450, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 600, 1600, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 1550, 600, 450, 600, 1550, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1600, 600, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600}; // NEC C3F90000
const unsigned int  airLevel26[211] = {9000, 4450, 600, 1550, 600, 1600, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 1550, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 1600, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 600, 1600, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 1550, 600, 1550, 650, 400, 650, 400, 650}; // NEC C3E90000
const unsigned int  airLevel25[211] = {9000, 4400, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1600, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 450, 600, 400, 650}; // NEC C3F10000
const unsigned int  airLevel24[211] = {8950, 4400, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 1550, 650, 400, 600, 450, 600}; // NEC C3E10000
const unsigned int  airLevel23[211] = {8950, 4450, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600, 1600, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 600, 450, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 600, 450, 600, 1600, 600, 400, 650, 400, 650}; // NEC C3FE0000
const unsigned int  airLevel22[211] = {9000, 4450, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 600, 1600, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 1550, 600, 1550, 600, 1600, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 1550, 650, 400, 600, 450, 600, 450, 600}; // NEC C3EE0000
const unsigned int  airLevel21[211] = {8900, 4400, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 450, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 600, 450, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 550, 500, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 1550, 650, 400, 650, 400, 600, 450, 600}; // NEC C3F60000
const unsigned int  airLevel20[211] = {9050, 4400, 650, 1550, 600, 1550, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600}; // NEC C3E60000
const unsigned int  airLevel19[211] = {9050, 4400, 600, 1600, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600}; // NEC C3FA0000
const unsigned int  airLevel18[211] = {9000, 4450, 600, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 600, 1600, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1600, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 1550, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 600}; // NEC C3EA0000
const unsigned int  airLevel17[211] = {9000, 4400, 650, 1550, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 600, 1600, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 1550, 600, 450, 600, 450, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650}; // NEC C3F20000
const unsigned int  airLevel16[211] = {9000, 4450, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 1550, 650, 1550, 600, 1550, 650, 1550, 600, 450, 600, 450, 600, 450, 600, 1550, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 600, 1600, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 400, 650, 400, 650, 400, 650, 400, 600, 450, 600, 450, 600, 450, 600, 1550, 650, 1550, 600, 450, 600, 1550, 600, 450, 600, 1550, 650, 1550, 600, 1550, 650}; // NEC C3E20000

const unsigned int *airArray[] = {
    airLevel16,
    airLevel17,
    airLevel18,
    airLevel19,
    airLevel20,
    airLevel21,
    airLevel22,
    airLevel23,
    airLevel24,
    airLevel25,
    airLevel26,
    airLevel27,
    airLevel28,
    airLevel29,
    airLevel30
};

#define AIR_MAX_TEMPERATURE 30
#define AIR_MIN_TEMPERATURE 16

const uint8_t trigPin = 12;
const uint8_t echopin = 33;

TTGOClass *ttgo;
IRsend irsend(25);
WiFiMulti wifiMulti;

static uint8_t air_level = 24;
static uint8_t on_flags = false;
static bool move_in = true, move_out = false;
static lv_obj_t *statusBarCont,
       *mainBarCont,
       *timeLabel,
       *wifiLabel,
       *sensorLabel,
       *imgbtn1,
       *imgbtn2,
       *imgbtn3,
       *tempLabel;


void ctrl_time_update_task(lv_task_t *t);
void lv_click_onoff_event(lv_obj_t *obj, lv_event_t e);
void lv_click_minus_event(lv_obj_t *obj, lv_event_t e);
void lv_click_plus_event(lv_obj_t *obj, lv_event_t e);
void gui_init();

void ir_air_on()
{
    irsend.sendRaw(airArray[air_level - sizeof(airArray) / sizeof(airArray[0]) - 1], 211, 38);
}

void ir_air_off()
{
    irsend.sendRaw(airOFF, 211, 38);
}

void ir_send_to_air(int level)
{
    irsend.sendRaw(airArray[level - sizeof(airArray) / sizeof(airArray[0]) - 1], 211, 38);
}

void relayTurnOn(void)
{
    ttgo->turnOnRelay();
}

void relayTurnOff(void)
{
    ttgo->turnOffRelay();
}

void turnOffUSB()
{
    ttgo->turnOffUSB();
}

float getVoltage()
{
    return ttgo->getVoltage();
}

void setup()
{
    Serial.begin(115200);

    //Get watch instance
    ttgo = TTGOClass::getWatch();

    // Initialize the hardware
    ttgo->begin();

    // Turn on the backlight
    ttgo->openBL();

    ttgo->lvgl_begin();

    ttgo->lvgl_whirling(1);

    irsend.enableIROut(38);

    WiFi.mode(WIFI_STA);
    wifiMulti.addAP(WIFI_SSID, WIFI_PASSWD);
    Serial.println("Connecting Wifi...");

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("Connected to access point");
    }, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);

    WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        Serial.println("WiFi connected");
        Serial.print("Obtained IP address: ");
        Serial.println(WiFi.localIP());
        configTzTime("CST-8", "pool.ntp.org");
    }, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);

    gui_init();

    ttgo->turnOnUSB();

#ifdef ENABLE_ULTRASONIC_MODULE
    pinMode(trigPin, OUTPUT);
    pinMode(echopin, INPUT);
    lv_task_create([](lv_task_t *t) {
#define SAMPLES 25
        static uint32_t out_tick = 0;
        uint32_t sum = 0;
        uint8_t i = SAMPLES;
        while (i--) {
            digitalWrite(trigPin, HIGH);
            delayMicroseconds(5);
            digitalWrite(trigPin, LOW);
            sum +=  pulseIn(echopin, HIGH);
        }
        sum /= SAMPLES;
        sum /= 58;

        // The detection average is less than 70, the setting screen is closed
        if (sum < 70 && !move_in) {
            move_in = true;
            for (uint8_t i = 20; i < 255; i++) {
                ttgo->setBrightness(i);
                delay(5);
            }
            out_tick = 0;
            // ttgo->openBL();
        } else if (sum > 70 && move_in) {
            if (out_tick++ > 10) {
                for (uint8_t i = 255; i > 0; i--) {
                    ttgo->setBrightness(i);
                    delay(5);
                }
                // ttgo->closeBL();
                move_in = false;
            }
        }
    }, 500, LV_TASK_PRIO_HIGH, NULL);
#endif
}



void loop()
{
    wifiMulti.run();
    lv_task_handler();
    delay(5);
}



void lv_click_plus_event(lv_obj_t *obj, lv_event_t e)
{
    if (e != LV_EVENT_CLICKED)return;
    if (air_level + 1 > AIR_MAX_TEMPERATURE)return;
    lv_label_set_text_fmt(tempLabel, "%u", ++air_level);
    ir_send_to_air(air_level);
}

void lv_click_minus_event(lv_obj_t *obj, lv_event_t e)
{
    if (e != LV_EVENT_CLICKED)return;
    if (air_level - 1 < AIR_MIN_TEMPERATURE)return;
    lv_label_set_text_fmt(tempLabel, "%u", --air_level);
    ir_send_to_air(air_level);
}

void lv_click_onoff_event(lv_obj_t *obj, lv_event_t e)
{
    if (e != LV_EVENT_CLICKED)return;
    on_flags = !on_flags;
    if (on_flags) {
        lv_obj_set_click(imgbtn2, false);
        lv_obj_set_click(imgbtn1, false);
        lv_obj_set_style_local_image_recolor(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_obj_set_style_local_image_recolor(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
        lv_obj_set_style_local_image_recolor_opa(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_30);
        lv_obj_set_style_local_image_recolor_opa(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_30);
        ir_air_off();
    } else {
        lv_obj_set_click(imgbtn2, true);
        lv_obj_set_click(imgbtn1, true);
        lv_obj_set_style_local_image_recolor(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_BLACK);
        lv_obj_set_style_local_image_recolor(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_COLOR_BLACK);
        lv_obj_set_style_local_image_recolor_opa(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_OPA_30);
        lv_obj_set_style_local_image_recolor_opa(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_PRESSED, LV_OPA_30);
        lv_obj_set_style_local_image_recolor(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_image_recolor(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
        lv_obj_set_style_local_image_recolor_opa(imgbtn1, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
        lv_obj_set_style_local_image_recolor_opa(imgbtn2, LV_IMGBTN_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_0);
        ir_air_on();
    }
}

void ctrl_time_update_task(lv_task_t *t)
{
    static char strBuffer[80];
    struct tm timeinfo;
    if (getLocalTime(&timeinfo, 1000)) {
        strftime(strBuffer, sizeof(strBuffer), "%H:%M", &timeinfo);
        lv_label_set_text(timeLabel, strBuffer);
        lv_obj_realign(timeLabel);
    }
}


void gui_init(void)
{
    lv_obj_t *wp = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(wp, &img_background);
    lv_obj_set_size(wp, LV_HOR_RES, LV_VER_RES);

    static lv_style_t bar_style;
    lv_style_init(&bar_style);
    lv_style_set_text_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_bg_color(&bar_style, LV_STATE_DEFAULT, LV_COLOR_GRAY);
    lv_style_set_bg_opa(&bar_style, LV_STATE_DEFAULT, LV_OPA_50);
    lv_style_set_radius(&bar_style, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&bar_style, LV_STATE_DEFAULT, 0);

    static lv_style_t style1;
    lv_style_init(&style1);
    lv_style_set_text_color(&style1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_image_recolor(&style1, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    //status bar
    statusBarCont = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(statusBarCont,  LV_HOR_RES, 30);
    lv_obj_add_style(statusBarCont, LV_OBJ_PART_MAIN, &bar_style);

    sensorLabel = lv_label_create(statusBarCont, NULL);
    lv_label_set_text(sensorLabel, "24*C");
    lv_obj_align(sensorLabel, NULL, LV_ALIGN_IN_LEFT_MID, 20, 0);
    lv_obj_add_style(sensorLabel, LV_OBJ_PART_MAIN, &style1);

    timeLabel = lv_label_create(statusBarCont, NULL);
    lv_label_set_text(timeLabel, "syncing time");
    lv_obj_align(timeLabel, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_style(timeLabel, LV_OBJ_PART_MAIN, &style1);

    wifiLabel = lv_img_create(statusBarCont, NULL);
    lv_img_set_src(wifiLabel, LV_SYMBOL_WIFI);
    lv_obj_align(wifiLabel, NULL, LV_ALIGN_IN_RIGHT_MID, -90, 0);
    lv_obj_add_style(wifiLabel, LV_OBJ_PART_MAIN, &style1);

    lv_obj_t *battery = lv_img_create(statusBarCont, NULL);
    lv_img_set_src(battery,   LV_SYMBOL_BLUETOOTH);
    lv_obj_align(battery, wifiLabel, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_style(battery, LV_OBJ_PART_MAIN, &style1);

    lv_obj_t *ble = lv_img_create(statusBarCont, NULL);
    lv_img_set_src(ble, LV_SYMBOL_BATTERY_FULL);
    lv_obj_align(ble, battery, LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    lv_obj_add_style(ble, LV_OBJ_PART_MAIN, &style1);

    static lv_style_t main_style;
    lv_style_init(&main_style);
    lv_style_set_bg_opa(&main_style, LV_STATE_DEFAULT, LV_OPA_0);
    lv_style_set_radius(&main_style, LV_STATE_DEFAULT, 0);
    lv_style_set_border_width(&main_style, LV_STATE_DEFAULT, 0);

    static lv_style_t font_style;
    lv_style_init(&font_style);
    lv_style_set_text_color(&font_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_style_set_text_font(&font_style, LV_STATE_DEFAULT, &font_outaeg_128);

    mainBarCont  = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(mainBarCont,  LV_HOR_RES, LV_VER_RES - 30);
    lv_obj_add_style(mainBarCont, LV_OBJ_PART_MAIN, &main_style);
    lv_obj_align(mainBarCont, statusBarCont, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    tempLabel = lv_label_create(mainBarCont, NULL);
    lv_label_set_text_fmt(tempLabel, "%u", air_level);
    lv_obj_add_style(tempLabel, LV_OBJ_PART_MAIN, &font_style);
    lv_obj_align(tempLabel, mainBarCont, LV_ALIGN_IN_TOP_RIGHT, -80, 30);

    lv_obj_t *img = lv_img_create(mainBarCont, NULL);
    lv_img_set_src(img, &img_unit);
    lv_obj_align(img, tempLabel, LV_ALIGN_OUT_RIGHT_TOP, 0, 0);

    static lv_style_t btn_style;
    lv_style_init(&btn_style);
    lv_style_set_image_recolor_opa(&btn_style, LV_STATE_PRESSED, LV_OPA_30);
    lv_style_set_image_recolor(&btn_style, LV_STATE_PRESSED, LV_COLOR_BLACK);
    lv_style_set_text_color(&btn_style, LV_STATE_DEFAULT, LV_COLOR_WHITE);

    imgbtn1 = lv_imgbtn_create(mainBarCont, NULL);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_RELEASED, &img_plus);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_PRESSED, &img_plus);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_CHECKED_RELEASED, &img_plus);
    lv_imgbtn_set_src(imgbtn1, LV_BTN_STATE_CHECKED_PRESSED, &img_plus);
    lv_obj_align(imgbtn1, NULL, LV_ALIGN_IN_BOTTOM_MID, 30, -50);
    lv_obj_set_event_cb(imgbtn1,  lv_click_plus_event);

    imgbtn2 = lv_imgbtn_create(mainBarCont, NULL);
    lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_RELEASED, &img_minus);
    lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_PRESSED, &img_minus);
    lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_CHECKED_RELEASED, &img_minus);
    lv_imgbtn_set_src(imgbtn2, LV_BTN_STATE_CHECKED_PRESSED, &img_minus);
    lv_obj_align(imgbtn2, imgbtn1, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_set_event_cb(imgbtn2,  lv_click_minus_event);

    imgbtn3 = lv_imgbtn_create(mainBarCont, NULL);
    lv_imgbtn_set_src(imgbtn3, LV_BTN_STATE_RELEASED, &img_power);
    lv_imgbtn_set_src(imgbtn3, LV_BTN_STATE_PRESSED, &img_power);
    lv_imgbtn_set_src(imgbtn3, LV_BTN_STATE_CHECKED_RELEASED, &img_power);
    lv_imgbtn_set_src(imgbtn3, LV_BTN_STATE_CHECKED_PRESSED, &img_power);
    lv_obj_align(imgbtn3, imgbtn2, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
    lv_obj_set_event_cb(imgbtn3,  lv_click_onoff_event);

    lv_obj_add_style(imgbtn1, LV_IMGBTN_PART_MAIN, &btn_style);
    lv_obj_add_style(imgbtn2, LV_IMGBTN_PART_MAIN, &btn_style);
    lv_obj_add_style(imgbtn3, LV_IMGBTN_PART_MAIN, &btn_style);

    lv_task_create(ctrl_time_update_task, 1000, LV_TASK_PRIO_LOWEST, NULL);
}