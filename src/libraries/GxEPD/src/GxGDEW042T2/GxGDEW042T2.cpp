// class GxGDEW042T2 : Display class for GDEW042T2 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
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

#include "GxGDEW042T2.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#define GxGDEW042T2_BUSY_TIMEOUT 10000000

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

GxGDEW042T2::GxGDEW042T2(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW042T2_WIDTH, GxGDEW042T2_HEIGHT), IO(io),
    _current_page(-1), _initial(true), _using_partial_mode(false), _diag_enabled(false),
    _rst(rst), _busy(busy)
{
}

void GxGDEW042T2::drawPixel(int16_t x, int16_t y, uint16_t color)
{

  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW042T2_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW042T2_WIDTH - x - 1;
      y = GxGDEW042T2_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW042T2_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW042T2_WIDTH / 8;
  if (_current_page < 0)
  {
    if (i >= sizeof(_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW042T2_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW042T2_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW042T2_WIDTH / 8;
  }

  if (!color)
    _buffer[i] = (_buffer[i] | (1 << (7 - x % 8)));
  else
    _buffer[i] = (_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
}

void GxGDEW042T2::init(uint32_t serial_diag_bitrate)
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
  _initial = true;
  _current_page = -1;
  _using_partial_mode = false;
}

void GxGDEW042T2::fillScreen(uint16_t color)
{
  uint8_t data = (color == GxEPD_BLACK) ? 0xFF : 0x00;
  for (uint16_t x = 0; x < sizeof(_buffer); x++)
  {
    _buffer[x] = data;
  }
}

void GxGDEW042T2::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
  {
    uint8_t data = i < sizeof(_buffer) ? _buffer[i] : 0x00;
    IO.writeDataTransaction(~data);
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("update");
#if 0
  if (_initial)
  {
    _initial = false;
    // use second full refresh to init second controller buffer
    // needed for subsequent partial updates
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
    {
      uint8_t data = i < sizeof(_buffer) ? _buffer[i] : 0x00;
      IO.writeDataTransaction(~data);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("update");
  }
#else
  // avoid double full refresh after deep sleep wakeup
  if (_initial)
  {
    _initial = false;
    // use full screen partial refresh to init second controller buffer
    // needed for subsequent partial updates
    _Init_PartialUpdate();
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, WIDTH, HEIGHT);
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
    {
      uint8_t data = i < sizeof(_buffer) ? _buffer[i] : 0x00;
      IO.writeDataTransaction(~data);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("update");
    IO.writeCommandTransaction(0x92); // partial out
  }
#endif
  _sleep();
}

void  GxGDEW042T2::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_invert;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW042T2::drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_default) mode |= bm_normal;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    _Init_PartialUpdate();
    // set full screen
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042T2_WIDTH - 1, GxGDEW042T2_HEIGHT - 1);
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
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
    IO.writeCommandTransaction(0x92); // partial out
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    // update erase buffer
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042T2_WIDTH - 1, GxGDEW042T2_HEIGHT - 1);
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
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
    IO.writeCommandTransaction(0x92); // partial out
    _waitWhileBusy("drawBitmap");
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
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
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    if (_initial)
    {
      _initial = false;
      IO.writeCommandTransaction(0x13);
      for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
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
      IO.writeCommandTransaction(0x12);      //display refresh
      _waitWhileBusy("drawBitmap");
    }
    _sleep();
  }
}

void GxGDEW042T2::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    _Init_PartialUpdate();
    // set full screen
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW042T2_WIDTH - 1, GxGDEW042T2_HEIGHT - 1);
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF);
    }
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF);
    }
    IO.writeCommandTransaction(0x92); // partial out
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x13);
    for (uint32_t i = 0; i < GxGDEW042T2_BUFFER_SIZE; i++)
    {
      IO.writeDataTransaction(0xFF);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW042T2::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (_current_page != -1) return;
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(x, y);
        swap(w, h);
        x = GxGDEW042T2_WIDTH - x - w - 1;
        break;
      case 2:
        x = GxGDEW042T2_WIDTH - x - w - 1;
        y = GxGDEW042T2_HEIGHT - y - h - 1;
        break;
      case 3:
        swap(x, y);
        swap(w, h);
        y = GxGDEW042T2_HEIGHT - y  - h - 1;
        break;
    }
  }
  //fillScreen(0x0);
  if (x >= GxGDEW042T2_WIDTH) return;
  if (y >= GxGDEW042T2_HEIGHT) return;
  // x &= 0xFFF8; // byte boundary, not here, use encompassing rectangle
  uint16_t xe = gx_uint16_min(GxGDEW042T2_WIDTH, x + w) - 1;
  uint16_t ye = gx_uint16_min(GxGDEW042T2_HEIGHT, y + h) - 1;
  // x &= 0xFFF8; // byte boundary, not needed here
  uint16_t xs_bx = x / 8;
  uint16_t xe_bx = (xe + 7) / 8;
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _Init_PartialUpdate();
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(x, y, xe, ye);
  IO.writeCommandTransaction(0x13);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
  IO.writeCommandTransaction(0x92); // partial out
  IO.writeCommandTransaction(0x12); //display refresh
  _waitWhileBusy("updateWindow");
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(x, y, xe, ye);
  IO.writeCommandTransaction(0x13);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW042T2::_writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h)
{
  //Serial.printf("_writeToWindow(%d, %d, %d, %d, %d, %d)\n", xs, ys, xd, yd, w, h);
  // the screen limits are the hard limits
  if (xs >= GxGDEW042T2_WIDTH) return;
  if (ys >= GxGDEW042T2_HEIGHT) return;
  if (xd >= GxGDEW042T2_WIDTH) return;
  if (yd >= GxGDEW042T2_HEIGHT) return;
  uint16_t xde = gx_uint16_min(GxGDEW042T2_WIDTH, xd + w) - 1;
  uint16_t yde = gx_uint16_min(GxGDEW042T2_HEIGHT, yd + h) - 1;
  // soft limits, must send as many bytes as set by _SetRamArea
  uint16_t yse = ys + yde - yd;
  uint16_t xss_d8 = xs / 8;
  IO.writeCommandTransaction(0x91); // partial in
  uint16_t xse_d8 = xss_d8 + _setPartialRamArea(xd, yd, xde, yde);
  IO.writeCommandTransaction(0x13);
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xss_d8; x1 < xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
  delay(2);
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW042T2::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW042T2_WIDTH - xs - w - 1;
        xd = GxGDEW042T2_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW042T2_WIDTH - xs - w - 1;
        ys = GxGDEW042T2_HEIGHT - ys - h - 1;
        xd = GxGDEW042T2_WIDTH - xd - w - 1;
        yd = GxGDEW042T2_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW042T2_HEIGHT - ys  - h - 1;
        yd = GxGDEW042T2_HEIGHT - yd  - h - 1;
        break;
    }
  }
  _wakeUp();
  _using_partial_mode = true;
  _Init_PartialUpdate();
  _writeToWindow(xs, ys, xd, yd, w, h);
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateToWindow");
  delay(500); // don't stress this display
}

void GxGDEW042T2::powerDown()
{
  _sleep();
}

uint16_t GxGDEW042T2::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye)
{
  x &= 0xFFF8; // byte boundary
  xe = (xe - 1) | 0x0007; // byte boundary - 1
  IO.writeCommandTransaction(0x90); // partial window
  IO.writeDataTransaction(x / 256);
  IO.writeDataTransaction(x % 256);
  IO.writeDataTransaction(xe / 256);
  IO.writeDataTransaction(xe % 256);
  IO.writeDataTransaction(y / 256);
  IO.writeDataTransaction(y % 256);
  IO.writeDataTransaction(ye / 256);
  IO.writeDataTransaction(ye % 256);
  IO.writeDataTransaction(0x01); // don't see any difference
  //IO.writeDataTransaction(0x00); // don't see any difference
  return (7 + xe - x) / 8; // number of bytes to transfer
}

void GxGDEW042T2::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  { //=0 BUSY
    if (digitalRead(_busy) == 1) break;
    delay(1);
    if (micros() - start > GxGDEW042T2_BUSY_TIMEOUT)
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

void GxGDEW042T2::_wakeUp(void)
{
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }
  IO.writeCommandTransaction(0x01); // POWER SETTING
  IO.writeDataTransaction(0x03);   // VDS_EN, VDG_EN internal
  IO.writeDataTransaction(0x00);   // VCOM_HV, VGHL_LV=16V
  IO.writeDataTransaction(0x2b);   // VDH=11V
  IO.writeDataTransaction(0x2b);   // VDL=11V
  IO.writeCommandTransaction(0x06); // boost soft start
  IO.writeDataTransaction(0x17);   // A
  IO.writeDataTransaction(0x17);   // B
  IO.writeDataTransaction(0x17);   // C
  IO.writeCommandTransaction(0x00); // panel setting
  IO.writeDataTransaction(0x3f);    // 300x400 B/W mode, LUT set by register
  IO.writeCommandTransaction(0x30); // PLL setting
  IO.writeDataTransaction(0x3a);   // 3a 100HZ   29 150Hz 39 200HZ 31 171HZ
  IO.writeCommandTransaction(0x61); // resolution setting
  IO.writeDataTransaction(WIDTH / 256);
  IO.writeDataTransaction(WIDTH % 256);
  IO.writeDataTransaction(HEIGHT / 256);
  IO.writeDataTransaction(HEIGHT % 256);
  IO.writeCommandTransaction(0x82); // vcom_DC setting
  //IO.writeDataTransaction(0x08);   // -0.1 + 8 * -0.05 = -0.5V from demo
  IO.writeDataTransaction(0x12);   // -0.1 + 18 * -0.05 = -1.0V from OTP, slightly better
  //IO.writeDataTransaction(0x1c);   // -0.1 + 28 * -0.05 = -1.5V test, worse
  IO.writeCommandTransaction(0x50); // VCOM AND DATA INTERVAL SETTING
  //IO.writeDataTransaction(0x97);    // WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
  IO.writeDataTransaction(0xd7);    // border floating to avoid flashing
  IO.writeCommandTransaction(0x04);
  _waitWhileBusy("Power On");
  _Init_FullUpdate();
}

void GxGDEW042T2::_sleep(void)
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

void GxGDEW042T2::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW042T2_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042T2_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042T2::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042T2_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042T2_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042T2::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW042T2_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042T2_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042T2::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW042T2_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW042T2_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW042T2_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        IO.writeDataTransaction(~data);
      }
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW042T2::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW042T2_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW042T2_WIDTH - x - w - 1;
      y = GxGDEW042T2_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW042T2_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW042T2::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  delay(2);
  _waitWhileBusy("updateToWindow");
  // update erase buffer
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  delay(2);
  _waitWhileBusy("updateToWindow");
  _current_page = -1;
}

void GxGDEW042T2::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      //fillScreen(p);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  delay(2);
  _waitWhileBusy("updateToWindow");
  // update erase buffer
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      //fillScreen(p);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  delay(2);
  _waitWhileBusy("updateToWindow");
  _current_page = -1;
}

void GxGDEW042T2::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  delay(2);
  _waitWhileBusy("updateToWindow");
  // update erase buffer
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  delay(2);
  _waitWhileBusy("updateToWindow");
  _current_page = -1;
}

void GxGDEW042T2::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  IO.writeCommandTransaction(0x12); //display refresh
  delay(2);
  _waitWhileBusy("updateToWindow");
  // update erase buffer
  for (_current_page = 0; _current_page < GxGDEW042T2_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW042T2_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW042T2_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW042T2_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds);
    }
  }
  delay(2);
  _waitWhileBusy("updateToWindow");
  _current_page = -1;
}

void GxGDEW042T2::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x13);
  for (uint32_t y = 0; y < GxGDEW042T2_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW042T2_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW042T2_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW042T2_WIDTH / 8 - 4) && (y > GxGDEW042T2_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW042T2_HEIGHT - 33)) data = 0x00;
      IO.writeDataTransaction(data);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}

void GxGDEW042T2::_writeDataPGM(const uint8_t* data, uint16_t n, int16_t fill_with_zeroes)
{
  for (uint16_t i = 0; i < n; i++)
  {
    IO.writeDataTransaction(pgm_read_byte(&*data++));
  }
  while (fill_with_zeroes > 0)
  {
    IO.writeDataTransaction(0x00);
    fill_with_zeroes--;
  }
}

void GxGDEW042T2::_Init_FullUpdate()
{
  //IO.writeCommandTransaction(0x00);
  //IO.writeDataTransaction(0x1f); // LUT from OTP Pixel with B/W.
  IO.writeCommandTransaction(0x00);
  IO.writeDataTransaction(0x3F); //300x400 B/W mode, LUT set by register
  unsigned int count;
  IO.writeCommandTransaction(0x20); //vcom
  for (count = 0; count < 44; count++)
  {
    IO.writeDataTransaction(lut_vcom0_full[count]);
  }
  IO.writeCommandTransaction(0x21); //ww --
  for (count = 0; count < 42; count++)
  {
    IO.writeDataTransaction(lut_ww_full[count]);
  }
  IO.writeCommandTransaction(0x22); //bw r
  for (count = 0; count < 42; count++)
  {
    IO.writeDataTransaction(lut_bw_full[count]);
  }
  IO.writeCommandTransaction(0x23); //wb w
  for (count = 0; count < 42; count++)
  {
    IO.writeDataTransaction(lut_wb_full[count]);
  }
  IO.writeCommandTransaction(0x24); //bb b
  for (count = 0; count < 42; count++)
  {
    IO.writeDataTransaction(lut_bb_full[count]);
  }
}

const unsigned char GxGDEW042T2::lut_vcom0_full[] =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x00, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxGDEW042T2::lut_ww_full[] =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxGDEW042T2::lut_bw_full[] =
{
  0x40, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x40, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0xA0, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxGDEW042T2::lut_wb_full[] =
{
  0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

const unsigned char GxGDEW042T2::lut_bb_full[] =
{
  0x80, 0x17, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x17, 0x17, 0x00, 0x00, 0x02,
  0x80, 0x0A, 0x01, 0x00, 0x00, 0x01,
  0x50, 0x0E, 0x0E, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// partial update waveform

// same waveform as by demo code from Good Display
//#define T1  0 // color change charge balance pre-phase
//#define T2  0 // color change or sustain charge balance pre-phase
//#define T3  0 // color change or sustain phase
//#define T4 25 // color change phase

// new waveform created by Jean-Marc Zingg for the actual panel
#define T1 25 // color change charge balance pre-phase
#define T2  1 // color change or sustain charge balance pre-phase
#define T3  2 // color change or sustain phase
#define T4 25 // color change phase

// for new waveform without sustain phase: uncomment next 2 lines, not good for fat fonts
//#define T2  0 // color change or sustain charge balance pre-phase // 0 no sustain
//#define T3  0 // color change or sustain phase // 0 no sustain

// "balanced flash once" variant
//#define T1  0 // color change charge balance pre-phase
//#define T2 25 // color change or sustain charge balance pre-phase
//#define T3 25 // color change or sustain phase
//#define T4  0 // color change phase

const unsigned char GxGDEW042T2::lut_20_vcom0_partial[] PROGMEM =
{
  0x00, T1, T2, T3, T4, 1, // 00 00 00 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char GxGDEW042T2::lut_21_ww_partial[] PROGMEM =
{ // 10 w
  0x18, T1, T2, T3, T4, 1, // 00 01 10 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char GxGDEW042T2::lut_22_bw_partial[] PROGMEM =
{ // 10 w
  0x5A, T1, T2, T3, T4, 1, // 01 01 10 10
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char GxGDEW042T2::lut_23_wb_partial[] PROGMEM =
{ // 01 b
  0xA5, T1, T2, T3, T4, 1, // 10 10 01 01
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

const unsigned char GxGDEW042T2::lut_24_bb_partial[] PROGMEM =
{ // 01 b
  0x24, T1, T2, T3, T4, 1, // 00 10 01 00
  0x00,  1,  0,  0,  0, 1, // gnd phase
};

void GxGDEW042T2::_Init_PartialUpdate()
{
  IO.writeCommandTransaction(0x00);
  IO.writeDataTransaction(0x3F); //300x400 B/W mode, LUT set by register
  IO.writeCommandTransaction(0x20);
  _writeDataPGM(lut_20_vcom0_partial, sizeof(lut_20_vcom0_partial), 44 - sizeof(lut_20_vcom0_partial));
  IO.writeCommandTransaction(0x21);
  _writeDataPGM(lut_21_ww_partial, sizeof(lut_21_ww_partial), 42 - sizeof(lut_21_ww_partial));
  IO.writeCommandTransaction(0x22);
  _writeDataPGM(lut_22_bw_partial, sizeof(lut_22_bw_partial), 42 - sizeof(lut_22_bw_partial));
  IO.writeCommandTransaction(0x23);
  _writeDataPGM(lut_23_wb_partial, sizeof(lut_23_wb_partial), 42 - sizeof(lut_23_wb_partial));
  IO.writeCommandTransaction(0x24);
  _writeDataPGM(lut_24_bb_partial, sizeof(lut_24_bb_partial), 42 - sizeof(lut_24_bb_partial));
}
