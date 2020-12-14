// An Arduino based framework for the Lilygo T-Watch 2020
// Much of the code is based on the sample apps for the
// T-watch that were written and copyrighted by Lewis He.
//(Copyright (c) 2019 lewis he)



// https://www.instructables.com/id/Lilygo-T-Watch-2020-Arduino-Framework/
// By DanGeiger


#include "config.h"
#include <soc/rtc.h>

TTGOClass *ttgo;

uint32_t targetTime = 0;       // for next 1 second display update
// uint32_t clockUpTime = 0;      // track the time the clock is displayed

uint8_t hh, mm, ss, mmonth, dday; // H, M, S variables
uint16_t yyear; // Year is 16 bit int

// The basic Time Display GUI
// if you are just updating the colon, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker

#include <time.h>

int getTnum();
void prtTime(byte digit);
void setMenuDisplay(int mSel);

byte xcolon = 0; // location of the colon

void displayTime(boolean fullUpdate)
{

    byte xpos = 40; // Stating position for the display
    byte ypos = 90;

    // Get the current data
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    ttgo->tft->setTextSize(1);

    if (fullUpdate) {
        // Font 7 is a 7-seg display but only contains
        // characters [space] 0 1 2 3 4 5 6 7 8 9 0 : .

        ttgo->tft->setTextColor(0x39C4, TFT_BLACK); // Set desired color
        ttgo->tft->drawString("88:88", xpos, ypos, 7);
        ttgo->tft->setTextColor(0xFBE0, TFT_BLACK); // Orange

        if (hh < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        xpos += ttgo->tft->drawNumber(hh, xpos, ypos, 7);
        xcolon = xpos + 3;
        xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
        if (mm < 10) xpos += ttgo->tft->drawChar('0', xpos, ypos, 7);
        ttgo->tft->drawNumber(mm, xpos, ypos, 7);
    }

    if (ss % 2) { // Toggle the colon every second
        ttgo->tft->setTextColor(0x39C4, TFT_BLACK);
        xpos += ttgo->tft->drawChar(':', xcolon, ypos, 7);
        ttgo->tft->setTextColor(0xFBE0, TFT_BLACK);
    } else {
        ttgo->tft->drawChar(':', xcolon, ypos, 7);
    }
    ttgo->tft->setTextSize(3);
    ttgo->tft->setCursor( 10, 210);

    ttgo->tft->print(mmonth);
    ttgo->tft->print("/");
    ttgo->tft->print(dday);
    ttgo->tft->print("/");
    ttgo->tft->print(yyear);
}


// Show the accelerometer working

void appAccel()
{
    ttgo->bma->begin();
    ttgo->bma->enableAccel();
    ttgo->tft->fillScreen(TFT_BLACK);
    int16_t x, y;
    int16_t xpos, ypos;

    Accel acc;

    while (!ttgo->getTouch(x, y)) { // Wait for touch to exit

        ttgo->bma->getAccel(acc);
        xpos = acc.x;
        ypos = acc.y;
        ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_RED); // draw dot
        delay(100);
        ttgo->tft->fillCircle(xpos / 10 + 119, ypos / 10 + 119, 10, TFT_BLACK); // erase previous dot
    }

    while (ttgo->getTouch(x, y)) {}  // Wait for release to return to the clock

    ttgo->tft->fillScreen(TFT_BLACK); // Clear screen
}

//
// Make the following updates:
// 1) Update maxApp to the total number of apps.
// 2) Update appName to add the title of the app.
// 3) in the main routine in TWatch_framework, add a case to the switch statement to call your app routine.


const int maxApp = 6; // number of apps
String appName[maxApp] = {"Clock", "Jupiter", "Accel", "Battery", "Touch", "Set Time"}; // app names

uint8_t modeMenu()
{
    int mSelect = 0; // The currently highlighted app
    int16_t x, y;

    boolean exitMenu = false; // used to stay in the menu until user selects app

    setMenuDisplay(0); // display the list of Apps

    while (!exitMenu) {
        if (ttgo->getTouch(x, y)) { // If you have touched something...

            while (ttgo->getTouch(x, y)) {} // wait until you stop touching

            if (y >= 160) { // you want the menu list shifted up
                mSelect += 1;
                if (mSelect == maxApp) mSelect = 0;
                setMenuDisplay(mSelect);
            }

            if (y <= 80) { // you want the menu list shifted down
                mSelect -= 1;
                if (mSelect < 0) mSelect = maxApp - 1;
                setMenuDisplay(mSelect);
            }
            if (y > 80 && y < 160) { // You selected the middle
                exitMenu = true;
            }
        }

    }
    //Return with mSelect containing the desired mode
    ttgo->tft->fillScreen(TFT_BLACK);
    return mSelect;

}

void setMenuDisplay(int mSel)
{

    int curSel = 0;
    // Display mode header
    ttgo->tft->fillScreen(TFT_BLUE);
    ttgo->tft->fillRect(0, 80, 239, 80, TFT_BLACK);

    // Display apps
    if (mSel == 0) curSel = maxApp - 1;
    else curSel = mSel - 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 30);
    ttgo->tft->println(appName[curSel]);

    ttgo->tft->setTextSize(3);
    ttgo->tft->setTextColor(TFT_RED);
    ttgo->tft->setCursor(40, 110);
    ttgo->tft->println(appName[mSel]);

    if (mSel == maxApp - 1) curSel = 0;
    else curSel = mSel + 1;

    ttgo->tft->setTextSize(2);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->setCursor(50, 190);
    ttgo->tft->print(appName[curSel]);
}

// Check out the touch screen
// Will display the x and y coordinates of where you touch
// for 10 seconds and then return to time

void appTouch()
{
    uint32_t endTime = millis() + 10000; // Timeout at 10 seconds
    int16_t x, y;
    ttgo->tft->fillScreen(TFT_BLACK);

    while (endTime > millis()) {
        ttgo->getTouch(x, y);
        ttgo->tft->fillRect(98, 100, 70, 85, TFT_BLACK);
        ttgo->tft->setCursor(80, 100);
        ttgo->tft->print("X:");
        ttgo->tft->println(x);
        ttgo->tft->setCursor(80, 130);
        ttgo->tft->print("Y:");
        ttgo->tft->println(y);
        delay(25);
    }

    while (ttgo->getTouch(x, y)) {}  // Wait for release to exit
    ttgo->tft->fillScreen(TFT_BLACK);
}

// Set the time - no error checking, you might want to add it

void appSetTime()
{
    // Get the current info
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

//Set up the interface buttons

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->fillRect(0, 35, 80, 50, TFT_BLUE);
    ttgo->tft->fillRect(161, 35, 78, 50, TFT_BLUE);
    ttgo->tft->fillRect(81, 85, 80, 50, TFT_BLUE);
    ttgo->tft->fillRect(0, 135, 80, 50, TFT_BLUE);
    ttgo->tft->fillRect(161, 135, 78, 50, TFT_BLUE);
    ttgo->tft->fillRect(0, 185, 80, 50, TFT_BLUE);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->drawNumber(1, 30, 40, 2);
    ttgo->tft->drawNumber(2, 110, 40, 2);
    ttgo->tft->drawNumber(3, 190, 40, 2);
    ttgo->tft->drawNumber(4, 30, 90, 2);
    ttgo->tft->drawNumber(5, 110, 90, 2);
    ttgo->tft->drawNumber(6, 190, 90, 2);
    ttgo->tft->drawNumber(7, 30, 140, 2);
    ttgo->tft->drawNumber(8, 110, 140, 2);
    ttgo->tft->drawNumber(9, 190, 140, 2);
    ttgo->tft->drawNumber(0, 30, 190, 2);
    ttgo->tft->fillRoundRect(120, 200, 119, 39, 6, TFT_WHITE);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor(0, 0);

    ttgo->tft->setCursor(155, 210);
    ttgo->tft->setTextColor(TFT_BLACK);
    ttgo->tft->print("DONE");
    ttgo->tft->setTextColor(TFT_WHITE);
    int wl = 0; // Track the current number selected
    byte curnum = 1;  // Track which digit we are on

    prtTime(curnum); // Display the time for the current digit

    while (wl != 13) {
        wl = getTnum();
        if (wl != -1 && wl != 13)

            switch (curnum) {
            case 1:
                hh = wl * 10 + hh % 10;
                break;
            case 2:
                hh = int(hh / 10) * 10 + wl;
                break;
            case 3:
                mm = wl * 10 + mm % 10;
                break;
            case 4:
                mm = int(mm / 10) * 10 + wl;
                break;
            }
        while (getTnum() != -1) {}
        curnum += 1;
        if (curnum > 4) curnum = 1;
        prtTime(curnum);
    }
    while (getTnum() != -1)
    {}
    ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
    ttgo->tft->fillScreen(TFT_BLACK);
}

// prtTime will display the current selected time and highlight
// the current digit to be updated in yellow

void prtTime(byte digit)
{
    ttgo->tft->fillRect(0, 0, 100, 34, TFT_BLACK);
    if (digit == 1)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(int(hh / 10), 5, 5, 2);
    if (digit == 2)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(hh % 10, 25, 5, 2);
    ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawString(":",  45, 5, 2);
    if (digit == 3)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(int(mm / 10), 65, 5, 2);
    if (digit == 4)   ttgo->tft->setTextColor(TFT_YELLOW);
    else   ttgo->tft->setTextColor(TFT_WHITE);
    ttgo->tft->drawNumber(mm % 10, 85, 5, 2);
}

// getTnum takes care of translating where we pressed into
// a number that was pressed. Returns -1 for no press
// and 13 for DONE

int getTnum()
{
    int16_t x, y;
    if (!ttgo->getTouch(x, y)) return - 1;
    if (y < 85) {
        if (x < 80) return 1;
        else if (x > 160) return 3;
        else return 2;
    } else if (y < 135) {
        if (x < 80) return 4;
        else if (x > 160) return 6;
        else return 5;
    } else if (y < 185) {
        if (x < 80) return 7;
        else if (x > 160) return 9;
        else return 8;
    } else if (x < 80) return 0;
    else return 13;
}

// Display Jupiters 4 moons
// It uses the current date time and corrects to get UTC time
// Make sure you set the correct Time Zone below

#define APP_TIME_ZONE   -4  // I am East Coast in Daylight Savings

void jSats()
{
    // Get the current info
    RTC_Date tnow = ttgo->rtc->getDateTime();

    hh = tnow.hour;
    mm = tnow.minute;
    ss = tnow.second;
    dday = tnow.day;
    mmonth = tnow.month;
    yyear = tnow.year;

    float tDay = dday; // Calculate the day plus fractional day
    tDay += (hh - APP_TIME_ZONE) / 24.0;
    tDay += mm / 1440.0;
    int16_t tYear = yyear;
    int8_t tMonth = mmonth;

    int16_t cYear, cMonth;
    if (tMonth < 3) {
        cYear = tYear - 1;
        cMonth = tMonth + 12;
    } else {
        cYear = tYear;
        cMonth = tMonth;
    }
    // Calculate the Julian Date offset from Epoch
    int a = cYear / 100;
    int b = 2 - a + (int)(a / 4);
    long c = 365.25 * cYear;
    long d = 30.6001 * (cMonth + 1);
    float N = b + c + d + tDay - 694025.5;

    // Calc moon positions
    float P = PI / 180;
    float MT = (358.476 + 0.9856003 * N) * P;
    float MJ = (225.328 + 0.0830853 * N) * P;
    float JJ = 221.647 + 0.9025179 * N;
    float VT = 1.92 * sin(MT) + 0.02 * sin(2 * MT);
    float VJ = 5.55 * sin(MJ) + 0.17 * sin(2 * MJ);
    float K = (JJ + VT - VJ) * P;
    float DT = sqrt(28.07 - 10.406 * cos(K));
    float Z1 = sin(K) / DT;
    float I = atan(Z1 / sqrt(1 - Z1 * Z1));
    I = I / P;
    float F = (N - DT / 173);
    float F1 = I - VJ;
    float U1 = 84.5506 + 203.405863 * F + F1;
    float U2 = 41.5015 + 101.2916323 * F + F1;
    float U3 = 109.9770 + 50.2345169 * F + F1;
    float U4 = 176.3586 + 21.4879802 * F + F1;
    float X1 = 5.906 * sin(U1 * P);
    float X2 = 9.397 * sin(U2 * P);
    float X3 = 14.989 * sin(U3 * P);
    float X4 = 26.364 * sin(U4 * P);
    // Print out results

    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextSize(2);
    ttgo->tft->setCursor( 0, 10);
    ttgo->tft->setTextColor(TFT_ORANGE);
    ttgo->tft->print(" IO: ");
    ttgo->tft->print(X1, 1);
    ttgo->tft->setCursor( 0, 30);
    ttgo->tft->setTextColor(TFT_BLUE);
    ttgo->tft->print(" EU: ");
    ttgo->tft->println(X2, 1);
    ttgo->tft->setCursor( 0, 50);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->print(" GA: ");
    ttgo->tft->println(X3, 1);
    ttgo->tft->setCursor( 0, 70);
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->print(" CA: ");
    ttgo->tft->println(X4, 1);

    //Now display them as they would appear

    ttgo->tft->fillCircle(119, 155, 6, TFT_RED); // Jupiter

    ttgo->tft->setTextColor(TFT_ORANGE);
    ttgo->tft->fillCircle(int(X1 * 4 + 119), 155, 2, TFT_ORANGE);
    ttgo->tft->drawString("I", int(X1 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_BLUE);
    ttgo->tft->fillCircle(int(X2 * 4 + 119), 155, 2, TFT_BLUE);
    ttgo->tft->drawString("E", int(X2 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_GREEN);
    ttgo->tft->fillCircle(int(X3 * 4 + 119), 155, 2, TFT_GREEN);
    ttgo->tft->drawString("G", int(X3 * 4 + 119) - 3, 175, 1);
    ttgo->tft->setTextColor(TFT_YELLOW);
    ttgo->tft->fillCircle(int(X4 * 4 + 119), 155, 2, TFT_YELLOW);
    ttgo->tft->drawString("C", int(X4 * 4 + 119) - 3, 175, 1);


    // Wait for touch to return
    int16_t x, y;
    while (!ttgo->getTouch(x, y)) {} // Wait for touch
    while (ttgo->getTouch(x, y)) {}  // Wait for release
    ttgo->tft->fillScreen(TFT_BLACK);
}

// Display Battery Data

void appBattery()
{
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK);
    ttgo->tft->drawString("BATT STATS",  35, 30, 2);
    ttgo->tft->setTextColor(TFT_GREEN, TFT_BLACK);

    // Turn on the battery adc to read the values
    ttgo->power->adc1Enable(AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1 | AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1, true);
    // get the values
    float vbus_v = ttgo->power->getVbusVoltage();
    float vbus_c = ttgo->power->getVbusCurrent();
    float batt_v = ttgo->power->getBattVoltage();
    int per = ttgo->power->getBattPercentage();

// Print the values
    ttgo->tft->setCursor(0, 100);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(0, 130);
    ttgo->tft->print("Vbus: "); ttgo->tft->print(vbus_c); ttgo->tft->println(" mA");
    ttgo->tft->setCursor(0, 160);
    ttgo->tft->print("BATT: "); ttgo->tft->print(batt_v); ttgo->tft->println(" mV");
    ttgo->tft->setCursor(0, 190);
    ttgo->tft->print("Per: "); ttgo->tft->print(per); ttgo->tft->println(" %");

    int16_t x, y;
    while (!ttgo->getTouch(x, y)) {} // Wait for touch
    while (ttgo->getTouch(x, y)) {}  // Wait for release to exit
    //Clear screen
    ttgo->tft->fillScreen(TFT_BLACK);
}


void setup()
{
    //initSetup();
    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->tft->setTextFont(1);
    ttgo->tft->fillScreen(TFT_BLACK);
    ttgo->tft->setTextColor(TFT_YELLOW, TFT_BLACK); // Note: the new fonts do not draw the background colour
    //LVGL is not used, this line is not needed
    // ttgo->lvgl_begin();

    //Check if the RTC clock matches, if not, use compile time
    ttgo->rtc->check();

    //Synchronize time to system time
    ttgo->rtc->syncToSystem();

    displayTime(true); // Our GUI to show the time
    ttgo->openBL(); // Turn on the backlight

}

void loop()
{

    if (targetTime < millis()) {
        targetTime = millis() + 1000;
        displayTime(ss == 0); // Call every second but only update time every minute
    }

    int16_t x, y;
    if (ttgo->getTouch(x, y)) {
        while (ttgo->getTouch(x, y)) {} // wait for user to release

        // This is where the app selected from the menu is launched
        // If you add an app, follow the variable update instructions
        // at the beginning of the menu code and then add a case
        // statement on to this switch to call your paticular
        // app routine.

        switch (modeMenu()) { // Call modeMenu. The return is the desired app number
        case 0: // Zero is the clock, just exit the switch
            break;
        case 1:
            jSats();
            break;
        case 2:
            appAccel();
            break;
        case 3:
            appBattery();
            break;
        case 4:
            appTouch();
            break;
        case 5:
            appSetTime();
            break;
        }
        displayTime(true);
    }
}
