/*
 Display all the fonts.

 This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 ######       TO SELECT THE FONTS AND PINS YOU USE, SEE ABOVE       ######
 #########################################################################

 */

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000

#include <LilyGoLib.h> // Graphics and font library for ILI9341 driver chip


// Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0; // Used for testing draw times

void setup(void)
{
    watch.begin();
    watch.setRotation(1);
}

void loop()
{
    targetTime = millis();

    // First we test them with a background colour set
    watch.setTextSize(1);
    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_GREEN, TFT_BLACK);

    watch.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
    watch.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
    watch.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
    watch.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
    int xpos = 0;
    xpos += watch.drawString("xyz{|}~", 0, 64, 2);
    watch.drawChar(127, xpos, 64, 2);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_GREEN, TFT_BLACK);

    watch.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
    watch.drawString("/0123456789:;", 0, 26, 4);
    watch.drawString("<=>?@ABCDE", 0, 52, 4);
    watch.drawString("FGHIJKLMNO", 0, 78, 4);
    watch.drawString("PQRSTUVWX", 0, 104, 4);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.drawString("YZ[\\]^_`abc", 0, 0, 4);
    watch.drawString("defghijklmno", 0, 26, 4);
    watch.drawString("pqrstuvwxyz", 0, 52, 4);
    xpos = 0;
    xpos += watch.drawString("{|}~", 0, 78, 4);
    watch.drawChar(127, xpos, 78, 4);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_BLUE, TFT_BLACK);

    watch.drawString("012345", 0, 0, 6);
    watch.drawString("6789", 0, 40, 6);
    watch.drawString("apm-:.", 0, 80, 6);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_RED, TFT_BLACK);

    watch.drawString("0123", 0, 0, 7);
    watch.drawString("4567", 0, 60, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.drawString("890:.", 0, 0, 7);
    watch.drawString("", 0, 60, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_YELLOW, TFT_BLACK);

    watch.drawString("0123", 0, 0, 8);
    watch.drawString("4567", 0, 72, 8);
    delay(WAIT);;

    watch.fillScreen(TFT_BLACK);
    watch.drawString("890:.", 0, 0, 8);
    watch.drawString("", 0, 72, 8);
    delay(WAIT);;

    watch.setTextSize(2);
    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_GREEN, TFT_BLACK);

    watch.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
    watch.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
    watch.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
    watch.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
    xpos = 0;
    xpos += watch.drawString("xyz{|}~", 0, 128, 2);
    watch.drawChar(127, xpos, 128, 2);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_GREEN, TFT_BLACK);

    watch.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
    watch.drawString("/0123456789:;", 0, 52, 4);
    watch.drawString("<=>?@ABCDE", 0, 104, 4);
    watch.drawString("FGHIJKLMNO", 0, 156, 4);
    watch.drawString("PQRSTUVWX", 0, 208, 4);
    delay(WAIT);
    watch.fillScreen(TFT_BLACK);
    watch.drawString("YZ[\\]^_`abc", 0, 0, 4);
    watch.drawString("defghijklmno", 0, 52, 4);
    watch.drawString("pqrstuvwxyz", 0, 104, 4);
    xpos = 0;
    xpos += watch.drawString("{|}~", 0, 156, 4);
    watch.drawChar(127, xpos, 156, 4);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_BLUE, TFT_BLACK);

    watch.drawString("01234", 0, 0, 6);
    watch.drawString("56789", 0, 80, 6);
    watch.drawString("apm-:.", 0, 160, 6);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_RED, TFT_BLACK);

    watch.drawString("0123", 0, 0, 7);
    watch.drawString("4567", 0, 120, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.drawString("890:.", 0, 0, 7);
    watch.drawString("", 0, 120, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BLACK);
    watch.setTextColor(TFT_YELLOW, TFT_BLACK);

    watch.drawString("0123", 0, 0, 8);
    watch.drawString("4567", 0, 144, 8);
    delay(WAIT);;

    watch.fillScreen(TFT_BLACK);
    watch.drawString("890:.", 0, 0, 8);
    watch.drawString("", 0, 144, 8);
    delay(WAIT);;

    watch.setTextColor(TFT_MAGENTA, TFT_BROWN);

    watch.drawNumber(millis() - targetTime, 0, 180, 4);
    delay(4000);;

    // Now test them with transparent background
    targetTime = millis();

    watch.setTextSize(1);
    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_GREEN);

    watch.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
    watch.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
    watch.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
    watch.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
    xpos = 0;
    xpos += watch.drawString("xyz{|}~", 0, 64, 2);
    watch.drawChar(127, xpos, 64, 2);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_GREEN);

    watch.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
    watch.drawString("/0123456789:;", 0, 26, 4);
    watch.drawString("<=>?@ABCDE", 0, 52, 4);
    watch.drawString("FGHIJKLMNO", 0, 78, 4);
    watch.drawString("PQRSTUVWX", 0, 104, 4);

    delay(WAIT);
    watch.fillScreen(TFT_BROWN);
    watch.drawString("YZ[\\]^_`abc", 0, 0, 4);
    watch.drawString("defghijklmno", 0, 26, 4);
    watch.drawString("pqrstuvwxyz", 0, 52, 4);
    xpos = 0;
    xpos += watch.drawString("{|}~", 0, 78, 4);
    watch.drawChar(127, xpos, 78, 4);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_BLUE);

    watch.drawString("012345", 0, 0, 6);
    watch.drawString("6789", 0, 40, 6);
    watch.drawString("apm-:.", 0, 80, 6);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_RED);

    watch.drawString("0123", 0, 0, 7);
    watch.drawString("4567", 0, 60, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.drawString("890:.", 0, 0, 7);
    watch.drawString("", 0, 60, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_YELLOW);

    watch.drawString("0123", 0, 0, 8);
    watch.drawString("4567", 0, 72, 8);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.drawString("890:.", 0, 0, 8);
    watch.drawString("", 0, 72, 8);
    delay(WAIT);

    watch.setTextSize(2);
    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_GREEN);

    watch.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
    watch.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
    watch.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
    watch.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
    xpos = 0;
    xpos += watch.drawString("xyz{|}~", 0, 128, 2);
    watch.drawChar(127, xpos, 128, 2);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_GREEN);

    watch.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
    watch.drawString("/0123456789:;", 0, 52, 4);
    watch.drawString("<=>?@ABCDE", 0, 104, 4);
    watch.drawString("FGHIJKLMNO", 0, 156, 4);
    watch.drawString("PQRSTUVWX", 0, 208, 4);
    delay(WAIT);
    watch.fillScreen(TFT_BROWN);
    watch.drawString("YZ[\\]^_`abc", 0, 0, 4);
    watch.drawString("defghijklmno", 0, 52, 4);
    watch.drawString("pqrstuvwxyz", 0, 104, 4);
    xpos = 0;
    xpos += watch.drawString("{|}~", 0, 156, 4);
    watch.drawChar(127, xpos, 156, 4);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_BLUE);

    watch.drawString("01234", 0, 0, 6);
    watch.drawString("56789", 0, 80, 6);
    watch.drawString("apm-:.", 0, 160, 6);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_RED);

    watch.drawString("0123", 0, 0, 7);
    watch.drawString("4567", 0, 120, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.drawString("890:.", 0, 0, 7);
    watch.drawString("", 0, 120, 7);
    delay(WAIT);

    watch.fillScreen(TFT_BROWN);
    watch.setTextColor(TFT_YELLOW);

    watch.drawString("0123", 0, 0, 8);
    watch.drawString("4567", 0, 144, 8);
    delay(WAIT);;

    watch.fillScreen(TFT_BROWN);
    watch.drawString("890:.", 0, 0, 8);
    watch.drawString("", 0, 144, 8);
    delay(WAIT);;

    watch.setTextColor(TFT_MAGENTA);

    watch.drawNumber(millis() - targetTime, 0, 180, 4);
    delay(4000);;
}

