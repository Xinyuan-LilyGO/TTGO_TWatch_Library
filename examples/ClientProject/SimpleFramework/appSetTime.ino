// Set the time - no error checking, you might want to add it

void appSetTime() {
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
    if (wl != -1) {

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
  }
  while (getTnum() != -1) {}
  ttgo->rtc->setDateTime(yyear, mmonth, dday, hh, mm, 0);
  ttgo->tft->fillScreen(TFT_BLACK);
}

// prtTime will display the current selected time and highlight
// the current digit to be updated in yellow

void prtTime(byte digit) {
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
  ttgo->tft->drawNumber(int(mm / 10), 65 , 5, 2);
  if (digit == 4)   ttgo->tft->setTextColor(TFT_YELLOW);
  else   ttgo->tft->setTextColor(TFT_WHITE);
  ttgo->tft->drawNumber(mm % 10, 85, 5, 2);
}

// getTnum takes care of translating where we pressed into
// a number that was pressed. Returns -1 for no press
// and 13 for DONE

int getTnum() {
  int16_t x, y;
  if (!ttgo->getTouch(x, y)) return - 1;
  if (y < 85) {
    if (x < 80) return 1;
    else if (x > 160) return 3;
    else return 2;
  }
  else if (y < 135) {
    if (x < 80) return 4;
    else if (x > 160) return 6;
    else return 5;
  }
  else if (y < 185) {
    if (x < 80) return 7;
    else if (x > 160) return 9;
    else return 8;
  }
  else if (x < 80) return 0;
  else return 13;
}
