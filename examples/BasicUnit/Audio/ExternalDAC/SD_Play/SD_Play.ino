#pragma mark - Depend ESP8266Audio and ESP8266_Spiram libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/earlephilhower/ESP8266Audio
git clone https://github.com/Gianbacchio/ESP8266_Spiram
*/

#include "config.h"
#include <WiFi.h>
#include <HTTPClient.h>         //Remove Audio Lib error
#include "AudioFileSourceSD.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include <Esp.h>

Ticker tk;
TTGOClass *ttgo;
AudioGeneratorMP3 *mp3;
AudioFileSourceSD *file;
AudioOutputI2S *out;
AudioFileSourceID3 *id3;

static lv_obj_t *list_cont;
static lv_obj_t *play_cont;
static lv_obj_t *curr_list;
static lv_obj_t *label;
static lv_obj_t *scr;
static lv_obj_t *list;
static lv_obj_t *img_btn[3];
static lv_obj_t *button[3];
static const void *src_img_btn[4] =  {LV_SYMBOL_PREV, LV_SYMBOL_PAUSE, LV_SYMBOL_NEXT, LV_SYMBOL_PLAY};
static void list_event_handler(lv_obj_t *obj, lv_event_t event);
static char  filename[128];
uint8_t state = 255;
LV_IMG_DECLARE(bg1);

enum {
    PLAY,
    PAUSE,
    NEXT,
    PREV,
};

void create_gui();

void pressed()
{
    ESP.restart();
}

static void list_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        curr_list = obj;
        const char *txt = lv_list_get_btn_text(obj);
        Serial.printf("Clicked %s\n", txt);
        state = PLAY;
        strcpy(filename, txt);
        create_gui();
        file->open(txt);
        mp3->begin(id3, out);
    }
}

static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED) {
        if (button[0] == obj) {
            if (state != PLAY)return;
            lv_obj_t *prev_btn = lv_list_get_prev_btn(list, curr_list);
            if (prev_btn == NULL) {
                Serial.println("prev is null");
                return;
            }
            curr_list = prev_btn;
            const char *txt = lv_list_get_btn_text(prev_btn);
            lv_label_set_text(label, txt);
            out->stop();
            if (mp3->isRunning())
                mp3->stop();
            file->open(txt);
            mp3->begin(id3, out);


        } else if (button[1] == obj) {
            if (state == PAUSE) {
                state = PLAY;
                lv_img_set_src(img_btn[1], LV_SYMBOL_PAUSE);
                out->begin();
            } else {
                state = PAUSE;
                out->stop();
                lv_img_set_src(img_btn[1], LV_SYMBOL_PLAY);
            }
        } else if (button[2] == obj) {
            if (state != PLAY)return;
            lv_obj_t *next_btn = lv_list_get_next_btn(list, curr_list);
            if (next_btn == NULL) {
                Serial.println("next is null");
                return;
            }
            curr_list = next_btn;
            const char *txt = lv_list_get_btn_text(next_btn);
            lv_label_set_text(label, txt);
            out->stop();
            if (mp3->isRunning())
                mp3->stop();
            file->open(txt);
            mp3->begin(id3, out);
        }
    }
}

void create_gui()
{
    lv_obj_set_hidden(list_cont, true);

    play_cont = lv_cont_create(scr, NULL);
    lv_obj_set_size(play_cont,  240, 240);

    lv_obj_t *img = lv_img_create(play_cont, NULL);
    lv_img_set_src(img, &bg1);
    lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);

    label = lv_label_create(play_cont, NULL);
    lv_label_set_text(label, filename);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SROLL);
    lv_obj_set_width(label, 80);
    lv_obj_align(label, play_cont, LV_ALIGN_IN_TOP_MID, 0, 50);

    for (uint8_t i = 0; i < 3; i++) {
        button[i] = lv_btn_create(play_cont, NULL);
        lv_obj_set_size(button[i], 50, 50);
        img_btn[i] = lv_img_create(button[i], NULL);
        lv_img_set_src(img_btn[i], src_img_btn[i]);
        lv_obj_set_event_cb(button[i], event_handler);
    }

    lv_obj_align(button[0], NULL, LV_ALIGN_IN_LEFT_MID, 40, 40);

    for (uint8_t i = 1; i < 3; i++) {
        lv_obj_align(button[i], button[i - 1], LV_ALIGN_OUT_RIGHT_MID, 10, 0);
    }

}

int searchMusic(fs::FS &fs, const char *dirname, uint8_t levels)
{
    int searchFiles = 0;
    File root = fs.open(dirname);
    if (!root) {
        Serial.println("- failed to open directory");
        return 0;
    }
    if (!root.isDirectory()) {
        Serial.println(" - not a directory");
        return 0;
    }
    File file = root.openNextFile();
    while (file) {
        if (!file.isDirectory()) {
            String name = String(file.name());
            if (name.endsWith(".mp3") || name.endsWith(".MP3")) {
                if (searchFiles++ > 50)return searchFiles;
                lv_obj_t *list_btn =  lv_list_add_btn(list, LV_SYMBOL_FILE, file.name());
                lv_obj_set_event_cb(list_btn, list_event_handler);
            }
        }
        file = root.openNextFile();
    }
    return searchFiles;
}



void setup()
{
    WiFi.mode(WIFI_OFF);
    delay(500);

    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextFont(2);

    ttgo->tft->setCursor(0, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->println("Sample MP3 playback begins");

    ttgo->button->setPressedHandler(pressed);

    tk.attach_ms(30, []() {
        ttgo->button->loop();
    });

    //!Turn on the audio power
    ttgo->enableLDO3();

    file = new AudioFileSourceSD();
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S();
    //! External DAC decoding
    out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
    mp3 = new AudioGeneratorMP3();

    while (1) {
        if (ttgo->sdcard_begin()) {
            Serial.println("sd begin pass");
            break;
        }
        Serial.println("sd begin fail,wait 1 sec");
        delay(1000);
    }

    ttgo->lvgl_begin();

    scr = lv_scr_act();

    list_cont = lv_cont_create(scr, NULL);

    lv_obj_set_size(list_cont,  240, 240);

    list = lv_list_create(list_cont, NULL);

    lv_obj_set_size(list,  240, 240);

    lv_obj_align(list, list_cont, LV_ALIGN_CENTER, 0, 0);

    if (searchMusic(SD, "/", 2) == 0) {
        Serial.println("No Search mp3 file");
        while (1) {
            delay(1000);
        }
    }

}

void loop()
{
    switch (state) {
    case PLAY:
        if (mp3->isRunning()) {
            if (!mp3->loop()) {
                mp3->stop();
                state = 255;
            }
        }
        break;
    case PAUSE:
        break;
    case NEXT:
        break;
    case PREV:
        break;
    default:
        delay(5);
        break;
    }
    lv_task_handler();
}

