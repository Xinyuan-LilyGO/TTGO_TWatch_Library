/*
 * T_Quick: Steel ball trolley produced by LilyGo
 * Copyright 2020 Lewis he
 */

#pragma mark - Depend DabbleESP32 library
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/DabbleESP32.git
*/
#include <DabbleESP32.h>
#include "config.h"
#include <esp32PWMUtilities.h>


TTGOClass *ttgo;
TMotor Motor1;
TMotor Motor2;

#define TMOTOR_MA_CHANNEL      20
#define TMOTOR_MB_CHANNEL      21

LV_IMG_DECLARE(icon1);
LV_IMG_DECLARE(icon2);
LV_IMG_DECLARE(icon3);
LV_IMG_DECLARE(icon4);
LV_IMG_DECLARE(icon5);
LV_IMG_DECLARE(icon6);



lv_obj_t *img;

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

    pinMode(TWATCH_TQUICK_EN, OUTPUT);
    digitalWrite(TWATCH_TQUICK_EN, HIGH);

    Motor1.attach(TMOTOR_MA_CHANNEL, TWATCH_TQUICK_MA1, TWATCH_TQUICK_MA2);
    Motor2.attach(TMOTOR_MB_CHANNEL, TWATCH_TQUICK_MB1, TWATCH_TQUICK_MB2);

    srand(time(NULL));
    char buf[128];
    snprintf(buf, 128, "T-Quick-%x:%x", rand() % 0xFF, rand() % 0xFF);
    Dabble.begin(buf);

    ttgo->lvgl_whirling(1);

    img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img, &icon1);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}


bool start = false;

void loop()
{
    int pos = 0;

    Dabble.processInput();

    lv_task_handler();

    if (GamePad.isUpPressed()) {
        lv_img_set_src(img, &icon2);
        Motor1.moveMotor(-2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isDownPressed()) {
        lv_img_set_src(img, &icon3);
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(-2.55 * 100);
        start = true;
    } else if (GamePad.isLeftPressed()) {
        lv_img_set_src(img, &icon4);
        Motor1.freeMotor();
        Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isRightPressed()) {
        lv_img_set_src(img, &icon6);
        Motor1.moveMotor(-2.55 * 100);
        Motor2.freeMotor();
        start = true;
    } else {
        if (start) {
            start = false;
            lv_img_set_src(img, &icon5);
            Motor1.freeMotor();
            Motor2.freeMotor();
        }
    }
}


