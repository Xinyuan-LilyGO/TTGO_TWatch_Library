// Show the accelerometer working

void appAccel() {
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
