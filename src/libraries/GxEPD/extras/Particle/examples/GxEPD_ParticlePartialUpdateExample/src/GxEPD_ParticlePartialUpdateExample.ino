// PartialUpdateExample : example for Waveshare 1.54", 2.31" and 2.9" e-Paper and the same e-papers from Dalian Good Display Inc.
//
// Created by Jean-Marc Zingg based on demo code from Good Display for GDEP015OC1.
//
// The e-paper displays are available from:
//
// https://www.aliexpress.com/store/product/Wholesale-1-54inch-E-Ink-display-module-with-embedded-controller-200x200-Communicate-via-SPI-interface-Supports/216233_32824535312.html
//
// http://www.buy-lcd.com/index.php?route=product/product&path=2897_8363&product_id=35120
// or https://www.aliexpress.com/store/product/E001-1-54-inch-partial-refresh-Small-size-dot-matrix-e-paper-display/600281_32815089163.html
//
#include <Adafruit_GFX_RK.h>
#include <GxEPD.h>

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// mapping suggestion from Waveshare SPI e-Paper to Particle Photon
// A5 MOSI
// A4 MISO
// A3 SCK
// A2 SS
// BUSY -> D4, RST -> A0, DC -> A1, CS -> A2, CLK -> A3, DIN -> A5, GND -> GND, 3.3V -> 3.3V
// NOTE: it looks like MISO can't be used as general input pin for BUSY.

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// select the display class to use, only one
//#include <GxGDEP015OC1/GxGDEP015OC1.h>    // 1.54" b/w
//#include <GxGDEW0213I5F/GxGDEW0213I5F.h>  // 2.13" b/w 104x212 flexible
//#include <GxGDE0213B1/GxGDE0213B1.h>      // 2.13" b/w
#include <GxGDEH0213B72/GxGDEH0213B72.h>  // 2.13" b/w new panel
//#include <GxGDEH029A1/GxGDEH029A1.h>      // 2.9" b/w
//#include <GxGDEW029T5/GxGDEW029T5.h>      // 2.9" b/w IL0373
//#include <GxGDEW027W3/GxGDEW027W3.h>      // 2.7" b/w
//#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
// these displays do not fully support partial update
//#include <GxGDEW0154Z17/GxGDEW0154Z17.h>  // 1.54" b/w/r 152x152
//#include <GxGDEW0213Z16/GxGDEW0213Z16.h>  // 2.13" b/w/r
//#include <GxGDEW029Z10/GxGDEW029Z10.h>    // 2.9" b/w/r
//#include <GxGDEW027C44/GxGDEW027C44.h>    // 2.7" b/w/r
//#include <GxGDEW042Z15/GxGDEW042Z15.h>    // 4.2" b/w/r
//#include <GxGDEW0583T7/GxGDEW0583T7.h>    // 5.83" b/w
//#include <GxGDEW075T8/GxGDEW075T8.h>      // 7.5" b/w
//#include <GxGDEW075Z09/GxGDEW075Z09.h>    // 7.5" b/w/r

// FreeFonts from Adafruit_GFX
#include <FreeMonoBold9pt7b.h>

#include GxEPD_BitmapExamples

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ A1, /*RST=*/ A0);
GxEPD_Class display(io, /*RST=*/ A0, /*BUSY=*/ D4);

//#define DEMO_DELAY 3*60 // seconds
//#define DEMO_DELAY 1*60 // seconds
#define DEMO_DELAY 30

void setup(void)
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  display.init(115200); // enable diagnostic output on Serial
  Serial.println("setup done");
}

void loop()
{
#if defined(__AVR) || false
  showPartialUpdatePaged();
#elif defined(_GxGDEW075Z09_H_) && (defined(ESP8266) || defined(ARDUINO_ARCH_STM32F1))
  showPartialUpdatePaged();
#elif defined(_GxGDEW075Z09_H_)
  showPartialUpdate_75Z09();
#else
  showPartialUpdate();
#endif
  delay(DEMO_DELAY * 1000);
}

#if !defined(__AVR)

void showPartialUpdate()
{
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  float value = 13.95;
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(0);
  // draw background
  display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.update();
  delay(2000);

  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);

  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
    delay(1000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    // reset the background
    display.setRotation(0);
    display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
    display.setRotation(r);
    for (uint16_t i = 1; i <= 10; i++)
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
      display.setCursor(box_x, cursor_y);
      display.print(value * i, 2);
      display.updateWindow(box_x, box_y, box_w, box_h, true);
      delay(2000);
    }
    delay(2000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // should have checked this, too
  box_x = GxEPD_HEIGHT - box_x - box_w - 1; // not valid for all corners
  // should show on right side of long side
  // reset the background
  display.setRotation(0);
  display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
  display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
    delay(1000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    // reset the background
    display.setRotation(0);
    display.drawExampleBitmap(BitmapExample1, 0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, GxEPD_BLACK);
    display.updateWindow(0, 0, GxEPD_WIDTH, GxEPD_HEIGHT, false);
    display.setRotation(r);
    if (box_x >= display.width()) continue; // avoid delay
    for (uint16_t i = 1; i <= 10; i++)
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
      display.setCursor(box_x, cursor_y);
      display.print(value * i, 2);
      display.updateWindow(box_x, box_y, box_w, box_h, true);
      delay(2000);
    }
    delay(2000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  display.setRotation(0);
  display.powerDown();
}

#endif

#if defined(_GxGDEW075Z09_H_) && !(defined(ESP8266) || defined(ARDUINO_ARCH_STM32F1))

void showPartialUpdate_75Z09()
{
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  float value = 13.95;
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(0);
  // draw background, partial update to full screen to preset for partial update of box window
  // (updateWindow() would clear display if partial update not set, to avoid strange background effect)
  display.drawExamplePicture_3C(BitmapPicture_3C, sizeof(BitmapPicture_3C), GxEPD::bm_partial_update);
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
    delay(1000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    // reset the background
    display.setRotation(0);
    display.drawExamplePicture_3C(BitmapPicture_3C, sizeof(BitmapPicture_3C), GxEPD::bm_partial_update);
    display.setRotation(r);
    for (uint16_t i = 1; i <= 10; i++)
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
      display.setCursor(box_x, cursor_y);
      display.print(value * i, 2);
      display.updateWindow(box_x, box_y, box_w, box_h, true);
      delay(2000);
    }
    delay(2000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // should have checked this, too
  box_x = GxEPD_HEIGHT - box_x - box_w - 1; // not valid for all corners
  // should show on right side of long side
  // reset the background
  display.setRotation(0);
  display.drawExamplePicture_3C(BitmapPicture_3C, sizeof(BitmapPicture_3C), GxEPD::bm_partial_update);
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
    delay(1000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    // reset the background
    display.setRotation(0);
    display.drawExamplePicture_3C(BitmapPicture_3C, sizeof(BitmapPicture_3C), GxEPD::bm_partial_update);
    display.setRotation(r);
    if (box_x >= display.width()) continue; // avoid delay
    for (uint16_t i = 1; i <= 10; i++)
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
      display.setCursor(box_x, cursor_y);
      display.print(value * i, 2);
      display.updateWindow(box_x, box_y, box_w, box_h, true);
      delay(2000);
    }
    delay(2000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.updateWindow(box_x, box_y, box_w, box_h, true);
  }
  display.setRotation(0);
  display.powerDown();
}

#endif

#if defined(__AVR) || defined(_GxGDEW075Z09_H_) && (defined(ESP8266) || defined(ARDUINO_ARCH_STM32F1)) || false

// modified to avoid float; reduces program size ~2k (for GxGDEW042T2)

void showBlackBoxCallback(uint32_t v)
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  display.fillRect(box_x, box_y, box_w, box_h, v);
}

void showValueBoxCallback(const uint32_t v)
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
  display.setCursor(box_x, cursor_y);
  display.print(v / 100);
  display.print(v % 100 > 9 ? "." : ".0");
  display.print(v % 100);
}

void showPartialUpdatePaged()
{
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  uint32_t value = 1395;
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setRotation(0);
  // draw background
  display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1));
  delay(2000);

  // partial update to full screen to preset for partial update of box window
  // (this avoids strange background effects)
  display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
  delay(1000);

  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.drawPagedToWindow(showBlackBoxCallback, box_x, box_y, box_w, box_h, GxEPD_BLACK);
    delay(1000);
    display.drawPagedToWindow(showBlackBoxCallback, box_x, box_y, box_w, box_h, GxEPD_WHITE);
  }

  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    // reset the background
    display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
    display.setRotation(r);
    for (uint16_t i = 1; i <= 10; i++)
    {
      uint32_t v = value * i;
      display.drawPagedToWindow(showValueBoxCallback, box_x, box_y, box_w, box_h, v);
      delay(500);
    }
    delay(1000);
    display.drawPagedToWindow(showBlackBoxCallback, box_x, box_y, box_w, box_h, GxEPD_WHITE);
  }
  // reset the background
  display.drawExampleBitmap(BitmapExample1, sizeof(BitmapExample1), GxEPD::bm_default | GxEPD::bm_partial_update);
  display.setRotation(0);
  display.powerDown();
}

#endif
