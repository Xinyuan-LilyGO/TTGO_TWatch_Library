
#include <TTGO.h>

TTGOClass *ttgo;

void pressed(const char * str)
{
    uint16_t color = random(0xFFFF);
    ttgo->eTFT->fillScreen(color);
    ttgo->eTFT->setTextColor(color, TFT_WHITE);
    ttgo->eTFT->drawString(str,  80, 100, 4);
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->gameControlBegin();
    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);
    ttgo->eTFT->setTextFont(4);
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
