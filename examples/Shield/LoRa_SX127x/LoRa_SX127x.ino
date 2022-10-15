#pragma mark - Depend arduino-LoRa library
/*
cd ~/Arduino/libraries
git clone https://github.com/sandeepmistry/arduino-LoRa.git
*/

#include "config.h"
#include <LoRa.h>


TTGOClass *ttgo;

uint32_t state = 0, prev_state = 0;
bool isInit = false;
lv_obj_t *btn2, *btn1, *ta1, *gContainer;
SPIClass LoraSPI(HSPI);
uint32_t sendCount = 0;
String recv = "";
char buf[256];


void createWin();
void add_message(const char *txt);

#define LORA_PERIOD 923

#if LORA_PERIOD == 433
#define BAND 433E6
#elif LORA_PERIOD == 868
#define BAND 868E6
#elif LORA_PERIOD == 915
#define BAND 915E6
#elif LORA_PERIOD == 923
#define BAND 923E6
#else
#define BAND 470E6
#endif


static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event != LV_EVENT_CLICKED) return;
    isInit = false;
    if (!ta1) {
        createWin();
    }
    if (obj == btn1) {
        state = 1;
        add_message("Lora Sender...");
    } else if (obj == btn2) {
        state = 2;
        add_message("Lora Received...");
    }

}

static void  createGui()
{
    lv_obj_clean(gContainer);

    ta1 = NULL;
    lv_obj_t *label ;
    /*Create a normal button*/
    btn1 = lv_btn_create(gContainer, NULL);
    lv_obj_set_size(btn1, 120, 65);
    lv_obj_align(btn1, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
    lv_obj_set_event_cb(btn1, event_handler);

    /*Add a label to the button*/
    label = lv_label_create(btn1, NULL);
    lv_label_set_text(label, "Sender");

    btn2 = lv_btn_create(gContainer, btn1);
    lv_obj_align(btn2, btn1, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_event_cb(btn2, event_handler);

    label = lv_label_create(btn2, NULL);
    lv_label_set_text(label, "Receiver");

}

void createWin()
{
    lv_obj_clean(gContainer);
    ta1 = lv_textarea_create(gContainer, NULL);
    lv_obj_set_size(ta1, LV_HOR_RES, LV_VER_RES);
    lv_obj_align(ta1, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_textarea_set_text(ta1, "");    /*Set an initial text*/
    lv_textarea_set_max_length(ta1, 128);
}

void add_message(const char *txt)
{
    if (!txt || !ta1)return;
    if (strlen(lv_textarea_get_text(ta1)) >= lv_textarea_get_max_length(ta1)) {
        lv_textarea_set_text(ta1, "");
    }
    String str = txt;
    str.trim();
    str += "\n";
    lv_textarea_add_text(ta1, str.c_str());
}


void pressed()
{
    isInit = false;
    state = 0;
    sendCount = 0;
    createGui();
}

void setup(void)
{
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->openBL();

    ttgo->lvgl_begin();

    //! Open lora module power
    ttgo->enableLDO3();

    ttgo->button->setPressedHandler(pressed);

    gContainer = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(gContainer,  LV_HOR_RES, LV_VER_RES);

    lv_obj_t *label = lv_label_create(gContainer, NULL);
    lv_label_set_text(label, "Begin Lora Module");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);


    LoraSPI.begin(TWATCH_LORA_SCK, TWATCH_LORA_MISO, TWATCH_LORA_MOSI, TWATCH_LORA_SS);
    LoRa.setSPI(LoraSPI);
    LoRa.setPins(TWATCH_LORA_SS, TWATCH_LORA_RST, TWATCH_LORA_DI0);

    int ret = 0;
    do {
        lv_task_handler();
        ret = LoRa.begin(BAND);
        delay(500);
    } while (!ret);
    Serial.print("LORA setup pass:");
    Serial.print(BAND);
    Serial.println("MHZ");

    createGui();

    lv_task_create([](lv_task_t *args) {
        ttgo->button->loop();
    }, 30, 1, nullptr);

}

uint32_t startmillis = 0;

void loop(void)
{
    switch (state) {
    case 1:
        if (millis() - startmillis > 1000 ) {
            startmillis = millis();
            LoRa.beginPacket();
            LoRa.print("lora: ");
            LoRa.print(sendCount);
            LoRa.endPacket();
            ++sendCount;
            snprintf(buf, sizeof(buf), "Send %lu\n", sendCount);
            Serial.println(buf);
            add_message(buf);
        }
        break;
    case 2:
        if (LoRa.parsePacket()) {
            recv = "";
            while (LoRa.available()) {
                recv += (char)LoRa.read();
            }
            snprintf(buf, sizeof(buf), "Received:%s - rssi:%d\n", recv.c_str(), LoRa.packetRssi());
            Serial.println(buf);
            add_message(buf);
        }
        break;
    default:
        break;
    }
    lv_task_handler();
    delay(5);
}




