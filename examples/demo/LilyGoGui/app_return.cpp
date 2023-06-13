#include "app_return.h"
#include "Arduino.h"

extern lv_obj_t *setupGUI();

void app_return_load(lv_obj_t *cont) {

    lv_obj_t* obj = lv_obj_get_parent(cont);
    if(obj != NULL)
    {
        lv_obj_t* obj_parent = lv_obj_get_parent(obj);
        if(obj_parent != NULL)
        {
            lv_obj_del_delayed(obj, 1);
            setupGUI();
        }
    }
}


app_t app_return = {
    .setup_func_cb = app_return_load,
    .exit_func_cb = nullptr,
    .user_data= nullptr,
};