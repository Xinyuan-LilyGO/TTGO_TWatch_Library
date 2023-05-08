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


#include <LilyGoLib.h>

// Use hardware SPI


unsigned long drawTime = 0;

void setup(void)
{

    watch.begin();

    watch.setRotation(1);

}

void loop()
{

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Show all 48 fonts in centre of screen ( x,y coordinate 160,120)
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // Where font sizes increase the screen is not cleared as the larger fonts overwrite
    // the smaller one with the background colour.

    // Set text datum to middle centre
    watch.setTextDatum(MC_DATUM);

    // Set text colour to orange with black background
    watch.setTextColor(TFT_WHITE, TFT_BLACK);

    watch.fillScreen(TFT_BLACK);            // Clear screen
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF1, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF1);                 // Select the font
    watch.drawString(TEXT, 160, 120, GFXFF);// Print the string name of the font
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF2, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF2);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF3, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF3);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF4, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF4);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF5, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF5);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF6, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF6);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF7, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF7);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF8, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF8);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF9, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF9);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF10, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF10);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF11, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF11);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF12, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF12);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF13, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF13);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF14, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF14);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF15, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF15);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF16, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF16);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF17, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF17);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF18, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF18);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF19, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF19);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF20, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF20);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF21, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF21);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF22, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF22);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF23, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF23);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF24, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF24);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF25, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF25);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF26, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF26);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF27, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF27);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF28, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF28);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF29, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF29);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF30, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF30);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF31, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF31);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF32, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF32);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF33, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF33);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF34, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF34);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF35, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF35);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF36, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF36);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF37, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF37);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF38, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF38);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF39, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF39);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF40, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF40);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF41, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF41);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF42, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF42);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF43, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF43);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF44, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF44);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);

    watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF45, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF45);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF46, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF46);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF47, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF47);
    watch.drawString(TEXT, 160, 120, GFXFF);
    delay(1000);
    //watch.fillScreen(TFT_BLACK);
    watch.setFreeFont(FF18);                 // Select the font
    watch.drawString(sFF48, 160, 60, GFXFF);// Print the string name of the font
    watch.setFreeFont(FF48);
    watch.drawString(TEXT, 160, 120, GFXFF);
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

