#pragma mark - Depend DabbleESP32 library
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/DabbleESP32.git
*/

#include "config.h"
#include <DabbleESP32.h>
#include <esp32PWMUtilities.h>

// Adapt to DRV8833 backplane
// Adapt to DRV8833 backplane
// Adapt to DRV8833 backplane

TTGOClass *ttgo;
TMotor Motor1;
TMotor Motor2;

LV_IMG_DECLARE(CAR_PNG);

#define TMOTOR_MA_CHANNEL      20
#define TMOTOR_MB_CHANNEL      21

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

    pinMode(TWATCH_TQUICK_EN, OUTPUT);
    digitalWrite(TWATCH_TQUICK_EN, true);

    Motor1.attach(TMOTOR_MA_CHANNEL, TWATCH_TQUICK_MA1, TWATCH_TQUICK_MA2);
    Motor2.attach(TMOTOR_MB_CHANNEL, TWATCH_TQUICK_MB1, TWATCH_TQUICK_MB2);
    Dabble.begin("T-Quick");

    lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img, &CAR_PNG);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
}

void loop()
{
    Dabble.processInput();
    lv_task_handler();
    if (GamePad.isUpPressed()) {
        Motor1.moveMotor(-2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
    } else if (GamePad.isDownPressed()) {
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(-2.55 * 100);
    } else if (GamePad.isLeftPressed()) {
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
    } else if (GamePad.isRightPressed()) {
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
    } else {
        Motor1.freeMotor();
        Motor2.freeMotor();
    }
}
