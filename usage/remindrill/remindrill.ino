/*
 An example digital clock using a TFT LCD screen to show the time.
 Demonstrates use of the font printing routines. (Time updates but date does not.)

 It uses the time of compile/upload to set the time
 For a more accurate clock, it would be better to use the RTClib library.
 But this is just a demo...

 Make sure all the display driver and pin connections are correct by
 editing the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################

 Based on clock sketch by Gilchrist 6/2/2014 1.0

A few colour codes:

code    color
0x0000  Black
0xFFFF  White
0xBDF7  Light Gray
0x7BEF  Dark Gray
0xF800  Red
0xFFE0  Yellow
0xFBE0  Orange
0x79E0  Brown
0x7E0   Green
0x7FF   Cyan
0x1F    Blue
0xF81F  Pink

 */

#include <LilyGoLib.h> // Hardware-specific library


#define TFT_GREY 0x5AEB

// Invoke custom library

uint32_t targetTime = 0;                    // for next 1 second timeout

const int16_t defaultAwakeTime = 15;

uint8_t ss = 0;
int16_t sleepCountdown_sec = defaultAwakeTime;

bool  pmu_flag = false;

// ISR for Power Button (crown button)
void setPMUFlag(void)
{
    pmu_flag = true;
}



void setup(void)
{
    //Serial.begin(115200);
    watch.begin();
    watch.setRotation(2);
    watch.fillScreen(TFT_BLACK);

    watch.setTextFont(4);
    watch.setTextSize(1);
    watch.setTextColor(TFT_YELLOW, TFT_BLACK);

    targetTime = millis() + 1000;

    // Set the interrupt handler of the PMU
    watch.attachPMU(setPMUFlag);
}

void loop()
{
    if (targetTime < millis()) {
        // Set next update for 1 second later
        targetTime = millis() + 1000;
        ss++;
        sleepCountdown_sec--;


        // Update digital time
        int xpos = 120;
        int ypos = 120;

        char time_text[] = "00:00";
        strncpy_P( time_text, watch.strftime(DATETIME_FORMAT_HM), 5 );

        // Blink colon in the string of RTC time
        time_text[2] = ss % 2 ? ':' : ' ';

        watch.drawString( time_text, xpos, ypos, 7);

        if (watch.getTouched()) // reset awake timer if the screen touched
            sleepCountdown_sec = defaultAwakeTime;

        if(sleepCountdown_sec<0) {
            sleepCountdown_sec = defaultAwakeTime;
            watch.setSleepMode(PMU_BTN_WAKEUP);
            watch.sleep();
        }
    }

    // If crown button was pressed, then enforce sleep
    if (pmu_flag) {
        
        // After the PMU interrupt is triggered, the interrupt status must be cleared,
        // otherwise the next interrupt will not be triggered
        watch.clearPMU();

        //Set to wake by pressing the button on the crown
        watch.setSleepMode(PMU_BTN_WAKEUP);

        watch.sleep();
    }
}



