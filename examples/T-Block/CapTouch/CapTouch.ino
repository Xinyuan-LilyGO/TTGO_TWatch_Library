#include "config.h"
#include <MPR121.h>

#define MPR121_SDA_PIN      19
#define MPR121_SCL_PIN      18
#define MPR121_IRQ_PIN      34
#define MPR121_BL_PIN       12
#define MPR121_BUZZER_PIN   23

TTGOClass *ttgo = nullptr;
MPR121_Class cap;

uint16_t lasttouched = 0;
uint16_t currtouched = 0;
bool enterComplete = false;
String tmpPassword = "";

#define KEYBOARD_PASSWORD   "12345678"
const char *keyboard[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "*", "0", "#"};

void playSound(int s)
{
    for (int i = 0; i < s; i++) {
        ledcWriteTone(10, 1000);
        delay(200);
        ledcWrite(10, 0);
    }
}

void setup()
{
    Serial.begin(115200);
    ttgo = TTGOClass::getWatch();
    ttgo->begin();

    Wire1.begin(MPR121_SDA_PIN, MPR121_SCL_PIN);
    ttgo->openBL();
    delay(2000);
    ttgo->closeBL();

    if (!cap.begin()) {
        Serial.println("MPR121 not found, check wiring?");
        while (1);
    }
    Serial.println("MPR121 found!");

    ledcAttachPin(MPR121_BUZZER_PIN, 10);
    ledcSetup(10, 12000, 8);
    ledcWrite(10, 0);
    playSound(2);
}

void loop()
{
    ttgo->button->loop();
    // Get the currently touched pads
    currtouched = cap.touched();
    for (uint8_t i = 0; i < 12; i++) {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {

            playSound(1);
            const char *keyVal = keyboard[i];
            if (String(keyVal).equals("*")) {
                Serial.println("***");
            } else if (String(keyVal).equals("#")) {
                Serial.println("####");
                enterComplete = true;
            } else {
                Serial.println(keyVal);
                tmpPassword += keyVal;
            }
        }
        if (enterComplete) {
            enterComplete = false;
            if (tmpPassword == KEYBOARD_PASSWORD) {
                Serial.println("Success");
                playSound(4);
            } else {
                Serial.println("Failed");
                // playSound(4);
                for (int i = 0; i < 4; i++) {
                    ttgo->closeBL();
                    playSound(1);
                    delay(200);
                    ttgo->openBL();
                    delay(200);
                    playSound(1);
                }
            }
            tmpPassword = "";
        }
    }
    // reset our state
    lasttouched = currtouched;
}