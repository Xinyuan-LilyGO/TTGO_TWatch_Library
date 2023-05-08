/**
 * @file      PMU_ADC.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

bool  pmu_flag = 0;
lv_obj_t *label1;
uint32_t lastMillis;

const char *chg_status[] = {
    "Tri ",
    "Pre",
    "Constant current",
    "Constant voltage",
    "Charge done",
    "No charge"
};

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Enable or Disable PMU Feature
    watch.enableBattDetection();
    // watch.disableBattDetection();

    watch.enableVbusVoltageMeasure();
    // watch.disableVbusVoltageMeasure();

    watch.enableBattVoltageMeasure();
    // watch.disableBattVoltageMeasure();

    watch.enableSystemVoltageMeasure();
    // watch.disableSystemVoltageMeasure();

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_obj_center(label1);
}

void loop()
{
    if (lastMillis < millis()) {
        uint8_t charge_status = watch.getChargerStatus();
        lv_label_set_text_fmt(label1, "Charging:%s\nDischarge:%s\nUSB PlugIn:%s\nCHG state:%s\nBattery Voltage:%u mV\n USB Voltage:%u mV\n SYS Voltage:%u mV\n Battery Percent:%d%%",
                              watch.isCharging() ? "#00ff00 YES" : "#ff0000 NO",
                              watch.isDischarge() ? "#00ff00 YES" : "#ff0000 NO",
                              watch.isVbusIn() ? "#00ff00 YES" : "#ff0000 NO",
                              chg_status[charge_status],
                              watch.getBattVoltage(),
                              watch.getVbusVoltage(),
                              watch.getSystemVoltage(),
                              watch.getBatteryPercent()
                             );
        lastMillis = millis() + 1000;
    }
    lv_task_handler();
    delay(5);
}

