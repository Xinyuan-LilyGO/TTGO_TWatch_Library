/**
 * @file      PlayMusicFromFATFS.ino
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-30
 *
 */
#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourceFATFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioFileSourceFunction.h>
#include <AudioGeneratorFLAC.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioGeneratorAAC.h>

// It is recommended to use I2S channel 1.
// If the PDM microphone is turned on,
// the decoder must use channel 1 because PDM can only use channel 1
const uint8_t i2sPort = 1;

AudioGeneratorWAV       *wav = NULL;
AudioFileSourceFATFS   *file = NULL;
AudioOutputI2S          *out = NULL;
AudioGeneratorMP3       *mp3 = NULL;
AudioFileSourceID3      *id3 = NULL;
AudioGeneratorFLAC      *flac = NULL;
AudioGeneratorAAC       *aac = NULL;

//lvgl event handler btn index
uint8_t btn_index[4] = {0, 1, 2, 3};

// Play task handler
TaskHandle_t playMP3Handler;
TaskHandle_t playWAVHandler;
TaskHandle_t playFLACHandler;
TaskHandle_t playACCHandler;


void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    uint8_t btn_index = *(uint8_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        switch (btn_index) {
        case 0:
            vTaskSuspend(playWAVHandler);
            vTaskSuspend(playMP3Handler);
            vTaskSuspend(playFLACHandler);
            vTaskSuspend(playACCHandler);
            file->open("/wav_file.wav");
            wav->begin(file, out);
            vTaskResume(playWAVHandler);
            break;
        case 1:
            vTaskSuspend(playWAVHandler);
            vTaskSuspend(playMP3Handler);
            vTaskSuspend(playFLACHandler);
            vTaskSuspend(playACCHandler);
            file->open("/mp3_file.mp3");
            mp3->begin(id3, out);
            vTaskResume(playMP3Handler);
            break;
        case 2:
            vTaskSuspend(playWAVHandler);
            vTaskSuspend(playMP3Handler);
            vTaskSuspend(playFLACHandler);
            vTaskSuspend(playACCHandler);
            file->open("/flac_file.flac");
            flac->begin(file, out);
            vTaskResume(playFLACHandler);
            break;
        case 3:
            vTaskSuspend(playWAVHandler);
            vTaskSuspend(playMP3Handler);
            vTaskSuspend(playFLACHandler);
            vTaskSuspend(playACCHandler);
            file->open("aac_file.aac");
            aac->begin(file, out);
            vTaskResume(playACCHandler);
            break;
        default:
            break;
        }
    }
}

void playWavTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (wav->isRunning()) {
            if (!wav->loop()) {
                wav->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void playMP3Task(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (mp3->isRunning()) {
            if (!mp3->loop()) {
                mp3->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void playFLACTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (flac->isRunning()) {
            if (!flac->loop()) {
                flac->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void playACCTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (aac->isRunning()) {
            if (!aac->loop()) {
                aac->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void setup(void)
{
    Serial.begin(115200);

    // When initializing the watch,
    // FFat has already been initialized internally
    watch.begin();

    beginLvglHelper();

    lv_obj_t *cont = lv_obj_create(lv_scr_act());
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_scroll_dir(cont, LV_DIR_VER);
    lv_obj_set_size(cont, LV_PCT(100), LV_PCT(100));
    lv_obj_center(cont);


    lv_obj_t *label;

    label = lv_label_create(cont);
    lv_label_set_text(label, "PlayMusicFromFFat");

    //Play wav Button
    lv_obj_t *btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[0]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Play WAV");
    lv_obj_center(label);

    //Play mp3 Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[1]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Play MP3");
    lv_obj_center(label);

    //Play flac Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[2]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Play FLAC");
    lv_obj_center(label);

    //Play aac Button
    btn1 = lv_btn_create(cont);
    lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, &btn_index[3]);
    lv_obj_set_width(btn1,  LV_PCT(100));

    label = lv_label_create(btn1);
    lv_label_set_text(label, "Play AAC");
    lv_obj_center(label);

    file = new AudioFileSourceFATFS();

    // Set up the use of an external decoder
    out = new AudioOutputI2S(i2sPort, AudioOutputI2S::EXTERNAL_I2S);

    // Set up hardware connection
    out->SetPinout(BOARD_DAC_IIS_BCK, BOARD_DAC_IIS_WS, BOARD_DAC_IIS_DOUT);

    //Adjust to appropriate gain
    out->SetGain(0.2);

    wav = new AudioGeneratorWAV();

    id3 = new AudioFileSourceID3(file);

    mp3 = new AudioGeneratorMP3();

    flac = new AudioGeneratorFLAC();

    aac = new AudioGeneratorAAC();

    xTaskCreate(playWavTask, "wav", 8192, NULL, 10, &playWAVHandler);
    xTaskCreate(playMP3Task, "mp3", 8192, NULL, 10, &playMP3Handler);
    xTaskCreate(playFLACTask, "flac", 8192, NULL, 10, &playFLACHandler);
    xTaskCreate(playACCTask, "aac", 8192, NULL, 10, &playACCHandler);
}

void loop()
{
    lv_task_handler();
    delay(5);
}


