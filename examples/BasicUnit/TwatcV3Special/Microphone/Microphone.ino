/**
 *  This sample only applies to TWATCH 2020 V3 and only tests PDM microphones
 */
#include "config.h"
#include <driver/i2s.h>

#define BUFFER_SIZE (2*1024)

// TWATCH 2020 V3 PDM microphone pin
#define MIC_DATA            2
#define MIC_CLOCK           0

uint8_t buffer[BUFFER_SIZE] = {0};

TTGOClass           *ttgo = nullptr;
lv_obj_t            *chart = nullptr;
lv_chart_series_t   *ser1 = nullptr;
const int           define_max = 200;
const int           define_avg = 15;
const int           define_zero = 3000;
float               val_avg = 0;
float               val_avg_1 = 0;
float               all_val_avg = 0;

uint8_t             val1, val2;
int16_t             val16 = 0;
int16_t             val_max = 0;
int16_t             val_max_1 = 0;
int32_t             all_val_zero1 = 0;
int32_t             all_val_zero2 = 0;
int32_t             all_val_zero3 = 0;
uint32_t            j = 0;

void setup()
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    ttgo->lvgl_begin();

    lv_obj_t *text = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(text, "PDM Microphone Test");
    lv_obj_align(text, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    chart = lv_chart_create(lv_scr_act(), NULL);
    lv_obj_set_size(chart, 200, 150);
    lv_obj_align(chart, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_chart_set_type(chart,  LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    lv_chart_set_range(chart, 0, 800);

    ser1 = lv_chart_add_series(chart, LV_COLOR_RED);

    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate =  44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t i2s_cfg;
    i2s_cfg.bck_io_num   = I2S_PIN_NO_CHANGE;
    i2s_cfg.ws_io_num    = MIC_CLOCK;
    i2s_cfg.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_cfg.data_in_num  = MIC_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_cfg);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

void loop()
{
    size_t read_len = 0;
    j = j + 1;
    i2s_read(I2S_NUM_0, (char *) buffer, BUFFER_SIZE, &read_len, portMAX_DELAY);
    for (int i = 0; i < BUFFER_SIZE / 2 ; i++) {
        val1 = buffer[i * 2];
        val2 = buffer[i * 2 + 1] ;
        val16 = val1 + val2 *  256;
        if (val16 > 0) {
            val_avg = val_avg + val16;
            val_max = max( val_max, val16);
        }
        if (val16 < 0) {
            val_avg_1 = val_avg_1 + val16;
            val_max_1 = min( val_max_1, val16);
        }
        all_val_avg = all_val_avg + val16;
        if (abs(val16) >= 20)
            all_val_zero1 = all_val_zero1 + 1;
        if (abs(val16) >= 15)
            all_val_zero2 = all_val_zero2 + 1;
        if (abs(val16) > 5)
            all_val_zero3 = all_val_zero3 + 1;
    }

    if (j % 2 == 0 && j > 0) {
        val_avg = val_avg / BUFFER_SIZE ;
        val_avg_1 = val_avg_1 / BUFFER_SIZE;
        all_val_avg = all_val_avg / BUFFER_SIZE ;

        lv_chart_set_next(chart, ser1, val_avg);

        val_avg = 0;
        val_max = 0;

        val_avg_1 = 0;
        val_max_1 = 0;

        all_val_avg = 0;
        all_val_zero1 = 0;
        all_val_zero2 = 0;
        all_val_zero3 = 0;
    }
    lv_task_handler();
    delay(5);
}
