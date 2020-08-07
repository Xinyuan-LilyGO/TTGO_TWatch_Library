// Check out the touch screen
// Will display the x and y coordinates of where you touch
// for 10 seconds and then return to time

void appTouch() {
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
