// Old time example, not recommended
#include "config.h"

TTGOClass *ttgo;
TFT_eSPI *tft ;
S7XG_Class *s7xg;
Ticker btnTicker;

uint32_t state = 0, prev_state = 0;
bool isInit = false;
lv_obj_t *btn2, *btn1, *btn3, *ta1, *gContainer;
bool sdOnline = false;
void createWin();
void stop_prev();


static void event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event != LV_EVENT_CLICKED) return;
    if (obj == btn1) {
        state = 1;
    } else if (obj == btn2) {
        state = 2;
    } else if (obj == btn3) {
        state = 3;
    }
    isInit = false;
    if (!ta1) {
        createWin();
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

    btn3 = lv_btn_create(gContainer, btn1);
    lv_obj_align(btn3, btn2, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
    lv_obj_set_event_cb(btn3, event_handler);

    label = lv_label_create(btn3, NULL);
    lv_label_set_text(label, "GPS");

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


void stop_prev()
{
    switch (prev_state) {
    case 1:
    case 2:
        Serial.println("Stop Lora");
        s7xg->loraPingPongStop();
        break;
    case 3:
        Serial.println("Stop GPS");
        s7xg->gpsStop();
        break;
    default:
        break;
    }
    prev_state = state;
}

void lora_sender()
{
    if (!isInit) {
        isInit = true;
        stop_prev();
        s7xg->loraSetPingPongSender();
        Serial.println("Start  lora_sender");
    }
    //Get data directly from the serial port
    String str = s7xg->loraGetPingPongMessage();
    if (str != "") {
        add_message(str.c_str());
    }
}

void lora_receiver()
{
    if (!isInit) {
        isInit = true;
        stop_prev();
        s7xg->loraSetPingPongReceiver();
        Serial.println("Start  lora_receiver");
    }
    //Get data directly from the serial port
    String str = s7xg->loraGetPingPongMessage();
    if (str != "") {
        add_message(str.c_str());
    }
}

void s7xg_gps()
{
    if (!isInit) {
        isInit = true;
        stop_prev();
        s7xg->gpsReset();
        s7xg->gpsSetLevelShift(true);
        s7xg->gpsSetStart();
        s7xg->gpsSetSystem(0);
        s7xg->gpsSetPositioningCycle(1000);
        s7xg->gpsSetPortUplink(20);
        s7xg->gpsSetFormatUplink(1);
        s7xg->gpsSetMode(1);
        Serial.println("Start  s7xg_gps");
    }
    static uint32_t timestamp = 0;
    // Send gps data command every 1 second interval
    if (millis() - timestamp > 1000) {
        timestamp = millis();
        ttgo->hwSerial->print("gps get_data dms");
    }
    //Get data directly from the serial port
    String str = s7xg->loraGetPingPongMessage();
    if (str != "") {
        add_message(str.c_str());
    }
}

void pressed()
{
    isInit = false;
    state = 0;
    stop_prev();
    createGui();
}

void setup(void)
{
    uint32_t cardSize = 0;
    Serial.begin(115200);

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->openBL();


    if (ttgo->sdcard_begin()) {
        Serial.println("SDCard begin SUCCESS!");
        cardSize = SD.cardSize() / (1024 * 1024);
        Serial.print("setupSDCard PASS . SIZE = ");
        Serial.print(cardSize);
        Serial.println(" MB");
        sdOnline = true;
    } else {
        Serial.println("SDCard begin FAILED!");
    }

    ttgo->lvgl_begin();

    //! Open s7xg chip power
    ttgo->enableLDO4();
    //! Open s7xg gps reset power
    ttgo->enableLDO3();


    ttgo->s7xg_begin();
    s7xg = ttgo->s7xg;

    ttgo->button->setPressedHandler(pressed);

    gContainer = lv_cont_create(lv_scr_act(), NULL);
    lv_obj_set_size(gContainer,  LV_HOR_RES, LV_VER_RES);
    // lv_obj_set_style(gContainer, &lv_style_transp_fit);

    lv_obj_t *label = lv_label_create(gContainer, NULL);
    lv_label_set_text(label, "Begin S7xG");
    lv_obj_align(label, NULL, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label1 = lv_label_create(gContainer, NULL);
    lv_label_set_text_fmt(label1, "SD Begin %s ", sdOnline ? "SUCCESS" : "FAILED");
    lv_obj_align(label1, label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    if (sdOnline) {
        label = lv_label_create(gContainer, NULL);
        lv_label_set_text_fmt(label, "SDCard:[%u] MB", cardSize);
        lv_obj_align(label, label1, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
    int len = 0;
    int retry = 0;
    do {
        lv_task_handler();
        len = s7xg->getHardWareModel().length();
        if (len == 0 && retry++ == 5) {
            s7xg->reset();
            retry = 0;
            Serial.println("Reset s7xg chip");
        }
        if (len == 0)
            delay(1000);
    } while (len == 0);

    Serial.println("Found s7xg module,Start gps module");

    createGui();

    btnTicker.attach_ms(30, []() {
        ttgo->button->loop();
    });
}


void loop(void)
{
    switch (state) {
    case 1:
        lora_sender();
        break;
    case 2:
        lora_receiver();
        break;
    case 3:
        s7xg_gps();
        break;
    default:
        break;
    }
    lv_task_handler();
    delay(5);
}




