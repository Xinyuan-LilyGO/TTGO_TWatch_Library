// class GxGDEW075T8 : Display class for GDEW075T8 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=525.html
// Controller: IL0371 : http://www.good-display.com/download_detail/downloadsId=536.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

// note 28.8.2017 : the controller UC8159 seems to know command 90H Partial Window, but neither 91H Partial In nor 92H Partial Out
// the GDEW075T8 specification does not contain these commands.
// the specification for UC8159 from UltraChip does not contain a command list, and their internet presence is "very up-to-date";
// see http://www.ultrachip.com/en/news.php?id=47

#include "GxGDEW075T8.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// Partial Update Delay, may have an influence on degradation
#define GxGDEW075T8_PU_DELAY 500

GxGDEW075T8::GxGDEW075T8(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW075T8_WIDTH, GxGDEW075T8_HEIGHT), IO(io),
    _current_page(-1), _using_partial_mode(false), _diag_enabled(false),
    _rst(rst), _busy(busy)
{
}

void GxGDEW075T8::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW075T8_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW075T8_WIDTH - x - 1;
      y = GxGDEW075T8_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW075T8_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW075T8_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW075T8_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW075T8_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW075T8_WIDTH / 8;
  }

  if (!color)
    _buffer[i] = (_buffer[i] | (1 << (7 - x % 8)));
  else
    _buffer[i] = (_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
}

void GxGDEW075T8::init(uint32_t serial_diag_bitrate)
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

void GxGDEW075T8::fillScreen(uint16_t color)
{
  uint8_t data = (color == GxEPD_BLACK) ? 0xFF : 0x00;
  for (uint16_t x = 0; x < sizeof(_buffer); x++)
  {
    _buffer[x] = data;
  }
}

void GxGDEW075T8::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (uint32_t i = 0; i < GxGDEW075T8_BUFFER_SIZE; i++)
  {
#if defined(ESP8266)
    // (10000 * 8bit * (8bits/bit + gap)/ 4MHz = ~ 200ms
    // (31000 * 8bit * (8bits/bit + gap)/ 4MHz = ~ 600ms is safe
    // if ((i % 10000) == 0) yield(); // avoid watchdog reset
#endif
    _send8pixel(i < sizeof(_buffer) ? _buffer[i] : 0x00);
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("update");
  _sleep();
}

void  GxGDEW075T8::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_normal;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW075T8::drawBitmap(const uint8_t* bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW075T8_WIDTH - 1, GxGDEW075T8_HEIGHT - 1);
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075T8_BUFFER_SIZE; i++)
    {
      uint8_t data = 0x00; // white is 0x00 on device
      if (i < size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[i]);
#else
        data = bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      _send8pixel(data);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075T8_BUFFER_SIZE; i++)
    {
#if defined(ESP8266)
      // (10000 * 8bit * (8bits/bit + gap)/ 4MHz = ~ 200ms
      // (31000 * 8bit * (8bits/bit + gap)/ 4MHz = ~ 600ms is safe
      // if ((i % 10000) == 0) yield(); // avoid watchdog reset
#endif
      uint8_t data = 0x00; // white is 0x00 on device
      if (i < size)
      {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
        data = pgm_read_byte(&bitmap[i]);
#else
        data = bitmap[i];
#endif
        if (mode & bm_invert) data = ~data;
      }
      _send8pixel(data);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    _sleep();
  }
}

void GxGDEW075T8::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    if (!_using_partial_mode) _wakeUp();
    _using_partial_mode = true; // remember
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW075T8_WIDTH - 1, GxGDEW075T8_HEIGHT - 1);
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075T8_BUFFER_SIZE; i++)
    {
      _send8pixel(0x00);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
    IO.writeCommandTransaction(0x92); // partial out
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075T8_BUFFER_SIZE; i++)
    {
      _send8pixel(0x00);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW075T8::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(x, y);
        swap(w, h);
        x = GxGDEW075T8_WIDTH - x - w - 1;
        break;
      case 2:
        x = GxGDEW075T8_WIDTH - x - w - 1;
        y = GxGDEW075T8_HEIGHT - y - h - 1;
        break;
      case 3:
        swap(x, y);
        swap(w, h);
        y = GxGDEW075T8_HEIGHT - y  - h - 1;
        break;
    }
  }
  if (x >= GxGDEW075T8_WIDTH) return;
  if (y >= GxGDEW075T8_HEIGHT) return;
  // x &= 0xFFF8; // byte boundary, not here, use encompassing rectangle
  uint16_t xe = gx_uint16_min(GxGDEW075T8_WIDTH, x + w) - 1;
  uint16_t ye = gx_uint16_min(GxGDEW075T8_HEIGHT, y + h) - 1;
  // x &= 0xFFF8; // byte boundary, not needed here
  uint16_t xs_bx = x / 8;
  uint16_t xe_bx = (xe + 7) / 8;
  //if (!_using_partial_mode) _wakeUp();
  if (!_using_partial_mode) eraseDisplay(true); // clean surrounding
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(x, y, xe, ye);
  IO.writeCommandTransaction(0x10);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
      _send8pixel((idx < sizeof(_buffer)) ? _buffer[idx] : 0x00);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateWindow");
  IO.writeCommandTransaction(0x92); // partial out
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _writeToWindow(xs, ys, xd, yd, w, h, using_rotation);
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateToWindow");
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::_writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW075T8_WIDTH - xs - w - 1;
        xd = GxGDEW075T8_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW075T8_WIDTH - xs - w - 1;
        ys = GxGDEW075T8_HEIGHT - ys - h - 1;
        xd = GxGDEW075T8_WIDTH - xd - w - 1;
        yd = GxGDEW075T8_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW075T8_HEIGHT - ys  - h - 1;
        yd = GxGDEW075T8_HEIGHT - yd  - h - 1;
        break;
    }
  }
  if (xs >= GxGDEW075T8_WIDTH) return;
  if (ys >= GxGDEW075T8_HEIGHT) return;
  if (xd >= GxGDEW075T8_WIDTH) return;
  if (yd >= GxGDEW075T8_HEIGHT) return;
  // the screen limits are the hard limits
  uint16_t xde = gx_uint16_min(GxGDEW075T8_WIDTH, xd + w) - 1;
  uint16_t yde = gx_uint16_min(GxGDEW075T8_HEIGHT, yd + h) - 1;
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
      uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
      _send8pixel((idx < sizeof(_buffer)) ? _buffer[idx] : 0x00);
    }
  }
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW075T8::powerDown()
{
  _using_partial_mode = false; // force _wakeUp()
  _sleep();
}

uint16_t GxGDEW075T8::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye)
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
  //IO.writeDataTransaction(0x01); // don't see any difference
  IO.writeDataTransaction(0x00); // don't see any difference
  return (7 + xe - x) / 8; // number of bytes to transfer per line
}

void GxGDEW075T8::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  { //=0 BUSY
    if (digitalRead(_busy) == 1) break;
     delay(1);
    if (micros() - start > 10000000)
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

void GxGDEW075T8::_send8pixel(uint8_t data)
{
  for (uint8_t j = 0; j < 8; j++)
  {
    uint8_t t = data & 0x80 ? 0x00 : 0x03;
    t <<= 4;
    data <<= 1;
    j++;
    t |= data & 0x80 ? 0x00 : 0x03;
    data <<= 1;
    IO.writeDataTransaction(t);
  }
}

void GxGDEW075T8::_wakeUp()
{
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }

  /**********************************release flash sleep**********************************/
  IO.writeCommandTransaction(0X65);     //FLASH CONTROL
  IO.writeDataTransaction(0x01);

  IO.writeCommandTransaction(0xAB);

  IO.writeCommandTransaction(0X65);     //FLASH CONTROL
  IO.writeDataTransaction(0x00);
  /**********************************release flash sleep**********************************/
  IO.writeCommandTransaction(0x01);
  IO.writeDataTransaction (0x37);       //POWER SETTING
  IO.writeDataTransaction (0x00);

  IO.writeCommandTransaction(0X00);     //PANNEL SETTING
  IO.writeDataTransaction(0xCF);
  IO.writeDataTransaction(0x08);

  IO.writeCommandTransaction(0x06);     //boost
  IO.writeDataTransaction (0xc7);
  IO.writeDataTransaction (0xcc);
  IO.writeDataTransaction (0x28);

  IO.writeCommandTransaction(0x30);     //PLL setting
  IO.writeDataTransaction (0x3c);

  IO.writeCommandTransaction(0X41);     //TEMPERATURE SETTING
  IO.writeDataTransaction(0x00);

  IO.writeCommandTransaction(0X50);     //VCOM AND DATA INTERVAL SETTING
  IO.writeDataTransaction(0x77);

  IO.writeCommandTransaction(0X60);     //TCON SETTING
  IO.writeDataTransaction(0x22);

  IO.writeCommandTransaction(0x61);     //tres 640*384
  IO.writeDataTransaction (0x02);       //source 640
  IO.writeDataTransaction (0x80);
  IO.writeDataTransaction (0x01);       //gate 384
  IO.writeDataTransaction (0x80);

  IO.writeCommandTransaction(0X82);     //VDCS SETTING
  IO.writeDataTransaction(0x1E);        //decide by LUT file

  IO.writeCommandTransaction(0xe5);     //FLASH MODE
  IO.writeDataTransaction(0x03);

  IO.writeCommandTransaction(0x04);     //POWER ON
  _waitWhileBusy();
}

void GxGDEW075T8::_sleep(void)
{
  /**********************************flash sleep**********************************/
  IO.writeCommandTransaction(0X65);     //FLASH CONTROL
  IO.writeDataTransaction(0x01);

  IO.writeCommandTransaction(0xB9);

  IO.writeCommandTransaction(0X65);     //FLASH CONTROL
  IO.writeDataTransaction(0x00);
  /**********************************flash sleep**********************************/

  IO.writeCommandTransaction(0x02);     // POWER OFF
  _waitWhileBusy();

  if (_rst >= 0)
  {
    IO.writeCommandTransaction(0x07);     // DEEP SLEEP
    IO.writeDataTransaction(0xa5);
  }
}

void GxGDEW075T8::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW075T8_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075T8_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        _send8pixel(data);
      }
    }
#if defined(ESP8266)
    yield();
#endif
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW075T8::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW075T8_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075T8_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        _send8pixel(data);
      }
    }
#if defined(ESP8266)
    yield();
#endif
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW075T8::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW075T8_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075T8_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        _send8pixel(data);
      }
    }
#if defined(ESP8266)
    yield();
#endif
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW075T8::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW075T8_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075T8_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW075T8_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
        _send8pixel(data);
      }
    }
#if defined(ESP8266)
    yield();
#endif
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW075T8::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW075T8_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW075T8_WIDTH - x - w - 1;
      y = GxGDEW075T8_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW075T8_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW075T8::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075T8_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075T8_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW075T8_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075T8_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075T8_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW075T8_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075T8_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075T8_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW075T8_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075T8_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075T8_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075T8_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW075T8_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075T8_PU_DELAY); // don't stress this display
}

void GxGDEW075T8::drawCornerTest(uint8_t em)
{
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (uint32_t y = 0; y < GxGDEW075T8_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW075T8_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW075T8_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW075T8_WIDTH / 8 - 4) && (y > GxGDEW075T8_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW075T8_HEIGHT - 33)) data = 0x00;
      _send8pixel(~data);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}

