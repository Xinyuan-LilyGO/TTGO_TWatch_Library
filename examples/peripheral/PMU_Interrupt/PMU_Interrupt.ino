/**
 * @file      PMU_Interrupt.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-28
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

bool  pmu_flag = 0;
lv_obj_t *label1;

void setFlag(void)
{
    pmu_flag = true;
}

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_recolor(label1, true);    /*Enable re-coloring by commands in the text*/
    lv_obj_center(label1);

    watch.attachPMU(setFlag);

    // Disable all interrupts
    watch.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);

    // Print interrupt register
    watch.printIntRegister(&Serial);

    // Clear all interrupt flags
    watch.clearPMU();

    // Enable the required interrupt function
    watch.enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
        // XPOWERS_AXP2101_PKEY_NEGATIVE_IRQ | XPOWERS_AXP2101_PKEY_POSITIVE_IRQ   |   //POWER KEY
    );

    watch.enableIRQ(XPOWERS_AXP2101_BAT_NOR_UNDER_TEMP_IRQ);

    watch.enableIRQ(XPOWERS_AXP2101_PKEY_SHORT_IRQ | XPOWERS_AXP2101_PKEY_NEGATIVE_IRQ);

    watch.enableIRQ(XPOWERS_AXP2101_BAT_OVER_VOL_IRQ);
}

void loop()
{

    if (pmu_flag) {

        pmu_flag = false;

        // Get watch Interrupt Status Register
        uint32_t status = watch.readPMU();
        Serial.print("STATUS => HEX:");
        Serial.print(status, HEX);
        Serial.print(" BIN:");
        Serial.println(status, BIN);

        if (watch.isDropWarningLevel2Irq()) {
            Serial.println("isDropWarningLevel2");
            lv_label_set_text(label1, "isDropWarningLevel2");
        }
        if (watch.isDropWarningLevel1Irq()) {
            Serial.println("isDropWarningLevel1");
            lv_label_set_text(label1, "isDropWarningLevel1");
        }
        if (watch.isGaugeWdtTimeoutIrq()) {
            Serial.println("isWdtTimeout");
            lv_label_set_text(label1, "isWdtTimeout");
        }
        if (watch.isBatChargerOverTemperatureIrq()) {
            Serial.println("isBatChargeOverTemperature");
            lv_label_set_text(label1, "isBatChargeOverTemperature");
        }
        if (watch.isBatWorkOverTemperatureIrq()) {
            Serial.println("isBatWorkOverTemperature");
            lv_label_set_text(label1, "isBatWorkOverTemperature");
        }
        if (watch.isBatWorkUnderTemperatureIrq()) {
            Serial.println("isBatWorkUnderTemperature");
            lv_label_set_text(label1, "isBatWorkUnderTemperature");
        }
        if (watch.isVbusInsertIrq()) {
            Serial.println("isVbusInsert");
            lv_label_set_text(label1, "isVbusInsert");
        }
        if (watch.isVbusRemoveIrq()) {
            Serial.println("isVbusRemove");
            lv_label_set_text(label1, "isVbusRemove");
        }
        if (watch.isBatInsertIrq()) {
            Serial.println("isBatInsert");
            lv_label_set_text(label1, "isBatInsert");
        }
        if (watch.isBatRemoveIrq()) {
            Serial.println("isBatRemove");
            lv_label_set_text(label1, "isBatRemove");
        }
        if (watch.isPekeyShortPressIrq()) {
            Serial.println("isPekeyShortPress");
            lv_label_set_text(label1, "isPekeyShortPress");
        }
        if (watch.isPekeyLongPressIrq()) {
            Serial.println("isPekeyLongPress");
            lv_label_set_text(label1, "isPekeyLongPress");
        }
        if (watch.isPekeyNegativeIrq()) {
            Serial.println("isPekeyNegative");
            lv_label_set_text(label1, "isPekeyNegative");
        }
        if (watch.isPekeyPositiveIrq()) {
            Serial.println("isPekeyPositive");
            lv_label_set_text(label1, "isPekeyPositive");
        }
        if (watch.isWdtExpireIrq()) {
            Serial.println("isWdtExpire");
            lv_label_set_text(label1, "isWdtExpire");
        }
        if (watch.isLdoOverCurrentIrq()) {
            Serial.println("isLdoOverCurrentIrq");
            lv_label_set_text(label1, "isLdoOverCurrentIrq");
        }
        if (watch.isBatfetOverCurrentIrq()) {
            Serial.println("isBatfetOverCurrentIrq");
            lv_label_set_text(label1, "isBatfetOverCurrentIrq");
        }
        if (watch.isBatChagerDoneIrq()) {
            Serial.println("isBatChagerDone");
            lv_label_set_text(label1, "isBatChagerDone");
        }
        if (watch.isBatChagerStartIrq()) {
            Serial.println("isBatChagerStart");
            lv_label_set_text(label1, "isBatChagerStart");
        }
        if (watch.isBatDieOverTemperatureIrq()) {
            Serial.println("isBatDieOverTemperature");
            lv_label_set_text(label1, "isBatDieOverTemperature");
        }
        if (watch.isChagerOverTimeoutIrq()) {
            Serial.println("isChagerOverTimeout");
            lv_label_set_text(label1, "isChagerOverTimeout");
        }
        if (watch.isBatOverVoltageIrq()) {
            Serial.println("isBatOverVoltage");
            lv_label_set_text(label1, "isBatOverVoltage");
        }

        // Clear watch Interrupt Status Register
        watch.clearPMU();

        // Print AXP2101 interrupt control register
        watch.printIntRegister(&Serial);

    }
    lv_task_handler();
    delay(5);
}

