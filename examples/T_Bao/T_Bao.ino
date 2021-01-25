/*
   T_Bao: Steel ball trolley produced by LilyGo
   Copyright 2020 Lewis he
*/
#pragma mark - Depend DabbleESP32 & Adafruit_NeoPixel library
/*
  cd ~/Arduino/libraries
  git clone https://github.com/Xinyuan-LilyGO/DabbleESP32.git
  git clone https://github.com/adafruit/Adafruit_NeoPixel.git
*/
#include <DabbleESP32.h>
#include "config.h"
#include <esp32PWMUtilities.h>
#include <Adafruit_NeoPixel.h>


TTGOClass *ttgo;
TMotor Motor1;
TMotor Motor2;

#define LED_PIN                 4
#define BUZZER_PIN              2
#define SERVO_PIN               14

#define BRIGHTNESS              50
#define LED_COUNT               3

#define TMOTOR_MA_CHANNEL      20
#define TMOTOR_MB_CHANNEL      21
#define TMOTOR_MA5_CHANNEL     8

LV_IMG_DECLARE(icon1);
LV_IMG_DECLARE(icon2);
LV_IMG_DECLARE(icon3);
LV_IMG_DECLARE(icon4);
LV_IMG_DECLARE(icon5);
LV_IMG_DECLARE(icon6);


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

lv_obj_t *img;
bool start = false;

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.setBrightness(255);
    strip.clear();

    strip.setPixelColor(0, strip.Color(50, 0, 0));
    strip.setPixelColor(1, strip.Color(50, 0, 0));
    strip.setPixelColor(2, strip.Color(50, 0, 0));
    strip.setPixelColor(3, strip.Color(50, 0, 0));
    strip.show();

    Motor1.attach(TMOTOR_MA_CHANNEL, TWATCH_TQUICK_MA1, TWATCH_TQUICK_MA2);
    Motor2.attach(TMOTOR_MB_CHANNEL, TWATCH_TQUICK_MB1, TWATCH_TQUICK_MB2);

    srand(time(NULL));
    char buf[128];
    snprintf(buf, 128, "T-Bao-%x:%x", rand() % 0xFF, rand() % 0xFF);

    Dabble.begin(buf);

    ttgo->lvgl_whirling(1);


    img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(img, &icon1);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

    ledcSetup(TMOTOR_MA5_CHANNEL, 1000, 16);
    ledcAttachPin(BUZZER_PIN, TMOTOR_MA5_CHANNEL);
}



void loop()
{
    int pos = 0;

    Dabble.processInput();

    lv_task_handler();

    if (GamePad.isUpPressed()) {
        lv_img_set_src(img, &icon2);
        Motor1.moveMotor(-2.55 * 100);
        Motor2.moveMotor(-2.55 * 100);
        start = true;
    } else if (GamePad.isDownPressed()) {
        lv_img_set_src(img, &icon3);
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isLeftPressed()) {
        lv_img_set_src(img, &icon4);
        Motor1.moveMotor(-2.55 * 100);
//    Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isRightPressed()) {
        lv_img_set_src(img, &icon4);
        Motor1.moveMotor(2.55 * 100);
//    Motor2.moveMotor(-2.55 * 100);
        start = true;
    } else {
        if (start) {
            start = false;
            lv_img_set_src(img, &icon1);
            Motor1.freeMotor();
            Motor2.freeMotor();
        }
    }
    if (GamePad.isStartPressed()) {
        Serial.println("isStartPressed");
        strip.clear();
        strip.show();
    } else if (GamePad.isSelectPressed()) {
        Serial.println("isSelectPressed");
        uint16_t color = rand() % 0xFFFF;
        for (int i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, color);
            strip.show();
        }
        delay(500);
    }

    if (GamePad.isTrianglePressed()) {
        Serial.println("isTrianglePressed");
        static Ticker t;
        ledcWrite(TMOTOR_MA5_CHANNEL, 1000);
        t.once_ms(150, []() {
            ledcWrite(TMOTOR_MA5_CHANNEL,  0 );
        });
    } else if (GamePad.isCrossPressed()) {
        Serial.println("isCrossPressed");
    }

    if (GamePad.isSquarePressed()) {
        Serial.println("isSquarePressed");
        if (Controls.angleServo1 <= 0) {
            return;
        }
        for (pos = 45; pos >= 0; pos -= 1) {
            Controls.angleServo1 = pos;
            Controls.runServo1(SERVO_PIN);
            delay(15);
        }
    } else if (GamePad.isCirclePressed()) {
        Serial.println("isCirclePressed");
        if (Controls.angleServo1 >= 45) {
            return;
        }
        for (pos = 0; pos <= 45; pos += 1) {
            Controls.angleServo1 = pos;
            Controls.runServo1(SERVO_PIN);
            delay(15);
        }
    }
}
