/*
  This example draws fonts (as used by the Adafruit_GFX library) onto the
  screen. These fonts are called the GFX Free Fonts (GFXFF) in this library.

  Other True Type fonts could be converted using the utility within the
  "fontconvert" folder inside the library. This converted has also been
  copied from the Adafruit_GFX library.

  Since these fonts are a recent addition Adafruit do not have a tutorial
  available yet on how to use the utility.   Linux users will no doubt
  figure it out!  In the meantime there are 48 font files to use in sizes
  from 9 point to 24 point, and in normal, bold, and italic or oblique
  styles.

  This example sketch uses both the print class and drawString() functions
  to plot text to the screen.

  Make sure LOAD_GFXFF is defined in the User_Setup.h file within the
  TFT_eSPI library folder.

  --------------------------- NOTE ----------------------------------------
  The free font encoding format does not lend itself easily to plotting
  the background without flicker. For values that changes on screen it is
  better to use Fonts 1- 8 which are encoded specifically for rapid
  drawing with background.
  -------------------------------------------------------------------------

  >>>>>>>>>>>>>>>>>>>>>>>>>>> WARNING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  As supplied with the default settings the sketch has 11 fonts loaded,
  i.e. GLCD (Font 1), Font 2, Font 4, Font 6, Font 7, Font 8 and five Free Fonts,
  even though they are not all used in the sketch.

  Disable fonts you do not need in User_Setup.h in the library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/


#include <LilyGoLib.h>

#include "Free_Fonts.h" // Include the header file attached to this sketch

// Use hardware SPI


unsigned long drawTime = 0;

void setup(void)
{

    watch.begin();

    watch.setRotation(1);

}

void loop()
{

    int xpos =  0;
    int ypos = 40;

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Select different fonts to draw on screen using the print class
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    header("Using print() method", TFT_NAVY);

    // For comaptibility with Adafruit_GFX library the text background is not plotted when using the print class
    // even if we specify it.
    watch.setTextColor(TFT_YELLOW);
    watch.setCursor(xpos, ypos);    // Set cursor near top left corner of screen

    watch.setFreeFont(TT1);     // Select the orginal small TomThumb font
    watch.println();             // Move cursor down a line
    watch.print("The really tiny TomThumb font");    // Print the font name onto the TFT screen
    watch.println();
    watch.println();

    watch.setFreeFont(FSB9);   // Select Free Serif 9 point font, could use:
    // watch.setFreeFont(&FreeSerif9pt7b);
    watch.println();          // Free fonts plot with the baseline (imaginary line the letter A would sit on)
    // as the datum, so we must move the cursor down a line from the 0,0 position
    watch.print("Serif Bold 9pt");  // Print the font name onto the TFT screen

    watch.setFreeFont(FSB12);       // Select Free Serif 12 point font
    watch.println();                // Move cursor down a line
    watch.print("Serif Bold 12pt"); // Print the font name onto the TFT screen

    watch.setFreeFont(FSB18);       // Select Free Serif 12 point font
    watch.println();                // Move cursor down a line
    watch.print("Serif Bold 18pt"); // Print the font name onto the TFT screen

    watch.setFreeFont(FSB24);       // Select Free Serif 24 point font
    watch.println();                // Move cursor down a line
    watch.print("Serif Bold 24pt"); // Print the font name onto the TFT screen


    delay(4000);

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Now use drawString() so we can set font background colours and the datum
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    header("Using drawString()", TFT_BLACK);

    watch.setTextColor(TFT_WHITE, TFT_BLACK);

    watch.setTextDatum(TC_DATUM); // Centre text on x,y position

    xpos = watch.width() / 2; // Half the screen width
    ypos = 50;

    watch.setFreeFont(FSB9);                              // Select the font
    watch.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
    ypos += watch.fontHeight(GFXFF);                      // Get the font height and move ypos down

    watch.setFreeFont(FSB12);
    watch.drawString("Serif Bold 12pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    watch.setFreeFont(FSB18);
    watch.drawString("Serif Bold 18pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    watch.setFreeFont(FSB24);
    watch.drawString("Serif Bold 24pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    // Set text padding to 100 pixels wide area to over-write old values on screen
    watch.setTextPadding(100);
    for (int i = 0; i <= 20; i++) {
        watch.drawFloat(i / 10.0, 1, xpos, ypos, GFXFF);
        delay (200);
    }

    delay(4000);

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Same again but with colours that show bounding boxes
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


    header("With background", TFT_DARKGREY);

    watch.setTextColor(TFT_YELLOW, TFT_BLACK);

    watch.setTextDatum(TC_DATUM); // Centre text on x,y position

    xpos = watch.width() / 2; // Half the screen width
    ypos = 50;

    watch.setFreeFont(FSB9);                              // Select the font
    watch.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
    ypos += watch.fontHeight(GFXFF);                        // Get the font height and move ypos down

    watch.setFreeFont(FSB12);
    watch.drawString("Serif Bold 12pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    watch.setFreeFont(FSB18);
    watch.drawString("Serif Bold 18pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    watch.setFreeFont(FSBI24);
    watch.drawString("Bold Italic 24pt", xpos, ypos, GFXFF);
    ypos += watch.fontHeight(GFXFF);

    // Set text padding to 100 pixels wide area to over-write old values on screen
    watch.setTextPadding(100);
    for (int i = 0; i <= 20; i++) {
        watch.drawFloat(i / 10.0, 1, xpos, ypos, GFXFF);
        delay (200);
    }

    delay(4000);

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // Now show setting the 12 datum positions works with free fonts
    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // Numbers, floats and strings can be drawn relative to a datum
    header("Text with a datum", TFT_BLACK);
    watch.setTextColor(TFT_DARKGREY, TFT_BLACK);
    watch.setFreeFont(FSS12);
    watch.setTextDatum(TL_DATUM);
    watch.drawString("[Top left]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(TC_DATUM);
    watch.drawString("[Top centre]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(TR_DATUM);
    watch.drawString("[Top right]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(ML_DATUM);
    watch.drawString("[Middle left]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(MC_DATUM);
    watch.drawString("[Middle centre]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(MR_DATUM);
    watch.drawString("[Middle right]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(BL_DATUM);
    watch.drawString("[Bottom left]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(BC_DATUM);
    watch.drawString("[Bottom centre]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(BR_DATUM);
    watch.drawString("[Bottom right]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(L_BASELINE);
    watch.drawString("[Left baseline]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(C_BASELINE);
    watch.drawString("[Centre baseline]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    watch.fillRect(0, 80, 320, 80, TFT_BLACK);
    watch.setTextDatum(R_BASELINE);
    watch.drawString("[Right baseline]", 160, 120, GFXFF);
    drawDatumMarker(160, 120);
    delay(1000);

    //while(1);
    delay(8000);

}

// Print the header for a display screen
void header(const char *string, uint16_t color)
{
    watch.fillScreen(color);
    watch.setTextSize(1);
    watch.setTextColor(TFT_MAGENTA, TFT_BLUE);
    watch.fillRect(0, 0, 320, 30, TFT_BLUE);
    watch.setTextDatum(TC_DATUM);
    watch.drawString(string, 160, 2, 4); // Font 4 for fast drawing with background
}

// Draw a + mark centred on x,y
void drawDatumMarker(int x, int y)
{
    watch.drawLine(x - 5, y, x + 5, y, TFT_GREEN);
    watch.drawLine(x, y - 5, x, y + 5, TFT_GREEN);
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

#ifndef LOAD_FONT2
//ERROR_Please_enable_LOAD_FONT2_in_User_Setup!
#endif

#ifndef LOAD_FONT4
//ERROR_Please_enable_LOAD_FONT4_in_User_Setup!
#endif

#ifndef LOAD_FONT6
//ERROR_Please_enable_LOAD_FONT6_in_User_Setup!
#endif

#ifndef LOAD_FONT7
//ERROR_Please_enable_LOAD_FONT7_in_User_Setup!
#endif

#ifndef LOAD_FONT8
//ERROR_Please_enable_LOAD_FONT8_in_User_Setup!
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif

