/*
*  In this example, the demonstration uses PNG image switching to make charging animation
*  Need to upload the content of the data file to SPIFFS
* Copyright 2020 Lewis he
*/
#include "config.h"

TTGOClass *ttgo;
AXP20X_Class *power;

bool irq = false;
lv_obj_t *label = nullptr;
lv_obj_t *img = nullptr;
lv_obj_t *chargelabel = nullptr;
lv_task_t *task = nullptr;

static void lv_power_monitor_task(lv_task_t *t);

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    ttgo->lvgl_begin();

    power = ttgo->power;

    pinMode(AXP202_INT, INPUT);
    attachInterrupt(AXP202_INT, [] {
        irq = true;
    }, FALLING);

    // ADC monitoring must be enabled to use the AXP202 monitoring function
    power->adc1Enable(AXP202_VBUS_VOL_ADC1
                      | AXP202_VBUS_CUR_ADC1
                      | AXP202_BATT_CUR_ADC1
                      | AXP202_BATT_VOL_ADC1,
                      true);

    power->enableIRQ(AXP202_PEK_SHORTPRESS_IRQ
                     | AXP202_VBUS_REMOVED_IRQ
                     | AXP202_VBUS_CONNECT_IRQ
                     | AXP202_CHARGING_IRQ, true
                    );

    power->clearIRQ();

    lv_obj_set_style_local_bg_color(lv_scr_act(), LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    label = lv_label_create(lv_scr_act(), NULL);
    lv_obj_set_style_local_text_color(label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_label_set_long_mode(label,   LV_LABEL_LONG_BREAK);
    lv_label_set_text(label, "Please unplug the USB plug  and plug it in again, you will be able to see a simple animation made with PNG images");
    lv_obj_set_width(label, 239);
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

}

#define IMG_POWER_COUNT     4

static void lv_power_monitor_task(lv_task_t *t)
{
    static uint8_t imgIndex = 0;
    char buf[128];
    lv_obj_t *obj = (lv_obj_t *)t->user_data;
    snprintf(buf, 128, "P:/%u.png", imgIndex++);
    lv_img_set_src(obj, buf);
    imgIndex %= IMG_POWER_COUNT;

    if (!chargelabel) {
        chargelabel  = lv_label_create(lv_scr_act(), NULL);
    }
    snprintf(buf, 128, "V:%.2f V C:%.2fmA", power->getVbusVoltage() / 1000.0
             , power->getVbusCurrent());
    lv_label_set_text(chargelabel, buf);
    lv_obj_align(chargelabel, obj, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);
}

void loop()
{
    if (irq) {
        irq = false;
        power->readIRQ();
        // Assuming that the USB is plugged in and set to charging
        if (power->isVbusPlugInIRQ()) {
            if (!img) {
                lv_obj_set_hidden(label, true);
                ttgo->setBrightness(65);
                img   = lv_img_create(lv_scr_act(), NULL);
                lv_img_set_src(img, "P:/0.png");
                lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, - 10);
                task =   lv_task_create(lv_power_monitor_task, 1000, LV_TASK_PRIO_MID, img);
            }
        }
        if (power->isVbusRemoveIRQ()) {
            if (img) {
                lv_task_del(task);
                lv_obj_del(chargelabel);
                lv_obj_del(img);
                img = nullptr;
                chargelabel = nullptr;
                task = nullptr;
                lv_obj_set_hidden(label, false);
            }
        }
        power->clearIRQ();
    }
    lv_task_handler();
    delay(5);
}
