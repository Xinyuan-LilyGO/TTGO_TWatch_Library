/**
 * @file      btnmatrix.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>

extern "C" {
    void lv_example_btnmatrix_1(void);
    void lv_example_btnmatrix_2(void);
    void lv_example_btnmatrix_3(void);
};

void setup()
{
    Serial.begin(115200);

    watch.begin();

    beginLvglHelper();

    //Tips : Select a separate function to see the effect
    lv_example_btnmatrix_1();
    // lv_example_btnmatrix_2();
    // lv_example_btnmatrix_3();
}


void loop()
{
    lv_task_handler();
    delay(5);
}
