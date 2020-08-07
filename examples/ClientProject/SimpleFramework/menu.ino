// The basic menu code
// If you add an app, this is where you will update the
// framework code to include it in the menu.
//
// Make the following updates:
// 1) Update maxApp to the total number of apps.
// 2) Update appName to add the title of the app.
// 3) in the main routine in TWatch_framework, add a case to the switch statement to call your app routine.


const int maxApp = 6; // number of apps
String appName[maxApp] = {"Clock", "Jupiter", "Accel", "Battery", "Touch", "Set Time"}; // app names

uint8_t modeMenu() {
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

void setMenuDisplay(int mSel) {

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
