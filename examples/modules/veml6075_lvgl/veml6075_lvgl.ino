#pragma mark - Depend SparkFun_VEML6075_Arduino_Library
/*
cd ~/Arduino/libraries
git clone https://github.com/sparkfun/SparkFun_VEML6075_Arduino_Library.git
*/

#include <TTGO.h>
#include <Wire.h>
#include <SparkFun_VEML6075_Arduino_Library.h>
TTGOClass *ttgo;
TFT_eSPI *tft ;
VEML6075 uv;

lv_obj_t *cont = NULL;
lv_obj_t *uvA = NULL;
lv_obj_t *uvB = NULL;
lv_obj_t *uvIndex = NULL;

LV_IMG_DECLARE(BG_PNG);
LV_IMG_DECLARE(ERR_PNG);

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    tft = ttgo->eTFT;

    tft->fillScreen(TFT_BLACK);

    ttgo->lvgl_begin();

    ttgo->openBL();

    Wire1.begin(25, 26);

    cont = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(cont,  LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style(cont, &lv_style_transp_fit);

    lv_obj_t *err = NULL;
    bool ret = false;
    do {
        ret = uv.begin(Wire1);
        if (!ret && !err) {
            lv_obj_t *img = lv_img_create(cont, NULL);
            lv_img_set_src(img, &ERR_PNG);
            lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
            err = lv_label_create(cont, NULL);
            lv_label_set_text(err, "Could not find VEML6075");
            lv_obj_align(err, img, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        }
        Serial.println("Could not find a valid VEML6075 sensor, check wiring!");
        delay(1000);
    } while (!ret);

    if (err) {
        lv_obj_clean(cont);
    }
    lv_obj_t *mountain = lv_img_create(cont, NULL);
    lv_img_set_src(mountain, &BG_PNG);
    lv_obj_align(mountain, NULL, LV_ALIGN_CENTER, 0, 0);
    uvA = lv_label_create(cont, NULL);
    uvB = lv_label_create(cont, NULL);
    uvIndex = lv_label_create(cont, NULL);
    lv_task_create(sensor_task, 1000, LV_TASK_PRIO_LOW, NULL);

}

static void sensor_task(lv_task_t *param)
{
    char buffer[128];
    snprintf(buffer, sizeof(buffer), "uvA:%s", String(uv.uva()).c_str());
    lv_label_set_text(uvA, buffer);
    lv_obj_align(uvA, cont, LV_ALIGN_CENTER, 40, -15);

    snprintf(buffer, sizeof(buffer), "uvB:%s", String(uv.uvb()).c_str());
    lv_label_set_text(uvB, buffer);
    lv_obj_align(uvB, uvA, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    snprintf(buffer, sizeof(buffer), "uvIndex:%s", String(uv.index()).c_str());
    lv_label_set_text(uvIndex, buffer);
    lv_obj_align(uvIndex, uvB, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
}

void loop(void)
{
    lv_task_handler();
    delay(5);
}




