// Demo based on:
// UTFT_Demo_320x240 by Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
/*

  This sketch uses the GLCD and font 2 only.

  Make sure all the display driver and pin comnenctions are correct by
  editting the User_Setup.h file in the TFT_eSPI library folder.

  Note that yield() or delay(0) must be called in long duration for/while
  loops to stop the ESP8266 watchdog triggering.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/
#include <TTGO.h>

TTGOClass *ttgo;
#define DELAY 500

#define TFT_GREY 0x7BEF
#define TFT_W 240
#define TFT_H 240

unsigned long runTime = 0;
void setup()
{
    // Setup the LCD
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
}

void loop()
{
    //randomSeed(millis());
    randomSeed(1234); // This ensure test is repeatable with exact same draws each loop
    int buf[TFT_W - 2];
    int x, x2;
    int y, y2;
    int r;
    runTime = millis();
    // Clear the screen and draw the frame
    ttgo->eTFT->fillScreen(TFT_BLACK);


    ttgo->eTFT->fillRect(0, 0, TFT_W - 1, 14, TFT_RED);

    ttgo->eTFT->fillRect(0, TFT_H - 14, TFT_W - 1, 14, TFT_GREY);

    ttgo->eTFT->setTextColor(TFT_BLACK, TFT_RED);
    ttgo->eTFT->drawCentreString("* TFT_S6D02A1 *", TFT_W / 2, 4, 1);
    ttgo->eTFT->setTextColor(TFT_YELLOW, TFT_GREY);
    ttgo->eTFT->drawCentreString("Adapted by Bodmer", TFT_W / 2, TFT_H - 12, 1);

    ttgo->eTFT->drawRect(0, 14, TFT_W - 1, TFT_H - 28, TFT_BLUE);

    // Draw crosshairs
    ttgo->eTFT->drawLine(TFT_W / 2 - 1, 15, TFT_W / 2 - 1, TFT_H - 16, TFT_BLUE);
    ttgo->eTFT->drawLine(1, TFT_H / 2 - 1, TFT_W - 2, TFT_H / 2 - 1, TFT_BLUE);
    for (int i = 9; i < TFT_W - 1; i += 10)
        ttgo->eTFT->drawLine(i, TFT_H / 2 - 3, i, TFT_H / 2 + 1, TFT_BLUE);
    for (int i = 19; i < TFT_H - 20; i += 10)
        ttgo->eTFT->drawLine(TFT_W / 2 - 3, i, TFT_W / 2 + 1, i, TFT_BLUE);

    // Draw sin-, cos- and tan-lines
    ttgo->eTFT->setTextColor(TFT_CYAN);
    ttgo->eTFT->drawString("Sin", 5, 15, 2);
    for (int i = 1; i < TFT_W - 2; i++) {
        ttgo->eTFT->drawPixel(i, TFT_H / 2 - 1 + (sin(((i * 2.26) * 3.14) / 180) * 48), TFT_CYAN);
    }
    ttgo->eTFT->setTextColor(TFT_RED);
    ttgo->eTFT->drawString("Cos", 5, 30, 2);
    for (int i = 1; i < TFT_W - 2; i++) {
        ttgo->eTFT->drawPixel(i, TFT_H / 2 - 1 + (cos(((i * 2.26) * 3.14) / 180) * 48), TFT_RED);
    }
    ttgo->eTFT->setTextColor(TFT_YELLOW);
    ttgo->eTFT->drawString("Tan", 5, 45, 2);
    for (int i = 1; i < TFT_W - 2; i++) {
        ttgo->eTFT->drawPixel(i, TFT_H / 2 - 1 + (tan(((i * 2.26) * 3.14) / 180)), TFT_YELLOW);
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    ttgo->eTFT->drawLine(TFT_W / 2 - 1, 15, TFT_W / 2 - 1, TFT_H - 16, TFT_BLUE);
    ttgo->eTFT->drawLine(1, TFT_H / 2 - 1, TFT_W - 2, TFT_H / 2 - 1, TFT_BLUE);
    int col = 0;
    // Draw a moving sinewave
    x = 1;
    for (int i = 1; i < ((TFT_W - 3) * 20); i++) {
        x++;
        if (x == TFT_W - 2)
            x = 1;
        if (i > TFT_W - 2) {
            if ((x == TFT_W / 2 - 1) || (buf[x - 1] == TFT_H / 2 - 1))
                col = TFT_BLUE;
            else
                ttgo->eTFT->drawPixel(x, buf[x - 1], TFT_BLACK);
        }
        y = TFT_H / 2 + (sin(((i * 2.2) * 3.14) / 180) * (49 - (i / 100)));
        ttgo->eTFT->drawPixel(x, y, TFT_BLUE);
        buf[x - 1] = y;
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some filled rectangles
    for (int i = 1; i < 6; i++) {
        switch (i) {
        case 1:
            col = TFT_MAGENTA;
            break;
        case 2:
            col = TFT_RED;
            break;
        case 3:
            col = TFT_GREEN;
            break;
        case 4:
            col = TFT_BLUE;
            break;
        case 5:
            col = TFT_YELLOW;
            break;
        }
        ttgo->eTFT->fillRect(30 + (i * 10), 20 + (i * 10), 30, 30, col);
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some filled, rounded rectangles
    for (int i = 1; i < 6; i++) {
        switch (i) {
        case 1:
            col = TFT_MAGENTA;
            break;
        case 2:
            col = TFT_RED;
            break;
        case 3:
            col = TFT_GREEN;
            break;
        case 4:
            col = TFT_BLUE;
            break;
        case 5:
            col = TFT_YELLOW;
            break;
        }
        ttgo->eTFT->fillRoundRect(TFT_W / 2 + 20 - (i * 10), 20 + (i * 10), 30, 30, 3, col);
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some filled circles
    for (int i = 1; i < 6; i++) {
        switch (i) {
        case 1:
            col = TFT_MAGENTA;
            break;
        case 2:
            col = TFT_RED;
            break;
        case 3:
            col = TFT_GREEN;
            break;
        case 4:
            col = TFT_BLUE;
            break;
        case 5:
            col = TFT_YELLOW;
            break;
        }
        ttgo->eTFT->fillCircle(45 + (i * 10), 30 + (i * 10), 15, col);
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some lines in a pattern

    for (int i = 15; i < TFT_H - 16; i += 5) {
        ttgo->eTFT->drawLine(1, i, (i * 1.44) - 10, TFT_H - 17, TFT_RED);
    }

    for (int i = TFT_H - 17; i > 15; i -= 5) {
        ttgo->eTFT->drawLine(TFT_W - 3, i, (i * 1.44) - 11, 15, TFT_RED);
    }

    for (int i = TFT_H - 17; i > 15; i -= 5) {
        ttgo->eTFT->drawLine(1, i, TFT_W + 11 - (i * 1.44), 15, TFT_CYAN);
    }

    for (int i = 15; i < TFT_H - 16; i += 5) {
        ttgo->eTFT->drawLine(TFT_W - 3, i, TFT_W + 10 - (i * 1.44), TFT_H - 17, TFT_CYAN);
    }

    delay(DELAY);


    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some random circles
    for (int i = 0; i < 100; i++) {
        x = 32 + random(TFT_W - 2 - 32 - 30);
        y = 45 + random(TFT_H - 19 - 45 - 30);
        r = random(30);
        ttgo->eTFT->drawCircle(x, y, r, random(0xFFFF));
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some random rectangles
    for (int i = 0; i < 100; i++) {
        x = 2 + random(TFT_W - 4);
        y = 16 + random(TFT_H - 33);
        x2 = 2 + random(TFT_H - 4);
        y2 = 16 + random(TFT_H - 33);
        if (x2 < x) {
            r = x; x = x2; x2 = r;
        }
        if (y2 < y) {
            r = y; y = y2; y2 = r;
        }
        ttgo->eTFT->drawRect(x, y, x2 - x, y2 - y, random(0xFFFF));
    }

    delay(DELAY);


    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    // Draw some random rounded rectangles
    for (int i = 0; i < 100; i++) {
        x = 2 + random(TFT_W - 4);
        y = 16 + random(TFT_H - 33);
        x2 = 2 + random(TFT_W - 4);
        y2 = 16 + random(TFT_H - 33);
        // We need to get the width and height and do some window checking
        if (x2 < x) {
            r = x; x = x2; x2 = r;
        }
        if (y2 < y) {
            r = y; y = y2; y2 = r;
        }
        // We need a minimum size of 6
        if ((x2 - x) < 6) x2 = x + 6;
        if ((y2 - y) < 6) y2 = y + 6;
        ttgo->eTFT->drawRoundRect(x, y, x2 - x, y2 - y, 3, random(0xFFFF));
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

    //randomSeed(1234);
    int colour = 0;
    for (int i = 0; i < 100; i++) {
        x = 2 + random(TFT_W - 4);
        y = 16 + random(TFT_H - 31);
        x2 = 2 + random(TFT_W - 4);
        y2 = 16 + random(TFT_H - 31);
        colour = random(0xFFFF);
        ttgo->eTFT->drawLine(x, y, x2, y2, colour);
    }

    delay(DELAY);

    ttgo->eTFT->fillRect(1, 15, TFT_W - 3, TFT_H - 31, TFT_BLACK);

#define RANDOM

#ifdef RANDOM
    // Draw 10,000 pixels
    // It takes 30ms to calculate the 30,000 random numbers so this is not a true drawPixel speed test
    for (int i = 0; i < 10000; i++) {
        ttgo->eTFT->drawPixel(2 + random(316), 16 + random(209), random(0xFFFF));
    }
#else
    // Draw 10,000 pixels to fill a 100x100 pixel box, better drawPixel speed test
    // use the coords as the colour to produce the banding
    byte i = 100;
    while (i--) {
        byte j = 100;
        while (j--) ttgo->eTFT->drawPixel(i + TFT_W / 2 - 50, j + TFT_H / 2 - 50, i + j);
    }
#endif

    delay(DELAY);

    ttgo->eTFT->fillScreen(TFT_BLUE);
    ttgo->eTFT->fillRoundRect(20, 20, TFT_W - 40, TFT_H - 60, 6, TFT_RED);

    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_RED);
    ttgo->eTFT->drawCentreString("That's it!", TFT_W / 2 - 1, 23, 2);
    ttgo->eTFT->drawCentreString("Restarting in a", TFT_W / 2 - 1, 40, 2);
    ttgo->eTFT->drawCentreString("few seconds...", TFT_W / 2 - 1, 57, 2);

    runTime = millis() - runTime;
    ttgo->eTFT->setTextColor(TFT_GREEN, TFT_BLUE);
    ttgo->eTFT->drawCentreString("Draw time: (msecs)", TFT_W / 2, TFT_H - 34, 2);
    ttgo->eTFT->drawNumber(runTime - 11 * DELAY, TFT_W / 2 - 20, TFT_H - 17, 2);
    delay (5000);
}


