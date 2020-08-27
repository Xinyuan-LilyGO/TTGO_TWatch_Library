// class GxGDEW075Z09 : Display class for GDEW075Z09 e-Paper from Dalian Good Display Co., Ltd.: www.good-display.com
//
// based on Demo Example from Good Display, available here: http://www.good-display.com/download_detail/downloadsId=526.html
// Controller: IL0371 : http://www.good-display.com/download_detail/downloadsId=536.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEW075Z09.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// Partial Update Delay, may have an influence on degradation
#define GxGDEW075Z09_PU_DELAY 500

GxGDEW075Z09::GxGDEW075Z09(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW075Z09_WIDTH, GxGDEW075Z09_HEIGHT), IO(io),
    _current_page(-1), _using_partial_mode(false), _diag_enabled(false),
    _rst(rst), _busy(busy)
{
}

void GxGDEW075Z09::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW075Z09_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW075Z09_WIDTH - x - 1;
      y = GxGDEW075Z09_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW075Z09_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW075Z09_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_black_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW075Z09_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW075Z09_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW075Z09_WIDTH / 8;
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

void GxGDEW075Z09::init(uint32_t serial_diag_bitrate)
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

void GxGDEW075Z09::fillScreen(uint16_t color)
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

void GxGDEW075Z09::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
  {
    uint32_t idx = i % sizeof(_black_buffer);
    _send8pixel(_black_buffer[idx], _red_buffer[idx]);
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("update");
  _sleep();
}

void  GxGDEW075Z09::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_normal;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW075Z09::drawExamplePicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size)
{
  drawPicture(black_bitmap, red_bitmap, black_size, red_size, bm_normal);
}

void GxGDEW075Z09::drawPicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW075Z09_WIDTH - 1, GxGDEW075Z09_HEIGHT - 1);
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
    {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
      uint8_t black_data = (i < black_size) ? pgm_read_byte(&black_bitmap[i]) : 0x00;
      uint8_t red_data = (i < red_size) ? pgm_read_byte(&red_bitmap[i]) : 0x00;
#else
      uint8_t black_data = (i < black_size) ? black_bitmap[i] : 0x00;
      uint8_t red_data = (i < red_size) ? red_bitmap[i] : 0x00;
#endif
      if (mode & bm_invert) black_data = ~black_data;
      if (mode & bm_invert_red) red_data = ~red_data;
      _send8pixel(black_data, red_data);
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
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
    {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
      uint8_t black_data = (i < black_size) ? pgm_read_byte(&black_bitmap[i]) : 0x00;
      uint8_t red_data = (i < red_size) ? pgm_read_byte(&red_bitmap[i]) : 0x00;
#else
      uint8_t black_data = (i < black_size) ? black_bitmap[i] : 0x00;
      uint8_t red_data = (i < red_size) ? red_bitmap[i] : 0x00;
#endif
      if (mode & bm_invert) black_data = ~black_data;
      if (mode & bm_invert_red) red_data = ~red_data;
      _send8pixel(black_data, red_data);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    _sleep();
  }
}

void GxGDEW075Z09::drawExamplePicture_3C(const uint8_t* bitmap_3C, uint32_t size_3C, int16_t mode)
{
  unsigned long int i;
  unsigned char j, temp1, temp2, temp3;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x91); // partial in
  }
  else
  {
    _using_partial_mode = false; // remember
    _wakeUp();
  }
  IO.writeCommandTransaction(0x10);
  for (i = 0; i < 2 * GxGDEW075Z09_BYTE_SIZE; i++)
  {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
    temp1 = (i < size_3C) ? pgm_read_byte(&bitmap_3C[i]) : 0x00;
#else
    temp1 = (i < size_3C) ? bitmap_3C[i] : 0x00;
#endif
    for (j = 0; j < 4; j++)
    {
      temp2 = temp1 & 0xc0 ;
      if (temp2 == 0xc0) temp3 = 0x00;
      else if (temp2 == 0x00) temp3 = 0x03;
      else temp3 = 0x04;
      temp1 <<= 2;
      temp3 <<= 4;
      j++;
      temp2 = temp1 & 0xc0 ;
      if (temp2 == 0xc0) temp3 |= 0x00;
      else if (temp2 == 0x00) temp3 |= 0x03;
      else temp3 |= 0x04;
      temp1 <<= 2;
      IO.writeDataTransaction(temp3);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawExamplePicture_3C");
  if (_using_partial_mode) IO.writeCommandTransaction(0x92); // partial out  
  else _sleep();
}

void GxGDEW075Z09::drawBitmap(const uint8_t* bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _wakeUp();
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW075Z09_WIDTH - 1, GxGDEW075Z09_HEIGHT - 1);
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
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
      _send8pixel(data, 0x00);
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
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
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
      _send8pixel(data, 0x00);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("drawBitmap");
    _sleep();
  }
}

void GxGDEW075Z09::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    if (!_using_partial_mode) _wakeUp();
    _using_partial_mode = true; // remember
    IO.writeCommandTransaction(0x91); // partial in
    _setPartialRamArea(0, 0, GxGDEW075Z09_WIDTH - 1, GxGDEW075Z09_HEIGHT - 1);
    IO.writeCommandTransaction(0x10);
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
    {
      _send8pixel(0x00, 0x00);
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
    for (uint32_t i = 0; i < GxGDEW075Z09_BYTE_SIZE; i++)
    {
      _send8pixel(0x00, 0x00);
    }
    IO.writeCommandTransaction(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW075Z09::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(x, y);
        swap(w, h);
        x = GxGDEW075Z09_WIDTH - x - w - 1;
        break;
      case 2:
        x = GxGDEW075Z09_WIDTH - x - w - 1;
        y = GxGDEW075Z09_HEIGHT - y - h - 1;
        break;
      case 3:
        swap(x, y);
        swap(w, h);
        y = GxGDEW075Z09_HEIGHT - y  - h - 1;
        break;
    }
  }
  if (x >= GxGDEW075Z09_WIDTH) return;
  if (y >= GxGDEW075Z09_HEIGHT) return;
  // x &= 0xFFF8; // byte boundary, not here, use encompassing rectangle
  uint16_t xe = gx_uint16_min(GxGDEW075Z09_WIDTH, x + w) - 1;
  uint16_t ye = gx_uint16_min(GxGDEW075Z09_HEIGHT, y + h) - 1;
  // x &= 0xFFF8; // byte boundary, not needed here
  uint16_t xs_bx = x / 8;
  uint16_t xe_bx = (xe + 7) / 8;
  if (!_using_partial_mode) eraseDisplay(true); // clean surrounding
  _using_partial_mode = true;
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(x, y, xe, ye);
  IO.writeCommandTransaction(0x10);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_bx; x1 < xe_bx; x1++)
    {
      uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
      _send8pixel(_black_buffer[idx], _red_buffer[idx]);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateWindow");
  IO.writeCommandTransaction(0x92); // partial out
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _writeToWindow(xs, ys, xd, yd, w, h, using_rotation);
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("updateToWindow");
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::_writeToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW075Z09_WIDTH - xs - w - 1;
        xd = GxGDEW075Z09_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW075Z09_WIDTH - xs - w - 1;
        ys = GxGDEW075Z09_HEIGHT - ys - h - 1;
        xd = GxGDEW075Z09_WIDTH - xd - w - 1;
        yd = GxGDEW075Z09_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW075Z09_HEIGHT - ys  - h - 1;
        yd = GxGDEW075Z09_HEIGHT - yd  - h - 1;
        break;
    }
  }
  if (xs >= GxGDEW075Z09_WIDTH) return;
  if (ys >= GxGDEW075Z09_HEIGHT) return;
  if (xd >= GxGDEW075Z09_WIDTH) return;
  if (yd >= GxGDEW075Z09_HEIGHT) return;
  // the screen limits are the hard limits
  uint16_t xde = gx_uint16_min(GxGDEW075Z09_WIDTH, xd + w) - 1;
  uint16_t yde = gx_uint16_min(GxGDEW075Z09_HEIGHT, yd + h) - 1;
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
      uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
      _send8pixel(_black_buffer[idx], _red_buffer[idx]);
    }
  }
  IO.writeCommandTransaction(0x92); // partial out
}

void GxGDEW075Z09::powerDown()
{
  _using_partial_mode = false; // force _wakeUp()
  _sleep();
}

uint16_t GxGDEW075Z09::_setPartialRamArea(uint16_t x, uint16_t y, uint16_t xe, uint16_t ye)
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
  return (7 + xe - x) / 8; // number of bytes to transfer per line
}

void GxGDEW075Z09::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  { //=0 BUSY
    if (digitalRead(_busy) == 1) break;
    delay(1);
    if (micros() - start > 40000000)
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

void GxGDEW075Z09::_send8pixel(uint8_t black_data, uint8_t red_data)
{
  for (uint8_t j = 0; j < 8; j++)
  {
    uint8_t t = 0x00; // black
    if (black_data & 0x80); // keep black
    else if (red_data & 0x80) t = 0x04; //red
    else t = 0x03; // white
    t <<= 4;
    black_data <<= 1;
    red_data <<= 1;
    j++;
    if (black_data & 0x80); // keep black
    else if (red_data & 0x80) t |= 0x04; //red
    else t |= 0x03; // white
    black_data <<= 1;
    red_data <<= 1;
    IO.writeDataTransaction(t);
  }
}

void GxGDEW075Z09::_wakeUp()
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

void GxGDEW075Z09::_sleep(void)
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

void GxGDEW075Z09::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW075Z09_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075Z09_WIDTH / 8; x1++)
      {
        uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
        _send8pixel(_black_buffer[idx], _red_buffer[idx]);
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

void GxGDEW075Z09::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW075Z09_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075Z09_WIDTH / 8; x1++)
      {
        uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
        _send8pixel(_black_buffer[idx], _red_buffer[idx]);
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

void GxGDEW075Z09::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW075Z09_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075Z09_WIDTH / 8; x1++)
      {
        uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
        _send8pixel(_black_buffer[idx], _red_buffer[idx]);
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

void GxGDEW075Z09::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW075Z09_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW075Z09_WIDTH / 8; x1++)
      {
        uint16_t idx = (y1 * (GxGDEW075Z09_WIDTH / 8) + x1) % sizeof(_black_buffer);
        _send8pixel(_black_buffer[idx], _red_buffer[idx]);
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

void GxGDEW075Z09::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW075Z09_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW075Z09_WIDTH - x - w - 1;
      y = GxGDEW075Z09_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW075Z09_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW075Z09::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075Z09_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075Z09_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      uint16_t ys = yds % GxGDEW075Z09_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075Z09_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075Z09_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW075Z09_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075Z09_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075Z09_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      uint16_t ys = yds % GxGDEW075Z09_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  for (_current_page = 0; _current_page < GxGDEW075Z09_PAGES; _current_page++)
  {
    uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW075Z09_PAGE_HEIGHT);
    uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW075Z09_PAGE_HEIGHT);
    if (yde > yds)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      uint16_t ys = yds % GxGDEW075Z09_PAGE_HEIGHT;
      _writeToWindow(x, ys, x, yds, w, yde - yds, false);
    }
  }
  _current_page = -1;
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawPagedToWindow");
  delay(GxGDEW075Z09_PU_DELAY); // don't stress this display
}

void GxGDEW075Z09::drawCornerTest(uint8_t em)
{
  _wakeUp();
  IO.writeCommandTransaction(0x10);
  for (uint32_t y = 0; y < GxGDEW075Z09_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW075Z09_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW075Z09_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW075Z09_WIDTH / 8 - 4) && (y > GxGDEW075Z09_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW075Z09_HEIGHT - 33)) data = 0x00;
      _send8pixel(~data, 0x00);
    }
  }
  IO.writeCommandTransaction(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}

