#pragma once

/*****************FFT CONFIG*******************/
#define SAMPLES                  512
#define SAMPLE_BLOCK             64
#define SAMPLE_FREQ              16000

/*******************global event group**********************/
#define LV_BUTTON                _BV(0)
#define LV_ENCODER_CW            _BV(1)
#define LV_ENCODER_CCW           _BV(2)

#define LV_ENCODER_LED_CW        _BV(3)
#define LV_ENCODER_LED_CCW       _BV(4)

#define LV_SELF_TEST_START       _BV(5)
#define LV_UI_DEMO_START         _BV(6)

#define RING_PAUSE               _BV(0)
#define RING_STOP                _BV(1)
#define WAV_RING_1               _BV(2) // Beep
#define FFT_READY                _BV(3)
#define FFT_STOP                 _BV(4)
/*******************app msg**********************/

#define MSG_MENU_NAME_CHANGED    100

#define MSG_LABEL_PARAM_CHANGE_1 200
#define MSG_LABEL_PARAM_CHANGE_2 201

#define MSG_TITLE_NAME_CHANGE    203

#define MSG_BLE_SEND_DATA_1      204
#define MSG_BLE_SEND_DATA_2      205

#define MSG_MUSIC_TIME_ID        300
#define MSG_MUSIC_TIME_END_ID    301

#define MSG_FFT_ID               400