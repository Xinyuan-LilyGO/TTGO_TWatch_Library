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

#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font

#include "Free_Fonts.h" // Include the header file attached to this sketch

#include <TTGO.h>

TTGOClass *ttgo;


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
    ttgo->eTFT->setTextDatum(MC_DATUM);

    // Set text colour to orange with black background
    ttgo->eTFT->setTextColor(TFT_WHITE, TFT_BLACK);

    ttgo->eTFT->fillScreen(TFT_BLACK);            // Clear screen
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF1, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF1);                 // Select the font
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);// Print the string name of the font
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF2, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF2);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF3, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF3);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF4, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF4);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF5, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF5);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF6, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF6);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF7, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF7);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF8, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF8);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF9, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF9);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF10, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF10);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF11, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF11);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF12, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF12);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF13, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF13);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF14, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF14);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF15, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF15);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF16, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF16);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF17, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF17);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF18, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF18);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF19, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF19);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF20, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF20);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF21, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF21);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF22, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF22);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF23, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF23);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF24, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF24);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF25, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF25);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF26, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF26);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF27, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF27);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF28, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF28);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF29, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF29);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF30, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF30);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF31, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF31);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF32, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF32);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF33, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF33);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF34, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF34);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF35, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF35);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF36, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF36);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF37, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF37);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF38, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF38);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF39, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF39);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF40, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF40);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF41, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF41);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF42, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF42);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF43, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF43);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF44, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF44);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF45, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF45);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF46, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF46);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF47, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF47);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //ttgo->eTFT->fillScreen(TFT_BLACK);
    ttgo->eTFT->setFreeFont(FF18);                 // Select the font
    ttgo->eTFT->drawString(sFF48, 160, 60, GFXFF);// Print the string name of the font
    ttgo->eTFT->setFreeFont(FF48);
    ttgo->eTFT->drawString(TEXT, 160, 120, GFXFF);
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

