/*
* Base on https://github.com/luisllamasbinaburo/ESP32_Faces
* Adapted to Twatch by Lewis 03/11/2020
*/
#pragma mark - Depend DabbleESP32 & Adafruit_NeoPixel library
/*
cd ~/Arduino/libraries
git clone https://github.com/Xinyuan-LilyGO/DabbleESP32.git
git clone https://github.com/adafruit/Adafruit_NeoPixel.git
*/
#include "Face.h"
#include <DabbleESP32.h>
#include "config.h"
#include <esp32PWMUtilities.h>
#include <Adafruit_NeoPixel.h>

#define WIDTH                   180
#define HEIGHT                  240
#define EYE                     40

#define LED_PIN                 4
#define BUZZER_PIN              2
#define SERVO_PIN               14

#define BRIGHTNESS              50
#define LED_COUNT               3

#define TMOTOR_MA_CHANNEL       20
#define TMOTOR_MB_CHANNEL       21
#define TMOTOR_MA5_CHANNEL      8

TTGOClass *watch;
TFT_eSPI *tft;
TFT_eSprite *Buffer;
Face *face;
TMotor Motor1;
TMotor Motor2;
bool start = false;

#ifndef TQUICK
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
#endif


void setup(void)
{
    char buf[128];

    srand(time(NULL));

    Serial.begin(115200);

    watch = TTGOClass::getWatch();

    watch->begin();

    watch->openBL();

    tft = watch->tft;

    tft->setRotation(1);

#ifdef TQUICK
    pinMode(TWATCH_TQUICK_EN, OUTPUT);
    digitalWrite(TWATCH_TQUICK_EN, HIGH);
    snprintf(buf, 128, "T-Quick-%x:%x", rand() % 0xFF, rand() % 0xFF);
#else
    ledcSetup(TMOTOR_MA5_CHANNEL, 1000, 16);
    ledcAttachPin(BUZZER_PIN, TMOTOR_MA5_CHANNEL);

    strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
    strip.setBrightness(255);
    strip.clear();

    strip.setPixelColor(0, strip.Color(50, 0, 0));
    strip.setPixelColor(1, strip.Color(50, 0, 0));
    strip.setPixelColor(2, strip.Color(50, 0, 0));
    strip.setPixelColor(3, strip.Color(50, 0, 0));
    strip.show();
    snprintf(buf, 128, "T-Bao-%x:%x", rand() % 0xFF, rand() % 0xFF);

#endif
    Motor1.attach(TMOTOR_MA_CHANNEL, TWATCH_TQUICK_MA1, TWATCH_TQUICK_MA2);
    Motor2.attach(TMOTOR_MB_CHANNEL, TWATCH_TQUICK_MB1, TWATCH_TQUICK_MB2);

    Dabble.begin(buf);

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
    int pos = 0;
    static uint32_t counter = millis();
    static uint8_t emotionno = 0;
    static uint8_t emotionflag = 1;
    static uint8_t n = 0;
    Dabble.processInput();

    if (GamePad.isUpPressed()) {
        Motor1.moveMotor(-2.55 * 100);
        Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isDownPressed()) {
        Motor1.moveMotor(2.55 * 100);
        Motor2.moveMotor(-2.55 * 100);
        start = true;
    } else if (GamePad.isLeftPressed()) {
        Motor1.freeMotor();
        Motor2.moveMotor(2.55 * 100);
        start = true;
    } else if (GamePad.isRightPressed()) {
        Motor1.moveMotor(-2.55 * 100);
        Motor2.freeMotor();
        start = true;
    } else {
        if (start) {
            start = false;
            Motor1.freeMotor();
            Motor2.freeMotor();
        }
    }
#ifndef TQUICK
    if (GamePad.isStartPressed()) {
        Serial.println("isStartPressed");
        strip.clear();
        strip.show();   // Send the updated pixel colors to the hardware.
    } else if (GamePad.isSelectPressed()) {
        uint16_t color = rand() % 0xFFFF;
        for (int i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, color);
            strip.show();   // Send the updated pixel colors to the hardware.
        }
        delay(500);
    }

    if (GamePad.isTrianglePressed()) {
        static Ticker t;
        ledcWrite(TMOTOR_MA5_CHANNEL, 1000);
        t.once_ms(150, []() {
            ledcWrite(TMOTOR_MA5_CHANNEL,  0 );
        });
    } else if (GamePad.isCrossPressed()) {
        Serial.println("isCrossPressed");
    }

    if (GamePad.isSquarePressed()) {
        if (Controls.angleServo1 <= 0) {
            return;
        }
        for (pos = 45; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
            Controls.angleServo1 = pos;
            Controls.runServo1(SERVO_PIN);
            delay(15);                       // waits 15ms for the servo to reach the position
        }
    } else if (GamePad.isCirclePressed()) {
        if (Controls.angleServo1 >= 45) {
            return;
        }
        for (pos = 0; pos <= 45; pos += 1) { // goes from 0 degrees to 180 degrees
            Controls.angleServo1 = pos;
            Controls.runServo1(SERVO_PIN);
            delay(15);                       // waits 15ms for the servo to reach the position
        }
    }
#endif

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
    }
    face->Update();
}
