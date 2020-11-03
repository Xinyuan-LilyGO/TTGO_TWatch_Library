/*
* Base on https://github.com/luisllamasbinaburo/ESP32_Faces
* Adapted to Twatch by Lewis 03/11/2020
*/
#include "Face.h"
#include "config.h"

#define WIDTH  180
#define HEIGHT 240
#define EYE 40

TTGOClass *watch;
TFT_eSPI *tft;
TFT_eSprite *Buffer;
Face *face;

String emotionName[] = {
    "Normal",
    "Angry",
    "Glee",
    "Happy",
    "Sad",
    "Worried",
    "Focused",
    "Annoyed",
    "Surprised",
    "Skeptic",
    "Fustrated",
    "Unimpresse",
    "Sleepy",
    "Suspicious",
    "Squint",
    "Furious",
    "Scared",
    "Awe"
};


void setup(void)
{
    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->openBL();

    tft = watch->tft;

    Buffer = new TFT_eSprite(tft);

    face = new Face(*Buffer, HEIGHT, WIDTH, EYE);

    Buffer->setColorDepth(16);
    Buffer->createSprite(WIDTH, HEIGHT);
    face->Expression.GoTo_Normal();
    face->Behavior.Clear();
    face->Background = TFT_BLACK;
    tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    tft->setTextSize(2);
}


void loop()
{
    static uint32_t counter = millis();
    static uint8_t emotionno = 0;
    static uint8_t emotionflag = 1;
    static uint8_t n = 0;

    if (millis() - counter > 3000) {
        counter = millis();
        emotionno = n++;
        if (n >= EMOTIONS_COUNT)n = 0;
        emotionflag = 1;
    }
    if (emotionflag) {
        emotionflag = 0;
        face->Behavior.Clear();
        face->Behavior.SetEmotion((eEmotions)emotionno, 1.0);
        tft->fillRect(0, 180, 240, 28, TFT_BLACK);
        tft->drawString(emotionName[emotionno], (240 - (emotionName[emotionno].length() * 12)) / 2, 180);
    }
    face->Update();
    delay(10);
}
