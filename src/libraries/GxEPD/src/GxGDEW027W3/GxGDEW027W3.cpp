// class GxGDEW027W3 : Display class for GDEW027W3 e-Paper from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=515.html
// Controller: IL91874 : http://www.e-paper-display.com/download_detail/downloadsId=539.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEW027W3.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

//full screen update LUT
const uint8_t GxGDEW027W3::lut_20_vcomDC[] =
{
  0x00, 0x00,
  0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x00, 0x14, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x12, 0x12, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_21_ww[] =
{
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
  0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_22_bw[] =
{
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x14, 0x00, 0x00, 0x00, 0x01,
  0xA0, 0x12, 0x12, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_23_wb[] =
{
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_24_bb[] =
{
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x14, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x12, 0x12, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//partial screen update LUT
const uint8_t GxGDEW027W3::lut_20_vcomDC_partial[] =
{
  0x00, 0x00,
  0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_21_ww_partial[] =
{
  0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_22_bw_partial[] =
{
  0x80, 0x19, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_23_wb_partial[] =
{
  0x40, 0x19, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const uint8_t GxGDEW027W3::lut_24_bb_partial[] =
{
  0x00, 0x19, 0x01, 0x00, 0x00, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

GxGDEW027W3::GxGDEW027W3(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT), IO(io),
    _current_page(-1), _initial(true), _using_partial_mode(false), _diag_enabled(false),
    _rst(rst), _busy(busy)
{
}

void GxGDEW027W3::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW027W3_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW027W3_WIDTH - x - 1;
      y = GxGDEW027W3_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW027W3_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW027W3_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEW027W3_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEW027W3_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEW027W3_WIDTH / 8;
  }

  if (!color)
    _buffer[i] = (_buffer[i] | (1 << (7 - x % 8)));
  else
    _buffer[i] = (_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
}


void GxGDEW027W3::init(uint32_t serial_diag_bitrate)
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

void GxGDEW027W3::fillScreen(uint16_t color)
{
  uint8_t data = (color == GxEPD_BLACK) ? 0xFF : 0x00;
  for (uint16_t x = 0; x < sizeof(_buffer); x++)
  {
    _buffer[x] = data;
  }
}

void GxGDEW027W3::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  if (_initial)
  { // init old data
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
    _initial = false;
  }
  _writeCommand(0x13); // update current data
  for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
  {
    _writeData((i < sizeof(_buffer)) ? ~_buffer[i] : 0xFF);
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("update");
  _writeCommand(0x10); // update old data
  for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
  {
    _writeData((i < sizeof(_buffer)) ? ~_buffer[i] : 0xFF);
  }
  _sleep();
}

void  GxGDEW027W3::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_invert;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW027W3::drawBitmap(const uint8_t* bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_default) mode |= bm_normal; // no change
  if (mode & bm_partial_update)
  {
    if (!_using_partial_mode)
    {
      _wakeUp();
      _using_partial_mode = true; // remember
      // init old data and keep full update wavetable
      _setPartialRamArea(0x14, 0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
      for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
      {
        _writeData(0xFF); // 0xFF is white
      }
    }
    else _Init_PartialUpdate(); // use partial update wavetable
    for (uint8_t command = 0x15; true; command = 0x14)
    { // leave both controller buffers equal
      _setPartialRamArea(command, 0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
      for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
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
      if (command == 0x14) break;
      _refreshWindow(0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
      _waitWhileBusy("drawBitmap");
    }
  }
  else
  {
    _using_partial_mode = false;
    _wakeUp();
    if (_initial)
    { // init old data
      _writeCommand(0x10);
      for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
      {
        _writeData(0xFF);
      }
    }
    for (uint8_t command = 0x13; true; command = 0x10)
    { // leave both controller buffers equal
      _writeCommand(command);
      for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
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
      if (command == 0x10) break;
      _writeCommand(0x12); //display refresh
      _waitWhileBusy("drawBitmap");
    }
    _sleep();
  }
}

void GxGDEW027W3::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    if (!_using_partial_mode)
    {
      _wakeUp();
      _using_partial_mode = true; // remember
      // init old data and keep full update wavetable
      _setPartialRamArea(0x14, 0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
      for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
      {
        _writeData(0xFF); // 0xFF is white
      }
    }
    else _Init_PartialUpdate(); // use partial update wavetable
    _setPartialRamArea(0x15, 0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
    for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
    _refreshWindow(0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
    _waitWhileBusy("drawBitmap");
    // leave both controller buffers equal
    _setPartialRamArea(0x14, 0, 0, GxGDEW027W3_WIDTH, GxGDEW027W3_HEIGHT);
    for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
  }
  else
  {
    _using_partial_mode = false;
    _wakeUp();
    _writeCommand(0x10);
    for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
    _writeCommand(0x13);
    for (uint32_t i = 0; i < GxGDEW027W3_BUFFER_SIZE; i++)
    {
      _writeData(0xFF); // 0xFF is white
    }
    _writeCommand(0x12);      //display refresh
    _waitWhileBusy("eraseDisplay");
    _sleep();
  }
}

void GxGDEW027W3::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (_current_page != -1) return;
  if (using_rotation) _rotate(x, y, w, h);
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _Init_PartialUpdate();
  _writeToWindow(0x15, x, y, x, y, w, h);
  _refreshWindow(x, y, w, h);
  _waitWhileBusy("updateWindow");
  // leave both controller buffers equal
  _writeToWindow(0x14, x, y, x, y, w, h);
  //_refreshWindow(x, y, w, h);
  //_waitWhileBusy("updateWindow");
}

void GxGDEW027W3::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEW027W3_WIDTH - xs - w - 1;
        xd = GxGDEW027W3_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEW027W3_WIDTH - xs - w - 1;
        ys = GxGDEW027W3_HEIGHT - ys - h - 1;
        xd = GxGDEW027W3_WIDTH - xd - w - 1;
        yd = GxGDEW027W3_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEW027W3_HEIGHT - ys  - h - 1;
        yd = GxGDEW027W3_HEIGHT - yd  - h - 1;
        break;
    }
  }
  if (!_using_partial_mode) _wakeUp();
  _using_partial_mode = true;
  _Init_PartialUpdate();
  _writeToWindow(0x15, xs, ys, xd, yd, w, h);
  _refreshWindow(xd, yd, w, h);
  _waitWhileBusy("updateToWindow");
  // leave both controller buffers equal
  _writeToWindow(0x14, xs, ys, xd, yd, w, h);
  //_refreshWindow(xd, yd, w, h);
  //_waitWhileBusy("updateToWindow");
  delay(500); // don't stress this display
}

void GxGDEW027W3::_writeToWindow(uint8_t command, uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h)
{
  //Serial.printf("_writeToWindow(%d, %d, %d, %d, %d, %d)\n", xs, ys, xd, yd, w, h);
  // the screen limits are the hard limits
  if (xs >= GxGDEW027W3_WIDTH) return;
  if (ys >= GxGDEW027W3_HEIGHT) return;
  if (xd >= GxGDEW027W3_WIDTH) return;
  if (yd >= GxGDEW027W3_HEIGHT) return;
  w = gx_uint16_min(w + 7, GxGDEW027W3_WIDTH - xd) + (xd % 8);
  h = gx_uint16_min(h, GxGDEW027W3_HEIGHT - yd);
  uint16_t xe = (xs / 8) + (w / 8);
  IO.writeCommandTransaction(0x91); // partial in
  _setPartialRamArea(command, xd, yd, w, h);
  for (uint16_t y1 = ys; y1 < ys + h; y1++)
  {
    for (uint16_t x1 = xs / 8; x1 < xe; x1++)
    {
      uint16_t idx = y1 * (GxGDEW027W3_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      IO.writeDataTransaction(~data);
    }
  }
  delay(2);
}

void GxGDEW027W3::powerDown()
{
  _using_partial_mode = false; // force _wakeUp()
  _sleep();
}

void GxGDEW027W3::_setPartialRamArea(uint8_t command, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  IO.writeCommandTransaction(command);
  IO.writeDataTransaction(x >> 8);
  IO.writeDataTransaction(x & 0xf8);
  IO.writeDataTransaction(y >> 8);
  IO.writeDataTransaction(y & 0xff);
  IO.writeDataTransaction(w >> 8);
  IO.writeDataTransaction(w & 0xf8);
  IO.writeDataTransaction(h >> 8);
  IO.writeDataTransaction(h & 0xff);
}

void GxGDEW027W3::_refreshWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  w += (x % 8) + 7;
  h = gx_uint16_min(h, 256); // strange controller error
  IO.writeCommandTransaction(0x16);
  IO.writeDataTransaction(x >> 8);
  IO.writeDataTransaction(x & 0xf8);
  IO.writeDataTransaction(y >> 8);
  IO.writeDataTransaction(y & 0xff);
  IO.writeDataTransaction(w >> 8);
  IO.writeDataTransaction(w & 0xf8);
  IO.writeDataTransaction(h >> 8);
  IO.writeDataTransaction(h & 0xff);
}

void GxGDEW027W3::_writeCommand(uint8_t command)
{
  IO.writeCommandTransaction(command);
}

void GxGDEW027W3::_writeData(uint8_t data)
{
  IO.writeDataTransaction(data);
}

void GxGDEW027W3::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  {
    if (digitalRead(_busy) == 1) break;
    delay(1);
    if (micros() - start > 20000000) // > 15.5s !
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

void GxGDEW027W3::_wakeUp()
{
  // reset required for wakeup
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }

  _writeCommand(0x01); //POWER SETTING
  _writeData (0x03);
  _writeData (0x00);
  _writeData (0x2b);
  _writeData (0x2b);

  _writeCommand(0x06); //boost
  _writeData (0x07);   //A
  _writeData (0x07);   //B
  _writeData (0x17);   //C

  _writeCommand(0x16);
  _writeData(0x00);

  _writeCommand(0x04);
  _waitWhileBusy("_wakeUp Power On");

  _writeCommand(0x00); //panel setting
  _writeData(0xbf);    //KW-BF   KWR-AF  BWROTP 0f

  _writeCommand(0x30); //PLL setting
  _writeData (0x3a);   //90 50HZ  3A 100HZ   29 150Hz 39 200HZ 31 171HZ

  _writeCommand(0x61); //resolution setting
  _writeData (0x00);
  _writeData (0xb0); //176
  _writeData (0x01);
  _writeData (0x08); //264

  _writeCommand(0x82); //vcom_DC setting
  _writeData (0x08);   //0x28:-2.0V,0x12:-0.9V
  delay(2);
  _writeCommand(0X50); //VCOM AND DATA INTERVAL SETTING
  _writeData(0x97);    //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
  _Init_FullUpdate();
}

void GxGDEW027W3::_sleep(void)
{
  _writeCommand(0x02); // power off
  _waitWhileBusy("_sleep Power Off");
  if (_rst >= 0)
  {
    _writeCommand(0x07); // deep sleep
    _writeData (0xa5);
  }
}

void GxGDEW027W3::_Init_FullUpdate(void)
{
  _writeCommand(0x82); //vcom_DC setting
  _writeData (0x08);
  _writeCommand(0X50); //VCOM AND DATA INTERVAL SETTING
  _writeData(0x97);    //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
  unsigned int count;
  {
    _writeCommand(0x20);              //vcom
    for (count = 0; count < 44; count++)
    {
      _writeData(lut_20_vcomDC[count]);
    }

    _writeCommand(0x21);              //ww --
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_21_ww[count]);
    }

    _writeCommand(0x22);              //bw r
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_22_bw[count]);
    }

    _writeCommand(0x23);              //wb w
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_23_wb[count]);
    }

    _writeCommand(0x24);              //bb b
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_24_bb[count]);
    }
  }
}

void GxGDEW027W3::_Init_PartialUpdate(void)
{
  _writeCommand(0x82); //vcom_DC setting
  _writeData (0x08);
  _writeCommand(0X50); //VCOM AND DATA INTERVAL SETTING
  _writeData(0x17);    //WBmode:VBDF 17|D7 VBDW 97 VBDB 57   WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7
  unsigned int count;
  {
    _writeCommand(0x20);              //vcom
    for (count = 0; count < 44; count++)
    {
      _writeData(lut_20_vcomDC_partial[count]);
    }

    _writeCommand(0x21);              //ww --
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_21_ww_partial[count]);
    }

    _writeCommand(0x22);              //bw r
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_22_bw_partial[count]);
    }

    _writeCommand(0x23);              //wb w
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_23_wb_partial[count]);
    }

    _writeCommand(0x24);              //bb b
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_24_bb_partial[count]);
    }
  }
}

void GxGDEW027W3::_writeLUT(void)
{
  unsigned int count;
  {
    _writeCommand(0x20);							//vcom
    for (count = 0; count < 44; count++)
    {
      _writeData(lut_20_vcomDC[count]);
    }

    _writeCommand(0x21);							//ww --
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_21_ww[count]);
    }

    _writeCommand(0x22);							//bw r
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_22_bw[count]);
    }

    _writeCommand(0x23);							//wb w
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_23_wb[count]);
    }

    _writeCommand(0x24);							//bb b
    for (count = 0; count < 42; count++)
    {
      _writeData(lut_24_bb[count]);
    }
  }
}

void GxGDEW027W3::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  for (uint8_t command = 0x13; true; command = 0x10)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      for (int16_t y1 = 0; y1 < GxGDEW027W3_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEW027W3_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEW027W3_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x10) break;
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawPaged");
  }
  _sleep();
}

void GxGDEW027W3::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  for (uint8_t command = 0x13; true; command = 0x10)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      for (int16_t y1 = 0; y1 < GxGDEW027W3_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEW027W3_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEW027W3_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x10) break;
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawPaged");
  }
  _sleep();
}

void GxGDEW027W3::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  for (uint8_t command = 0x13; true; command = 0x10)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      for (int16_t y1 = 0; y1 < GxGDEW027W3_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEW027W3_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEW027W3_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x10) break;
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawPaged");
  }
  _sleep();
}

void GxGDEW027W3::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  for (uint8_t command = 0x13; true; command = 0x10)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      for (int16_t y1 = 0; y1 < GxGDEW027W3_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEW027W3_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEW027W3_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x10) break;
    _writeCommand(0x12); //display refresh
    _waitWhileBusy("drawPaged");
  }
  _sleep();
}

void GxGDEW027W3::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEW027W3_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEW027W3_WIDTH - x - w - 1;
      y = GxGDEW027W3_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEW027W3_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEW027W3::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode) eraseDisplay(true);
  _using_partial_mode = true;
  _Init_PartialUpdate();
  for (uint8_t command = 0x15; true; command = 0x14)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW027W3_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW027W3_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback();
        uint16_t ys = yds % GxGDEW027W3_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x14) break;
    _refreshWindow(x, y, w, h);
    _waitWhileBusy("updateToWindow");
  }
}

void GxGDEW027W3::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode) eraseDisplay(true);
  _using_partial_mode = true;
  _Init_PartialUpdate();
  for (uint8_t command = 0x15; true; command = 0x14)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW027W3_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW027W3_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p);
        uint16_t ys = yds % GxGDEW027W3_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x14) break;
    _refreshWindow(x, y, w, h);
    _waitWhileBusy("updateToWindow");
  }
}

void GxGDEW027W3::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode) eraseDisplay(true);
  _using_partial_mode = true;
  _Init_PartialUpdate();
  for (uint8_t command = 0x15; true; command = 0x14)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW027W3_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW027W3_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p);
        uint16_t ys = yds % GxGDEW027W3_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x14) break;
    _refreshWindow(x, y, w, h);
    _waitWhileBusy("updateToWindow");
  }
}

void GxGDEW027W3::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode) eraseDisplay(true);
  _using_partial_mode = true;
  _Init_PartialUpdate();
  for (uint8_t command = 0x15; true; command = 0x14)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEW027W3_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEW027W3_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEW027W3_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p1, p2);
        uint16_t ys = yds % GxGDEW027W3_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x14) break;
    _refreshWindow(x, y, w, h);
    _waitWhileBusy("updateToWindow");
  }
}

void GxGDEW027W3::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _wakeUp();
  _writeCommand(0x13);
  for (uint32_t y = 0; y < GxGDEW027W3_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW027W3_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW027W3_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW027W3_WIDTH / 8 - 4) && (y > GxGDEW027W3_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW027W3_HEIGHT - 33)) data = 0x00;
      _writeData(data);
    }
  }
  _writeCommand(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}
