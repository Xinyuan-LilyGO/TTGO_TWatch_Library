/*
 Font draw speed and flicker test, draws all numbers 0-999 in each font
 (0-99 in font 8)
 Average time in milliseconds to draw a character is shown in red
 A total of 2890 characters are drawn in each font (190 in font 8)

 Needs fonts 2, 4, 6, 7 and 8

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 Note that yield() or delay(0) must be called in long duration for/while
 loops to stop the ESP8266 watchdog triggering.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */

#include "config.h"

TTGOClass *ttgo;

unsigned long drawTime = 0;

void setup(void)
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->tft->setTextFont(2);
}

void loop()
{

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);

    drawTime = millis();

    for (int i = 0; i < 1000; i++) {
        ttgo->tft->drawNumber(i, 0, 0, 1);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
    if (drawTime < 100) ttgo->tft->drawString("Font 1 not loaded!", 0, 108, 2);

    delay(4000);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    drawTime = millis();

    for (int i = 0; i < 1000; i++) {
        ttgo->tft->drawNumber(i, 0, 0, 2);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
    if (drawTime < 200) ttgo->tft->drawString("Font 2 not loaded!", 0, 108, 2);

    delay(4000);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    drawTime = millis();

    for (int i = 0; i < 1000; i++) {
        ttgo->tft->drawNumber(i, 0, 0, 4);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
    if (drawTime < 200) ttgo->tft->drawString("Font 4 not loaded!", 0, 108, 2);

    delay(4000);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    drawTime = millis();

    for (int i = 0; i < 1000; i++) {
        yield(); ttgo->tft->drawNumber(i, 0, 0, 6);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
    if (drawTime < 200) ttgo->tft->drawString("Font 6 not loaded!", 0, 108, 2);

    delay(4000);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    drawTime = millis();

    for (int i = 0; i < 1000; i++) {
        yield(); ttgo->tft->drawNumber(i, 0, 0, 7);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 2890.0, 3, 0, 80, 4);
    if (drawTime < 200) ttgo->tft->drawString("Font 7 not loaded!", 0, 108, 2);

    delay(4000);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);
    drawTime = millis();

    for (int i = 0; i < 100; i++) {
        yield(); ttgo->tft->drawNumber(i, 0, 0, 8);
    }

    drawTime = millis() - drawTime;

    ttgo->tft->setTextColor(TFT_RED, TFT_BLACK);
    ttgo->tft->drawFloat(drawTime / 190.0, 3, 0, 80, 4);
    if (drawTime < 200) ttgo->tft->drawString("Font 8 not loaded!", 0, 108, 2);

    delay(4000);
}








