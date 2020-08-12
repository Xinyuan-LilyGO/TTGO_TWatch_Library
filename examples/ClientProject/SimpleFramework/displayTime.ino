// The basic Time Display GUI
// if you are just updating the colon, fullUpdate =0
// if you want to update the complete display, fullUpdate =1
// This helps reduce flicker

#include <time.h>

byte xcolon = 0; // location of the colon

void displayTime(boolean fullUpdate) {

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
