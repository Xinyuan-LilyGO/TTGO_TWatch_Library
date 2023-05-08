/**
 * @file      IRsendDemo.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-29
 *
 */

#include <Arduino.h>
#include <IRsend.h>
#include <LilyGoLib.h>
#include <LV_Helper.h>


IRsend irsend(BOARD_IR_PIN); // T-Watch S3 GPIO2 pin to use.

//lvgl event handler btn index
uint8_t btn_index[4] = {0, 1, 2, 3};

// Example of data captured by IRrecvDumpV2.ino
uint16_t rawData[67] = {9000, 4500, 650, 550, 650, 1650, 600, 550, 650, 550,
                        600, 1650, 650, 550, 600, 1650, 650, 1650, 650, 1650,
                        600, 550, 650, 1650, 650, 1650, 650, 550, 600, 1650,
                        650, 1650, 650, 550, 650, 550, 650, 1650, 650, 550,
                        650, 550, 650, 550, 600, 550, 650, 550, 650, 550,
                        650, 1650, 600, 550, 650, 1650, 650, 1650, 650, 1650,
                        650, 1650, 650, 1650, 650, 1650, 600
                       };
// Example Samsung A/C state captured from IRrecvDumpV2.ino
uint8_t samsungState[kSamsungAcStateLength] = {
    0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
    0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0
};


void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t btn_index = *(uint8_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        switch (btn_index) {
        case 0:
            Serial.println("NEC");
            irsend.sendNEC(0x00FFE01FUL);
            break;
        case 1:
            Serial.println("Sony");
            irsend.sendSony(0xa90, 12, 2);  // 12 bits & 2 repeats
            break;
        case 2:
            Serial.println("a rawData capture from IRrecvDumpV2");
            irsend.sendRaw(rawData, 67, 38);  // Send a raw data capture at 38kHz.
            break;
        case 3:
            Serial.println("a Samsung A/C state from IRrecvDumpV2");
            irsend.sendSamsungAC(samsungState);
            break;
        default:
            break;
        }
    }
}

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    irsend.begin();

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);

    //Send NEC Button
    lv_obj_t *btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[0]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    lv_obj_t *label;
    label = lv_label_create(btn1);
    lv_label_set_text(label, "Send NEC");
    lv_obj_center(label);

    //Send Sony Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[1]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Send Sony");
    lv_obj_center(label);

    //Send rawData Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[2]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Send rawData");
    lv_obj_center(label);

    //Send rawData Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[3]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Samsung A/C state");
    lv_obj_center(label);
}

void loop()
{
    lv_task_handler();
    delay(5);
}


