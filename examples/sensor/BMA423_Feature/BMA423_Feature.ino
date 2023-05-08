/**
 * @file      BMA423_Feature.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */

#include <LilyGoLib.h>
#include <LV_Helper.h>

bool sensorIRQ = false;
bool pmuIRQ = false;
uint32_t lastMillis;
lv_obj_t *label1;
lv_obj_t *label2;
lv_obj_t *label3;

void setFlag()
{
    sensorIRQ = true;
}

void setPMUFlag()
{
    pmuIRQ = true;
}

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Default 4G ,200HZ
    watch.configAccelerometer();

    watch.enableAccelerometer();

    watch.enablePedometer();

    watch.configInterrupt();


    watch.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
                        SensorBMA423::FEATURE_ANY_MOTION |
                        SensorBMA423::FEATURE_NO_MOTION |
                        SensorBMA423::FEATURE_ACTIVITY |
                        SensorBMA423::FEATURE_TILT |
                        SensorBMA423::FEATURE_WAKEUP,
                        true);

    watch.enablePedometerIRQ();
    watch.enableTiltIRQ();
    watch.enableWakeupIRQ();
    watch.enableAnyNoMotionIRQ();
    watch.enableActivityIRQ();


    watch.attachBMA(setFlag);

    watch.attachPMU(setPMUFlag);

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);

    label1 = lv_label_create(cont);
    lv_label_set_text(label1, "INT MASK:0x00");

    label2 = lv_label_create(cont);
    lv_label_set_text(label2, "STEP COUNTER:0");

    label3 = lv_label_create(cont);
    lv_label_set_text(label3, "Feature: null");
}


void loop()
{
    // Pressing the crown button will clear the step counting data
    if (pmuIRQ) {
        watch.readPMU();
        if (watch.isPekeyShortPressIrq()) {
            watch.resetPedometer();
            lv_label_set_text_fmt(label2, "[%lu]STEP COUNTER:%u", millis() / 1000, 0);
        }
        watch.clearPMU();
    }
    if (sensorIRQ) {
        sensorIRQ = false;
        // The interrupt status must be read after an interrupt is detected
        uint16_t status =   watch.readBMA();
        Serial.printf("Accelerometer interrupt mask : 0x%x\n", status);
        lv_label_set_text_fmt(label1, "[%lu]INT MASK:0x%x", millis() / 1000, status);

        if (watch.isPedometer()) {
            uint32_t stepCounter = watch.getPedometerCounter();
            Serial.printf("Step count interrupt,step Counter:%u\n", stepCounter);
            lv_label_set_text_fmt(label2, "[%lu]STEP COUNTER:%u", millis() / 1000, stepCounter);
        }
        if (watch.isActivity()) {
            Serial.println("Activity interrupt");
            lv_label_set_text_fmt(label3, "[%lu]Feature: Activity", millis() / 1000);
        }
        if (watch.isTilt()) {
            Serial.println("Tilt interrupt");
            lv_label_set_text_fmt(label3, "[%lu]Feature: Tilt", millis() / 1000);
        }
        if (watch.isDoubleTap()) {
            Serial.println("DoubleTap interrupt");
            lv_label_set_text_fmt(label3, "[%lu]Feature: DoubleTap", millis() / 1000);
        }
        if (watch.isAnyNoMotion()) {
            Serial.println("Any motion / no motion interrupt");
            lv_label_set_text_fmt(label3, "[%lu]Feature: Any motion/NoMotion", millis() / 1000);
        }
    }
    lv_task_handler();
    delay(5);
}


