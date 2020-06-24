

#include "config.h"
#include "fingerprint.h"

#define MODULE_TX       33
#define MODULE_RX       34
#define UART_BAUD       57600
#define VTOUCH_PIN      26
#define BUZZER_PIN      25
#define BUZZER_CHANNEL  2

#define ModuleSerial    Serial1

TTGOClass *watch =  nullptr;
TFT_eSPI *tft = nullptr;

Fingerprint eic(&Serial1);
SysParams_t params;
bool flag   = false;

char buff[512];

void touchPadAttach(void)
{
    Serial.println("TouchPad Attach");
    flag = true;
}

void playSound()
{
    ledcWriteTone(BUZZER_CHANNEL, 1000);
    delay(200);
    ledcWriteTone(BUZZER_CHANNEL, 0);
}

void setup()
{
    Serial.begin(115200);

    delay(500);

    watch = TTGOClass::getWatch();

    watch->begin();

    tft = watch->tft;

    tft->setTextFont(2);

    tft->fillScreen(TFT_BLACK);

    tft->setCursor(0, 0);

    watch->sdcard_begin();

    if (watch->sdcard_begin()) {
        tft->println("SD Begin OK");
    } else {
        tft->println("SD Begin Failed");
    }

    tft->println("Begin Fingerprint module ...");

    // enable module power
    watch->enableLDO3();

    // open blacklight
    watch->openBL();

    ModuleSerial.begin(UART_BAUD, SERIAL_8N1, MODULE_RX, MODULE_TX);

    // buzzer init
    ledcSetup(BUZZER_CHANNEL, 1000, 8);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);

    // module interrupt pin init
    pinMode(VTOUCH_PIN, INPUT_PULLDOWN);
    attachInterrupt(VTOUCH_PIN, touchPadAttach, CHANGE);

    // Wait for module ack
    do {
        eic.readSysParams(&params);

        Serial.printf("address: 0x%x --packSize : %u  -- %d bps  -- level:%d -- statusReg:0x%x  -- code:0x%x\n",
                      params.address,
                      params.packSize,
                      9600 * params.bps,
                      params.level,
                      params.statusReg,
                      params.code
                     );
        delay(1000);
    } while (params.address != 0xFFFFFFFF);

    tft->setTextColor(TFT_GREEN, TFT_BLACK);

    tft->print("Address:"); tft->println(params.address, HEX);
    tft->print("Pack Size:"); tft->println(params.packSize);
    tft->print("Speed:"); tft->println(9600 * params.bps);
    tft->print("Level:"); tft->println(params.level);
    tft->print("StatusReg:"); tft->println(params.statusReg, HEX);
    tft->print("Code:"); tft->println(params.code);

    playSound();
    playSound();
    playSound();
    playSound();

}


bool loop1(GR_BufID_t id)
{
    // while (!flag) {
    //     delay(500);
    // }
    flag = false;
    if (eic.entryFinger()) {
        Serial.println("entery PASS");
        delay(200);
        if (eic.generateFeature(id)) {
            Serial.println("generater PASS");
            return true;
        } else {
            Serial.println("generater FAIL");
        }
    }
    return false;
}

void loop()
{
    while (!loop1(GR_CHAR_BUFFER_1));
    while (!loop1(GR_CHAR_BUFFER_2));
    if (eic.mergeFeature()) {
        Serial.println("merge PASS");
        if (eic.storeFeature(GR_CHAR_BUFFER_1, 0)) {
            Serial.println("storeFeature PASS");
        } else {
            Serial.println("storeFeature FAIL");
        }
    } else {
        Serial.println("merge FAIL");
    }
    while (1) {
        if (loop1(GR_CHAR_BUFFER_1)) {
            if (eic.searchFingerprint(GR_CHAR_BUFFER_1, 0, 10)) {
                Serial.println("Fingerprint valid PASS");
                playSound();

            } else {
                // Serial.println("Fingerprint valid FAIL");
            }
        }
    }
}