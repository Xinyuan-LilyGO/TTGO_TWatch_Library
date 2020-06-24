/*
  Example for TFT_eSPI library

  Created by Bodmer 31/12/16

  This example draws all fonts (as used by the Adafruit_GFX library) onto the
  screen. These fonts are called the GFX Free Fonts (GFXFF) in this library.

  The fonts are referenced by a short name, see the Free_Fonts.h file
  attached to this sketch.

  Other True Type fonts could be converted using the utility within the
  "fontconvert" folder inside the library. This converted has also been
  copied from the Adafruit_GFX library.

  Since these fonts are a recent addition Adafruit do not have a tutorial
  available yet on how to use the fontconvert utility.   Linux users will
  no doubt figure it out!  In the meantime there are 48 font files to use
  in sizes from 9 point to 24 point, and in normal, bold, and italic or
  oblique styles.

  This example sketch uses both the print class and drawString() functions
  to plot text to the screen.

  Make sure LOAD_GFXFF is defined in the User_Setup.h file within the
  library folder.

  --------------------------- NOTE ----------------------------------------
  The free font encoding format does not lend itself easily to plotting
  the background without flicker. For values that changes on screen it is
  better to use Fonts 1- 8 which are encoded specifically for rapid
  drawing with background.
  -------------------------------------------------------------------------

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT YOUR DISPLAY TYPE AND ENABLE FONTS          ######
  #########################################################################
*/

#include "config.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch

TTGOClass *ttgo;

#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font

unsigned long drawTime = 0;

void setup(void)
{
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
}

void loop()
{

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Show all 48 fonts in centre of screen ( x,y coordinate 160,120)
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // Where font sizes increase the screen is not cleared as the larger fonts overwrite
    // the smaller one with the background colour.

    // Set text datum to middle centre
    ttgo->tft->setTextDatum(MC_DATUM);

    // Set text colour to orange with black background
    ttgo->tft->setTextColor(TFT_WHITE, TFT_BLACK);

    ttgo->tft->fillScreen(TFT_BLACK);            // Clear screen
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF1, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF1);                 // Select the font
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);// Print the string name of the font
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF2, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF2);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF3, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF3);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF4, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF4);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF5, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF5);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF6, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF6);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF7, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF7);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF8, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF8);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF9, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF9);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF10, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF10);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF11, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF11);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF12, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF12);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF13, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF13);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF14, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF14);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF15, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF15);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF16, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF16);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF17, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF17);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF18, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF18);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF19, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF19);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF20, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF20);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF21, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF21);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF22, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF22);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF23, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF23);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF24, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF24);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF25, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF25);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF26, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF26);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF27, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF27);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF28, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF28);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF29, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF29);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF30, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF30);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF31, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF31);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF32, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF32);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF33, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF33);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF34, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF34);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF35, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF35);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF36, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF36);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF37, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF37);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF38, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF38);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF39, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF39);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF40, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF40);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF41, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF41);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF42, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF42);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF43, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF43);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF44, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF44);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF45, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF45);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF46, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF46);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF47, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF47);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setFreeFont(FF18);                 // Select the font
    ttgo->tft->drawString(sFF48, 160, 60, GFXFF);// Print the string name of the font
    ttgo->tft->setFreeFont(FF48);
    ttgo->tft->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

}

// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enbabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif

