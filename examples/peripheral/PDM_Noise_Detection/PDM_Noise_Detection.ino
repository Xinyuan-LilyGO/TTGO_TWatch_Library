/**
 * @file      PDM_Noise_Detection.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xin Yuan Electronic Technology Co., Ltd
 * @date      2023-04-23
 *
 */

#include <LilyGoLib.h>
#include <LV_Helper.h>
#include "esp_vad.h"

#define VAD_FRAME_LENGTH_MS             30
#define VAD_BUFFER_LENGTH               (VAD_FRAME_LENGTH_MS * MIC_I2S_SAMPLE_RATE / 1000)

int16_t *vad_buff;
vad_handle_t vad_inst;
lv_obj_t *label1;
uint32_t detectCount = 0;

void setup(void)
{
    Serial.begin(115200);

    watch.begin();

    // Initialize onboard PDM microphone
    watch.initMicrophone();

    beginLvglHelper();

    label1 = lv_label_create(lv_scr_act());
    lv_label_set_text(label1, "----");
    lv_obj_center(label1);



    // Initialize esp-sr vad detected
#if ESP_IDF_VERSION_VAL(4,4,1) == ESP_IDF_VERSION
    vad_inst = vad_create(VAD_MODE_0, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
#elif  ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4,4,1)
    vad_inst = vad_create(VAD_MODE_0);
#else
#error "No support this version."
#endif
    vad_buff = (int16_t *)malloc(VAD_BUFFER_LENGTH * sizeof(short));
    if (vad_buff == NULL) {
        Serial.println("Memory allocation failed!");
        while (1) {
            delay(1000);
        }
    }
}

void loop()
{
    size_t read_len;

    if (watch.readMicrophone((char *) vad_buff, VAD_BUFFER_LENGTH * sizeof(short), &read_len)) {
        // Feed samples to the VAD process and get the result
#if   ESP_IDF_VERSION_VAL(4,4,1) == ESP_IDF_VERSION
        vad_state_t vad_state = vad_process(vad_inst, vad_buff);
#elif ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4,4,1)
        vad_state_t vad_state = vad_process(vad_inst, vad_buff, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
#else
#error "No support this version."
#endif
        if (vad_state == VAD_SPEECH) {
            Serial.print(millis());
            Serial.println(" Noise detected!!!");
            lv_label_set_text_fmt(label1, "Noise detected\nDetect Count:%lu", detectCount++);
        }
    }
    lv_task_handler();
    delay(5);
}

