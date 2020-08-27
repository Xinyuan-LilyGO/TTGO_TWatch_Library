// class GxGDEW042Z15 : Display class for GDEW042Z15 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=515.html
// Controller: IL0398 : http://www.good-display.com/download_detail/downloadsId=537.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEW042Z15.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

// partial update produces distortion on the right half of the screen in b/w/r mode (on my display)

// this workaround updates the whole screen
#define USE_PARTIAL_UPDATE_WORKAROUND

#define GxGDEW042Z15_BUSY_TIMEOUT 20000000

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

GxGDEW042Z15::GxGDEW042Z15(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW042Z15_WIDTH, GxGDEW042Z15_HEIGHT), IO(io),
    _current_page(-1), _using_partial_mode(false), _diag_enabled(false),
    _rst(rst), _busy(busy)
{
}

void GxGDEW042Z15::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW042Z15_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW042Z15_WIDTH - x - 1;
      y = GxGDEW042Z15_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW042Z15_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW042Z15_WIDTH / 8;
  if (_current_page < 0)
  {
    if (i >= sizeof(_black_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW042Z15_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW042Z15_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW042Z15_WIDTH / 8;
  }

  _black_buffer[i] = (_black_buffer[i] & (0xFF ^ (1 << (7 - x % 8)))); // white
  _red_buffer[i] = (_red_buffer[i] & (0xFF ^ (1 << (7 - x % 8)))); // white
  if (color == GxEPD_WHITE) return;
  else if (color == GxEPD_BLACK) _black_buffer[i] = (_black_buffer[i] | (1 << (7 - x % 8)));
  else if (color == GxEPD_RED) _red_buffer[i] = (_red_buffer[i] | (1 << (7 - x % 8)));
  else
  {
    if ((color & 0xF100) > (0xF100 / 2)) _red_buffer[i] = (_red_buffer[i] | (1 << (7 - x % 8)));
    else if ((((color & 0xF100) >> 11) + ((color & 0x07E0) >> 5) + (color & 0x001F)) < 3 * 255 / 2)
    {
      _black_buffer[i] = (_black_buffer[i] | (1 << (7 - x % 8)));
    }
  }
}

void GxGDEW042Z15::init(uint32_t serial_diag_bitrate)
{
  if (serial_diag_bitrate > 0)
  {
    Serial.begin(serial_diag_bitrate);
    _diag_enabled = true;
  }
  IO.init();
  IO.setFrequency(4000000); // 4MHz
  if (_rst >= 0)
  {
    digitalWrite(_rst, HIGH);
    pinMode(_rst, OUTPUT);
  }
  pinMode(_busy, INPUT);
  fillScreen(GxEPD_WHITE);
  _current_page = -1;
  _using_partial_mode = false;
}

void GxGDEW042Z15::fillScreen(uint16_t color)
{
  uint8_t black = 0x00;
  uint8_t red = 0x00;
  if (color == GxEPD_WHITE);
  else if (color == GxEPD_BLACK) black = 0xFF;
  else if (color == GxEPD_RED) red = 0xFF;
  else if ((color & 0xF100) > (0xF100 / 2))  red = 0xFF;
  else if ((((color & 0xF100) >> 11) + ((color & 0x07E0) >> 5) + (color & 0x001F)) < 3 * 255 / 2) black = 0xFF;
  for (uint16_t x = 0; x < sizeof(_black_buffer); x++)
  {
    _black_buffer[x] = black;
    _red_buffer[x] = red;
  }
}

void GxGDEW042Z15::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
  {
    uint8_t data = i < sizeof(_black_buffer) ? _black_buffer[i] : 0x00;
    IO.writeDataTransaction(~data);
  }
  IO.writeCommandTransaction(0x13); // red
  for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
  {
    uint8_t data = i < sizeof(_red_buffer) ? _red_buffer[i] : 0x00;
    IO.writeDataTransaction(~data);
  }
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("update");
  _sleep();
}

void  GxGDEW042Z15::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_invert;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW042Z15::drawExamplePicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size)
{
  drawPicture(black_bitmap, red_bitmap, black_size, red_size, bm_normal);
}

void GxGDEW042Z15::drawPicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    // set full screen
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042Z15_WIDTH - 1, GxGDEW042Z15_HEIGHT - 1);
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // 0xFF is white
      if (i < black_size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&black_bitmap[i]);
#else
        data = black_bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x13); // red
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // 0xFF is white
      if (i < red_size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&red_bitmap[i]);
#else
        data = red_bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x12); //display refresh
    _waitWhileBusy("drawPicture");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false;
    _wakeUp();
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // white is 0xFF on device
      if (i < black_size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&black_bitmap[i]);
#else
        data = black_bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x13); // red
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // white is 0xFF on device
      if (i < red_size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&red_bitmap[i]);
#else
        data = red_bitmap[i];
#endif
        if (mode & bm_invert_red) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x12); //display refresh
    _waitWhileBusy("drawPicture");
    _sleep();
  }
}

void GxGDEW042Z15::drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_default) mode |= bm_normal;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    // set full screen
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042Z15_WIDTH - 1, GxGDEW042Z15_HEIGHT - 1);
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // white is 0xFF on device
      if (i < size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[i]);
#else
        data = bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x13); // red
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x92); // partial out
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      uint8_t data = 0xFF; // white is 0xFF on device
      if (i < size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[i]);
#else
        data = bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      IO.writeDataTransaction(data);
    }
    IO.writeCommandTransaction(0x13); // red
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x12); //display refresh
    _waitWhileBusy("drawBitmap");
    _sleep();
  }
}

void GxGDEW042Z15::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    // set full screen
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042Z15_WIDTH - 1, GxGDEW042Z15_HEIGHT - 1);
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x12); //display refresh
    _waitWhileBusy("eraseDisplay");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x10); // black
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x13); // red
    for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF); // 0xFF is white
    }
    IO.writeCommandTransaction(0x12); //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW042Z15::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (_current_page != -1) return;
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(x, y);
        swap(w, h);
        x = GxGDEW042Z15_WIDTH - x - w - 1;
        break;
      case 2:
        x = GxGDEW042Z15_WIDTH - x - w - 1;
        y = GxGDEW042Z15_HEIGHT - y - h - 1;
        break;
      case 3:
        swap(x, y);
        swap(w, h);
        y = GxGDEW042Z15_HEIGHT - y  - h - 1;
        break;
    }
  }
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  _writeToWindow(x, y, x, y, w, h);
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateWindow");
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW042Z15::_writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h)
{
  //Serial.printf("_writeToWindow(%d, %d, %d, %d, %d, %d)\n", xs, ys, xd, yd, w, h);
  // the screen limits are the hard limits
  if (xs >= GxGDEW042Z15_WIDTH) return;
  if (ys >= GxGDEW042Z15_HEIGHT) return;
  if (xd >= GxGDEW042Z15_WIDTH) return;
  if (yd >= GxGDEW042Z15_HEIGHT) return;
  uint16_t xde = gx_uint16_min(GxGDEW042Z15_WIDTH, xd + w) - 1;
  uint16_t yde = gx_uint16_min(GxGDEW042Z15_HEIGHT, yd + h) - 1;
  // soft limits, must send as many bytes as set by _setPartialRamArea
  uint16_t yse = ys + yde - yd; // inclusive
  uint16_t xss_d8 = xs / 8;
  uint16_t xse_d8 = xss_d8 + _setPartialRamArea(xd, yd, xde, yde); // exclusive
  IO.writeCommandTransaction(0x10); // black
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xss_d8; x1 < xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
  delay(2);
  //_setPartialRamArea(xd, yd, xde, yde);
  IO.writeCommandTransaction(0x13); // red
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xss_d8; x1 < xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
#ifdef USE_PARTIAL_UPDATE_WORKAROUND
  _setPartialRamArea(0, 0, GxGDEW042Z15_WIDTH - 1, GxGDEW042Z15_HEIGHT - 1);
#endif
}

void GxGDEW042Z15::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW042Z15_WIDTH - xs - w - 1;
        xd = GxGDEW042Z15_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW042Z15_WIDTH - xs - w - 1;
        ys = GxGDEW042Z15_HEIGHT - ys - h - 1;
        xd = GxGDEW042Z15_WIDTH - xd - w - 1;
        yd = GxGDEW042Z15_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW042Z15_HEIGHT - ys  - h - 1;
        yd = GxGDEW042Z15_HEIGHT - yd  - h - 1;
        break;
    }
  }
  _wakeUp();
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  _writeToWindow(xs, ys, xd, yd, w, h);
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateToWindow");
  IO.writeCommandTransaction(0x92); // partial out
  delay(1000); // don't stress this display
}

void GxGDEW042Z15::powerDown()
{
  _sleep();
}

uint16_t GxGDEW042Z15::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye)
{
  x &= 0xFFF8; // byte boundary
  xe |= 0x0007; // byte boundary
  IO.writeCommandTransaction(0x90); // partial window
  IO.writeDataTransaction(x / 256);
  IO.writeDataTransaction(x % 256);
  IO.writeDataTransaction(xe / 256);
  IO.writeDataTransaction(xe % 256);
  IO.writeDataTransaction(y / 256);
  IO.writeDataTransaction(y % 256);
  IO.writeDataTransaction(ye / 256);
  IO.writeDataTransaction(ye % 256);
  //IO.writeDataTransaction(0x01); // distortion on full right half
  IO.writeDataTransaction(0x00); // distortion on right half
  uint16_t xb = x / 8; // first byte (containing first bit)
  uint16_t xeb = xe / 8; // last byte (containing last bit)
  return xeb - xb + 1; // number of bytes to transfer
}

void GxGDEW042Z15::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  { //=0 BUSY
    if (digitalRead(_busy) == 1) break;
    delay(1);
    if (micros() - start > GxGDEW042Z15_BUSY_TIMEOUT)
    {
      if (_diag_enabled) Serial.println("Busy Timeout!");
      break;
    }
  }
  if (comment)
  {
#if !defined(DISABLE_DIAGNOSTIC_OUTPUT)
    if (_diag_enabled)
    {
      unsigned long elapsed = micros() - start;
      Serial.print(comment);
      Serial.print(" : ");
      Serial.println(elapsed);
    }
#endif
  }
  (void) start;
}

void GxGDEW042Z15::_wakeUp(void)
{
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }
  IO.writeCommandTransaction(0x06); //boost
  IO.writeDataTransaction (0x17);
  IO.writeDataTransaction (0x17);
  IO.writeDataTransaction (0x17);
  IO.writeCommandTransaction(0x04);
  _waitWhileBusy("Power On");
  IO.writeCommandTransaction(0x00);
  IO.writeDataTransaction(0x0f); // LUT from OTP Pixel with B/W/R.
}

void GxGDEW042Z15::_sleep(void)
{
  IO.writeCommandTransaction(0x50); // border floating
  IO.writeDataTransaction(0x17);
  IO.writeCommandTransaction(0x02); // power off
  _waitWhileBusy("Power Off");
  if (_rst >= 0)
  {
    IO.writeCommandTransaction(0x07); // deep sleep
    IO.writeDataTransaction(0xA5);
  }
}

void GxGDEW042Z15::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  IO.writeCommandTransaction(0x13); // red
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042Z15::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  IO.writeCommandTransaction(0x13); // red
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042Z15::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  IO.writeCommandTransaction(0x13); // red
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042Z15::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  IO.writeCommandTransaction(0x13); // red
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW042Z15_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042Z15_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042Z15_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042Z15::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW042Z15_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW042Z15_WIDTH - x - w - 1;
      y = GxGDEW042Z15_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW042Z15_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW042Z15::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042Z15_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042Z15_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW042Z15_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateToWindow");
  IO.writeCommandTransaction(0x92); // partial out
  _current_page = -1;
}

void GxGDEW042Z15::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042Z15_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042Z15_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW042Z15_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateToWindow");
  IO.writeCommandTransaction(0x92); // partial out
  _current_page = -1;
}

void GxGDEW042Z15::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042Z15_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042Z15_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW042Z15_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateToWindow");
  IO.writeCommandTransaction(0x92); // partial out
  _current_page = -1;
}

void GxGDEW042Z15::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  for (_current_page = 0; _current_page < GxGDEW042Z15_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042Z15_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042Z15_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW042Z15_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateToWindow");
  IO.writeCommandTransaction(0x92); // partial out
  _current_page = -1;
}

void GxGDEW042Z15::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10); // black
  for (uint32_t y = 0; y < GxGDEW042Z15_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW042Z15_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW042Z15_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW042Z15_WIDTH / 8 - 4) && (y > GxGDEW042Z15_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW042Z15_HEIGHT - 33)) data = 0x00;
      IO.writeDataTransaction(data);
    }
  }
  IO.writeCommandTransaction(0x13); // red
  for (uint32_t i = 0; i < GxGDEW042Z15_BUFFER_SIZE; i++)
  {
    IO.writeDataTransaction(0xFF); // 0xFF is white
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}

