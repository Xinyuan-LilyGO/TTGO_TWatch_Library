/*
Tests string alignment

Normally strings are printed relative to the top left corner but this can be
changed with the setTextDatum() function. The library has #defines for:

TL_DATUM = Top left
TC_DATUM = Top centre
TR_DATUM = Top right
ML_DATUM = Middle left
MC_DATUM = Middle centre
MR_DATUM = Middle right
BL_DATUM = Bottom left
BC_DATUM = Bottom centre
BR_DATUM = Bottom right


 Needs fonts 2, 4, 6, 7 and 8

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */


#include <LilyGoLib.h> // Hardware-specific library


// Invoke custom library

unsigned long drawTime = 0;

void setup(void)
{
    Serial.begin(115200);
    watch.begin();
    watch.setRotation(1);
}

void loop()
{

    watch.fillScreen(TFT_BLACK);

    for (byte datum = 0; datum < 9; datum++) {
        watch.setTextColor(TFT_WHITE, TFT_BLACK);

        watch.setTextDatum(datum);

        watch.drawNumber(88, 160, 120, 8);
        watch.fillCircle(160, 120, 5, TFT_RED);

        watch.setTextDatum(MC_DATUM);

        watch.setTextColor(TFT_BLACK);
        watch.drawString("X", 160, 120, 2);
        delay(1000);
        watch.fillScreen(TFT_BLACK);
    }

    watch.setTextColor(TFT_BLUE, TFT_BLACK);
    watch.drawCentreString("69", 160, 120, 8);
    watch.fillCircle(160, 120, 5, TFT_YELLOW);

    watch.setTextDatum(MC_DATUM);

    watch.setTextColor(TFT_BLACK);
    watch.drawString("X", 160, 120, 2);
    delay(1000);
    watch.fillScreen(TFT_BLACK);

    watch.setTextColor(TFT_RED, TFT_BLACK);
    watch.drawRightString("88", 160, 120, 8);
    watch.fillCircle(160, 120, 5, TFT_YELLOW);

    watch.setTextDatum(MC_DATUM);

    watch.setTextColor(TFT_BLACK);
    watch.drawString("X", 160, 120, 2);
    delay(1000);
    watch.fillScreen(TFT_BLACK);

    watch.setTextColor(TFT_WHITE, TFT_BLUE);

    watch.setTextDatum(MC_DATUM);

    //Test floating point drawing function
    float test = 67.125;
    watch.drawFloat(test, 4, 160, 180, 4);
    delay(1000);
    watch.fillScreen(TFT_BLACK);
    test = -0.555555;
    watch.drawFloat(test, 3, 160, 180, 4);
    delay(1000);
    watch.fillScreen(TFT_BLACK);
    test = 0.1;
    watch.drawFloat(test, 4, 160, 180, 4);
    delay(1000);
    watch.fillScreen(TFT_BLACK);
    test = 9999999;
    watch.drawFloat(test, 1, 160, 180, 4);
    delay(1000);

    watch.fillCircle(160, 180, 5, TFT_YELLOW);

    watch.setTextDatum(MC_DATUM);

    watch.setTextColor(TFT_BLACK);
    watch.drawString("X", 160, 180, 2);

    delay(4000);
}








