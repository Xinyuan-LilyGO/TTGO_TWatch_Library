#include "config.h"

TTGOClass   *ttgo = nullptr;
lv_chart_series_t *ser1;
lv_obj_t    *chart;

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    ttgo->openBL();
    ttgo->lvgl_begin();

    /*Create a chart*/
    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 200, 200);
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart,  LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_range(chart, 0, 100);

    /*Add two data series*/
    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);

    /*Set the next points on 'dl1'*/
    lv_chart_set_next(chart, ser1, 10);
}

uint32_t lastMillis = 0;

void loop()
{
    if (millis() - lastMillis > 200) {
        lastMillis = millis();
        //Read the analog value of IO25 and display it in series
        uint16_t data = analogRead(25);
        lv_chart_set_next(chart, ser1, map(data, 0, 4095, 0, 100));
    }
    lv_task_handler();
    delay(5);
}
