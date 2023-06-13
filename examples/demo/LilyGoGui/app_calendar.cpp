#include "app_calendar.h"
#include "Arduino.h"

extern void lv_example_calendar_1(lv_obj_t *parent);

void app_calendar_load(lv_obj_t *cont) {
	  lv_example_calendar_1(cont);
}

app_t app_calendar = {
    .setup_func_cb = app_calendar_load,
    .exit_func_cb = nullptr,
    .user_data= nullptr,
};