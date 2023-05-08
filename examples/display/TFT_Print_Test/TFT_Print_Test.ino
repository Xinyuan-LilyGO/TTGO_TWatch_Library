/*
 Test the watch.print() viz embedded watch.write() function

 This sketch used font 2, 4, 7

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */


#include <LilyGoLib.h> // Graphics and font library for ILI9341 driver chip


#define TFT_GREY 0x5AEB // New colour

// Invoke library


void setup(void)
{
    watch.begin();
    watch.setRotation(2);
}

void loop()
{

    // Fill screen with grey so we can see the effect of printing with and without
    // a background colour defined
    watch.fillScreen(TFT_GREY);

    // Set "cursor" at top left corner of display (0,0) and select font 2
    // (cursor will move to next line automatically during printing with 'watch.println'
    //  or stay on the line is there is room for the text with watch.print)
    watch.setCursor(0, 0, 2);
    // Set the font colour to be white with a black background, set text size multiplier to 1
    watch.setTextColor(TFT_WHITE, TFT_BLACK);  watch.setTextSize(1);
    // We can now plot text on screen using the "print" class
    watch.println("Hello World!");

    // Set the font colour to be yellow with no background, set to font 7
    watch.setTextColor(TFT_YELLOW); watch.setTextFont(7);
    watch.println(1234.56);

    // Set the font colour to be red with black background, set to font 4
    watch.setTextColor(TFT_RED, TFT_BLACK);    watch.setTextFont(4);
    //watch.println(3735928559L, HEX); // Should print DEADBEEF

    // Set the font colour to be green with black background, set to font 4
    watch.setTextColor(TFT_GREEN, TFT_BLACK);
    watch.setTextFont(4);
    watch.println("Groop");
    watch.println("I implore thee,");

    // Change to font 2
    watch.setTextFont(2);
    watch.println("my foonting turlingdromes.");
    watch.println("And hooptiously drangle me");
    watch.println("with crinkly bindlewurdles,");
    // This next line is deliberately made too long for the display width to test
    // automatic text wrapping onto the next line
    watch.println("Or I will rend thee in the gobberwarts with my blurglecruncheon, see if I don't!");

    // Test some print formatting functions
    float fnumber = 123.45;
    // Set the font colour to be blue with no background, set to font 4
    watch.setTextColor(TFT_BLUE);    watch.setTextFont(4);
    watch.print("Float = "); watch.println(fnumber);           // Print floating point number
    watch.print("Binary = "); watch.println((int)fnumber, BIN); // Print as integer value in binary
    watch.print("Hexadecimal = "); watch.println((int)fnumber, HEX); // Print as integer number in Hexadecimal
    delay(10000);
}



