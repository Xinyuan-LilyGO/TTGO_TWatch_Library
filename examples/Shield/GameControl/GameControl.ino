#include "config.h"

TTGOClass *ttgo;

void pressed(const char *str)
{
    uint16_t color = random(0xFFFF);
    ttgo->tft->fillScreen(color);
    ttgo->tft->setTextColor(color, TFT_WHITE);
    ttgo->tft->drawString(str,  80, 100, 4);
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->gameControlBegin();
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->tft->setTextFont(4);
    ttgo->tft->drawString("Gamepad Start",  0, 0, 4);
}

void loop()
{
    if (ttgo->gameAisPressed()) {
        ttgo->gameControlBuzzer();
        pressed("A Pressed");
    }
    if (ttgo->gameBisPressed()) {
        ttgo->gameControlBuzzer();
        pressed("B Pressed");
    }
    if (ttgo->gameXisPressed()) {
        ttgo->gameControlBuzzer();
        pressed("X Pressed");
    }
    if (ttgo->gameYisPressed()) {
        ttgo->gameControlBuzzer();
        pressed("Y Pressed");
    }
    if (ttgo->gameCisPressed()) {
        ttgo->gameControlBuzzer();
        pressed("C Pressed");
    }
    ttgo->gameControlHandler();
}
