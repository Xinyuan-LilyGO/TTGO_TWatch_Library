#pragma mark - Depend Rotary library
/*
cd ~/Arduino/libraries
git clone https://github.com/brianlow/Rotary
*/

#include "config.h"
#include <Rotary.h>


// Rotary Encoder Inputs
#define PORTA       26
#define PORTB       33
#define BUTTON      34

Rotary rot = Rotary(PORTA, PORTB);

Button2 btn = Button2(BUTTON);
TTGOClass *watch = nullptr;
TFT_eSPI *tft;
uint8_t count = 0;
uint8_t prev;

void pressedHander(void)
{
    tft->drawCentreString("OK is Pressed ",
                          tft->width() / 2,
                          tft->height() / 2 - 20,
                          2);
}

void releasedHandler(void)
{
    tft->drawCentreString("OK is Released",
                          tft->width() / 2,
                          tft->height() / 2 - 20,
                          2);
}

void setup()
{
    Serial.begin(115200);


    watch = TTGOClass::getWatch();

    watch->begin();

    watch->openBL();

    // Receive as a local variable for easy writing
    tft = watch->tft;

    btn.setPressedHandler(pressedHander);
    btn.setReleasedHandler(releasedHandler);

    rot.begin(true);


    tft->setTextColor(TFT_GREEN, TFT_BLACK);

    tft->drawCentreString("Rotary test",
                          tft->width() / 2,
                          tft->height() / 2 - 40,
                          2);
}


void loop()
{
    btn.loop();
    unsigned char result = rot.process();
    if (result) {
        Serial.println(result == DIR_CW ? "Right" : "Left");
        tft->drawCentreString(result == DIR_CW ? "Right" : "Left ",
                              tft->width() / 2,
                              tft->height() / 2,
                              2);
    }
}