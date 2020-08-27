// class GxGDEW029Z10 : Display class for GDEW029Z10 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=515.html
// Controller: IL0373 : http://www.good-display.com/download_detail/downloadsId=535.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEW029Z10.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// Partial Update Delay, may have an influence on degradation
#define GxGDEW029Z10_PU_DELAY 500

GxGDEW029Z10::GxGDEW029Z10(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW029Z10_WIDTH, GxGDEW029Z10_HEIGHT), IO(io), 
  _current_page(-1), _using_partial_mode(false), _diag_enabled(false),
  _rst(rst), _busy(busy)
{
}

void GxGDEW029Z10::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW029Z10_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW029Z10_WIDTH - x - 1;
      y = GxGDEW029Z10_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW029Z10_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW029Z10_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_black_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW029Z10_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW029Z10_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW029Z10_WIDTH / 8;
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


void GxGDEW029Z10::init(uint32_t serial_diag_bitrate)
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

void GxGDEW029Z10::fillScreen(uint16_t color)
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

void GxGDEW029Z10::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
  {
    _writeData((i < sizeof(_black_buffer)) ? ~_black_buffer[i] : 0xFF);
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
  {
    _writeData((i < sizeof(_red_buffer)) ? ~_red_buffer[i] : 0xFF);
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("update");
  _sleep();
}

void  GxGDEW029Z10::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_invert;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW029Z10::drawExamplePicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size)
{
  drawPicture(black_bitmap, red_bitmap, black_size, red_size, bm_invert_red);
}

void GxGDEW029Z10::drawPicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size, int16_t mode)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
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
    _writeData(data);
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
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
    _writeData(data);
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPicture");
  _sleep();
}

void GxGDEW029Z10::drawBitmap(const uint8_t* bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  // example bitmaps are normal on b/w, but inverted on red
  if (mode & bm_default) mode |= bm_normal;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true;
    _wakeUp();
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW029Z10_WIDTH - 1, GxGDEW029Z10_HEIGHT - 1);
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
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
      _writeData(data);
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawBitmap");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false;
    _wakeUp();
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
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
      _writeData(data);
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawBitmap");
    _sleep();
  }
}

void GxGDEW029Z10::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    if (!_using_partial_mode) _wakeUp();
    _using_partial_mode = true; // remember
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW029Z10_WIDTH - 1, GxGDEW029Z10_HEIGHT - 1);
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE * 2; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("eraseDisplay");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE * 2; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // white is 0xFF on device
    }
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW029Z10::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (_current_page != -1) return;
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(x, y);
        swap(w, h);
        x = GxGDEW029Z10_WIDTH - x - w - 1;
        break;
      case 2:
        x = GxGDEW029Z10_WIDTH - x - w - 1;
        y = GxGDEW029Z10_HEIGHT - y - h - 1;
        break;
      case 3:
        swap(x, y);
        swap(w, h);
        y = GxGDEW029Z10_HEIGHT - y  - h - 1;
        break;
    }
  }
  if (x >= GxGDEW029Z10_WIDTH) return;
  if (y >= GxGDEW029Z10_HEIGHT) return;
  // x &= 0xFFF8; // byte boundary, not here, use encompassing rectangle
  uint16_t xe = gx_uint16_min(GxGDEW029Z10_WIDTH, x + w) - 1;
  uint16_t ye = gx_uint16_min(GxGDEW029Z10_HEIGHT, y + h) - 1;
  // x &= 0xFFF8; // byte boundary, not needed here
  uint16_t xs_bx = x / 8;
  uint16_t xe_bx = (xe + 7) / 8;
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(x, y, xe, ye);
  IO.writeCommandTransaction(0x10);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00; // white is 0x00 in buffer
      IO.writeDataTransaction(~data); // white is 0xFF on device
    }
  }
  IO.writeCommandTransaction(0x13);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00; // white is 0x00 in buffer
      IO.writeDataTransaction(~data); // white is 0xFF on device
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateWindow");
  IO.writeCommandTransaction(0x92); // partial out
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _writeToWindow(xs, ys, xd, yd, w, h, using_rotation);
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateToWindow");
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::_writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW029Z10_WIDTH - xs - w - 1;
        xd = GxGDEW029Z10_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW029Z10_WIDTH - xs - w - 1;
        ys = GxGDEW029Z10_HEIGHT - ys - h - 1;
        xd = GxGDEW029Z10_WIDTH - xd - w - 1;
        yd = GxGDEW029Z10_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW029Z10_HEIGHT - ys  - h - 1;
        yd = GxGDEW029Z10_HEIGHT - yd  - h - 1;
        break;
    }
  }
  if (xs >= GxGDEW029Z10_WIDTH) return;
  if (ys >= GxGDEW029Z10_HEIGHT) return;
  if (xd >= GxGDEW029Z10_WIDTH) return;
  if (yd >= GxGDEW029Z10_HEIGHT) return;
  // the screen limits are the hard limits
  uint16_t xde = gx_uint16_min(GxGDEW029Z10_WIDTH, xd + w) - 1;
  uint16_t yde = gx_uint16_min(GxGDEW029Z10_HEIGHT, yd + h) - 1;
  IO.writeCommandTransaction(0x91); // partial in
  // soft limits, must send as many bytes as set by _SetRamArea
  uint16_t yse = ys + yde - yd;
  uint16_t xss_d8 = xs / 8;
  uint16_t xse_d8 = xss_d8 + _setPartialRamArea(xd, yd, xde, yde);
  IO.writeCommandTransaction(0x10);
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xss_d8; x1 < xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00; // white is 0x00 in buffer
      IO.writeDataTransaction(~data); // white is 0xFF on device
    }
  }
  IO.writeCommandTransaction(0x13);
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xss_d8; x1 < xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00; // white is 0x00 in buffer
      IO.writeDataTransaction(~data); // white is 0xFF on device
    }
  }
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW029Z10::powerDown()
{
  _using_partial_mode = false; // force _wakeUp()
  _sleep();
}

uint16_t GxGDEW029Z10::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye)
{
  x &= 0xFFF8; // byte boundary
  xe = (xe - 1) | 0x0007; // byte boundary - 1
  IO.writeCommandTransaction(0x90); // partial window
  //IO.writeDataTransaction(x / 256);
  IO.writeDataTransaction(x % 256);
  //IO.writeDataTransaction(xe / 256);
  IO.writeDataTransaction(xe % 256);
  IO.writeDataTransaction(y / 256);
  IO.writeDataTransaction(y % 256);
  IO.writeDataTransaction(ye / 256);
  IO.writeDataTransaction(ye % 256);
  IO.writeDataTransaction(0x01); // don't see any difference
  //IO.writeDataTransaction(0x00); // don't see any difference
  return (7 + xe - x) / 8; // number of bytes to transfer per line
}

void GxGDEW029Z10::_writeCommand(uint8_t command)
{
  IO.writeCommandTransaction(command);
}

void GxGDEW029Z10::_writeData(uint8_t data)
{
  IO.writeDataTransaction(data);
}

void GxGDEW029Z10::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  {
    if (digitalRead(_busy) == 1) break;
    delay(1);
    if (micros() - start > 20000000) // >14.9s !
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

void GxGDEW029Z10::_wakeUp()
{
  // reset required for wakeup
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }

  _writeCommand(0x06);
  _writeData (0x17);
  _writeData (0x17);
  _writeData (0x17);
  _writeCommand(0x04);
  _waitWhileBusy("_wakeUp Power On");
  _writeCommand(0X00);
  _writeData(0x8f);
  _writeCommand(0X50);
  _writeData(0x77);
  _writeCommand(0x61);
  _writeData (0x80);
  _writeData (0x01);
  _writeData (0x28);
}

void GxGDEW029Z10::_sleep(void)
{
  _writeCommand(0x02);      //power off
  _waitWhileBusy("_sleep Power Off");
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData (0xa5);
  }
}

void GxGDEW029Z10::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW029Z10::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW029Z10::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW029Z10::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW029Z10_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW029Z10_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW029Z10_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data); // white is 0xFF on device
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW029Z10::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW029Z10_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW029Z10_WIDTH - x - w - 1;
      y = GxGDEW029Z10_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW029Z10_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW029Z10::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW029Z10_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW029Z10_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW029Z10_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW029Z10_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW029Z10_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW029Z10_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW029Z10_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW029Z10_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW029Z10_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW029Z10_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW029Z10_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW029Z10_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW029Z10_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW029Z10_PU_DELAY); // don't stress this display
}

void GxGDEW029Z10::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t y = 0; y < GxGDEW029Z10_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW029Z10_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF; // white is 0xFF on device
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW029Z10_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW029Z10_WIDTH / 8 - 4) && (y > GxGDEW029Z10_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW029Z10_HEIGHT - 33)) data = 0x00;
      _writeData(data);
    }
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW029Z10_BUFFER_SIZE; i++)
  {
    _writeData(0xFF); // white is 0xFF on device
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}

