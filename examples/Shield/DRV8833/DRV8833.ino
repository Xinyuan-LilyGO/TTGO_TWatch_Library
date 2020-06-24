
#include "config.h"

TTGOClass *ttgo;

const int SLEEP = 2;

// motor outputs
const int MA1 = 25;
const int MA2 = 26;
const int MB1 = 13;
const int MB2 = 15;

// setting PWM properties
const int A1Ch = 4;
const int A2Ch = 5;
const int B1Ch = 6;
const int B2Ch = 7;


lv_obj_t *btn1;
lv_obj_t *btn2;

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_PRESSED) {
        if (obj == btn1) {
            ledcWriteTone(A1Ch, 255);
            ledcWriteTone(A2Ch, 0);

        } else if (obj == btn2) {
            ledcWriteTone(B1Ch, 255);
            ledcWriteTone(B2Ch, 0);
        }
    } else if (event == LV_EVENT_PRESS_LOST || event == LV_EVENT_RELEASED) {
        if (obj == btn1) {
            ledcWriteTone(A1Ch, 0);
            ledcWriteTone(A2Ch, 0);

        } else if (obj == btn2) {
            ledcWriteTone(B1Ch, 0);
            ledcWriteTone(B2Ch, 0);
        }
    }
}

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    ttgo->lvgl_begin();

    lv_obj_t *label;
    btn1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn1, event_handler);
    lv_obj_align(btn1, NULL, LV_ALIGN_CENTER, 0, -40);

    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Motro R");

    btn2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn2, event_handler);
    lv_obj_align(btn2, NULL, LV_ALIGN_CENTER, 0, 40);

    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Motro L");

    // Pull up sleep enable
    pinMode(SLEEP, OUTPUT);
    digitalWrite(SLEEP, HIGH);

    // Drive it with pwm
    ledcAttachPin(MA1, A1Ch);
    ledcAttachPin(MA2, A2Ch);
    ledcAttachPin(MB1, B1Ch);
    ledcAttachPin(MB2, B2Ch);

    ledcSetup(A1Ch, 1000, 8);
    ledcSetup(A2Ch, 1000, 8);
    ledcSetup(B1Ch, 1000, 8);
    ledcSetup(B2Ch, 1000, 8);
}


void loop()
{
    lv_task_handler();
    delay(5);
}
