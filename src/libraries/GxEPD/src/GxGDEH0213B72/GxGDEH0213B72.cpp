// class GxGDEH0213B72 : Display class for GDEH0213B72 e-Paper from Dalian Good Display Co., Ltd.: http://www.e-paper-display.com
//
// based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
// Controller: IL3897 :
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEH0213B72.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

// Partial Update Delay, may have an influence on degradation
#define GxGDEH0213B72_PU_DELAY 300

const uint8_t GxGDEH0213B72::LUT_DATA_full[] =
{
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x03, 0x03, 0x00, 0x00, 0x02, // TP0 A~D RP0
  0x09, 0x09, 0x00, 0x00, 0x02, // TP1 A~D RP1
  0x03, 0x03, 0x00, 0x00, 0x02, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};

const uint8_t GxGDEH0213B72::LUT_DATA_part[] =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x0A, 0x00, 0x00, 0x00, 0x00, // TP0 A~D RP0
  0x00, 0x00, 0x00, 0x00, 0x00, // TP1 A~D RP1
  0x00, 0x00, 0x00, 0x00, 0x00, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};

GxGDEH0213B72::GxGDEH0213B72(GxIO& io, int8_t rst, int8_t busy) :
  GxEPD(GxGDEH0213B72_VISIBLE_WIDTH, GxGDEH0213B72_HEIGHT), IO(io),
  _current_page(-1), _using_partial_mode(false), _diag_enabled(false),
  _rst(rst), _busy(busy)
{
}

void GxGDEH0213B72::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEH0213B72_VISIBLE_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEH0213B72_VISIBLE_WIDTH - x - 1;
      y = GxGDEH0213B72_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEH0213B72_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEH0213B72_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_buffer)) return;
  }
  else
  {
    y -= _current_page * GxGDEH0213B72_PAGE_HEIGHT;
    if ((y < 0) || (y >= GxGDEH0213B72_PAGE_HEIGHT)) return;
    i = x / 8 + y * GxGDEH0213B72_WIDTH / 8;
  }

  if (!color)
    _buffer[i] = (_buffer[i] | (1 << (7 - x % 8)));
  else
    _buffer[i] = (_buffer[i] & (0xFF ^ (1 << (7 - x % 8))));
}

void GxGDEH0213B72::init(uint32_t serial_diag_bitrate)
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

void GxGDEH0213B72::fillScreen(uint16_t color)
{
  uint8_t data = (color == GxEPD_BLACK) ? 0xFF : 0x00;
  for (uint16_t x = 0; x < sizeof(_buffer); x++)
  {
    _buffer[x] = data;
  }
}

void GxGDEH0213B72::update(void)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(0x03);
  _writeCommand(0x24);
  for (uint16_t y = 0; y < GxGDEH0213B72_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < GxGDEH0213B72_WIDTH / 8; x++)
    {
      uint16_t idx = y * (GxGDEH0213B72_WIDTH / 8) + x;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      _writeData(~data);
    }
  }
  _writeCommand(0x26);
  for (uint16_t y = 0; y < GxGDEH0213B72_HEIGHT; y++)
  {
    for (uint16_t x = 0; x < GxGDEH0213B72_WIDTH / 8; x++)
    {
      uint16_t idx = y * (GxGDEH0213B72_WIDTH / 8) + x;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      _writeData(~data);
    }
  }
  _Update_Full();
  _PowerOff();
}

void  GxGDEH0213B72::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_flip_x | bm_invert;
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEH0213B72::drawBitmap(const uint8_t *bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  // example bitmaps are made for y-decrement, x-increment, for origin on opposite corner
  // bm_flip_y for normal display is rotated but avoids black boarder
  if (mode & bm_default) mode |= bm_flip_y;
  uint8_t ram_entry_mode = 0x03; // y-increment, x-increment : normal mode
  if ((mode & bm_flip_y) && (mode & bm_flip_x)) ram_entry_mode = 0x00; // y-decrement, x-decrement
  else if (mode & bm_flip_y) ram_entry_mode = 0x01; // y-decrement, x-increment
  else if (mode & bm_flip_x) ram_entry_mode = 0x02; // y-increment, x-decrement
  if (mode & bm_partial_update)
  {
    _using_partial_mode = true; // remember
    _Init_Part(ram_entry_mode);
    for (uint8_t command = 0x24; true; command = 0x26)
    { // leave both controller buffers equal
      _writeCommand(0x24);
      for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
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
      if (command == 0x26) break;
      _Update_Part();
    }
    delay(GxGDEH0213B72_PU_DELAY);
    _PowerOff();
  }
  else
  {
    _using_partial_mode = false; // remember
    _Init_Full(ram_entry_mode);
    for (uint8_t command = 0x24; true; command = 0x26)
    { // leave both controller buffers equal
      _writeCommand(command);
      for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
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
      if (command == 0x26) break;
    }
    _Update_Full();
    _PowerOff();
  }
}

void GxGDEH0213B72::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  if (using_partial_update)
  {
    _using_partial_mode = true; // remember
    _Init_Part(0x01);
    _writeCommand(0x24);
    for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
    {
      _writeData(0xFF);
    }
    _Update_Part();
    delay(GxGDEH0213B72_PU_DELAY);
    // update erase buffer
    _writeCommand(0x26);
    for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
    {
      _writeData(0xFF);
    }
    delay(GxGDEH0213B72_PU_DELAY);
    _PowerOff();
  }
  else
  {
    _using_partial_mode = false; // remember
    _Init_Full(0x01);
    _writeCommand(0x24);
    for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
    {
      _writeData(0xFF);
    }
    // update erase buffer
    _writeCommand(0x26);
    for (uint32_t i = 0; i < GxGDEH0213B72_BUFFER_SIZE; i++)
    {
      _writeData(0xFF);
    }
    _Update_Full();
    _PowerOff();
  }
}

void GxGDEH0213B72::updateWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool using_rotation)
{
  if (_current_page != -1) return;
  if (using_rotation) _rotate(x, y, w, h);
  if (x >= GxGDEH0213B72_WIDTH) return;
  if (y >= GxGDEH0213B72_HEIGHT) return;
  uint16_t xe = gx_uint16_min(GxGDEH0213B72_WIDTH, x + w) - 1;
  uint16_t ye = gx_uint16_min(GxGDEH0213B72_HEIGHT, y + h) - 1;
  uint16_t xs_d8 = x / 8;
  uint16_t xe_d8 = xe / 8;
  _Init_Part(0x03);
  _SetRamArea(xs_d8, xe_d8, y % 256, y / 256, ye % 256, ye / 256); // X-source area,Y-gate area
  _SetRamPointer(xs_d8, y % 256, y / 256); // set ram
  _waitWhileBusy();
  _writeCommand(0x24);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_d8; x1 <= xe_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      _writeData(~data);
    }
  }
  _Update_Part();
  delay(GxGDEH0213B72_PU_DELAY);
  // update erase buffer
  _SetRamArea(xs_d8, xe_d8, y % 256, y / 256, ye % 256, ye / 256); // X-source area,Y-gate area
  _SetRamPointer(xs_d8, y % 256, y / 256); // set ram
  _waitWhileBusy();
  _writeCommand(0x26);
  for (int16_t y1 = y; y1 <= ye; y1++)
  {
    for (int16_t x1 = xs_d8; x1 <= xe_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      _writeData(~data);
    }
  }
  delay(GxGDEH0213B72_PU_DELAY);
}

void GxGDEH0213B72::_writeToWindow(uint8_t command, uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h)
{
  //Serial.printf("_writeToWindow(%d, %d, %d, %d, %d, %d)\n", xs, ys, xd, yd, w, h);
  // the screen limits are the hard limits
  if (xs >= GxGDEH0213B72_WIDTH) return;
  if (ys >= GxGDEH0213B72_HEIGHT) return;
  if (xd >= GxGDEH0213B72_WIDTH) return;
  if (yd >= GxGDEH0213B72_HEIGHT) return;
  w = gx_uint16_min(w, GxGDEH0213B72_WIDTH - xs);
  w = gx_uint16_min(w, GxGDEH0213B72_WIDTH - xd);
  h = gx_uint16_min(h, GxGDEH0213B72_HEIGHT - ys);
  h = gx_uint16_min(h, GxGDEH0213B72_HEIGHT - yd);
  uint16_t xds_d8 = xd / 8;
  uint16_t xde_d8 = (xd + w - 1) / 8;
  uint16_t yde = yd + h - 1;
  // soft limits, must send as many bytes as set by _SetRamArea
  uint16_t xse_d8 = xs / 8 + xde_d8 - xds_d8;
  uint16_t yse = ys + h - 1;
  _SetRamArea(xds_d8, xde_d8, yd % 256, yd / 256, yde % 256, yde / 256); // X-source area,Y-gate area
  _SetRamPointer(xds_d8, yd % 256, yd / 256); // set ram
  _waitWhileBusy();
  _writeCommand(command);
  for (int16_t y1 = ys; y1 <= yse; y1++)
  {
    for (int16_t x1 = xs / 8; x1 <= xse_d8; x1++)
    {
      uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
      uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
      _writeData(~data);
    }
  }
}

void GxGDEH0213B72::updateToWindow(uint16_t xs, uint16_t ys, uint16_t xd, uint16_t yd, uint16_t w, uint16_t h, bool using_rotation)
{
  if (using_rotation)
  {
    switch (getRotation())
    {
      case 1:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        xs = GxGDEH0213B72_WIDTH - xs - w - 1;
        xd = GxGDEH0213B72_WIDTH - xd - w - 1;
        break;
      case 2:
        xs = GxGDEH0213B72_WIDTH - xs - w - 1;
        ys = GxGDEH0213B72_HEIGHT - ys - h - 1;
        xd = GxGDEH0213B72_WIDTH - xd - w - 1;
        yd = GxGDEH0213B72_HEIGHT - yd - h - 1;
        break;
      case 3:
        swap(xs, ys);
        swap(xd, yd);
        swap(w, h);
        ys = GxGDEH0213B72_HEIGHT - ys  - h - 1;
        yd = GxGDEH0213B72_HEIGHT - yd  - h - 1;
        break;
    }
  }
  _Init_Part(0x03);
  _writeToWindow(0x24, xs, ys, xd, yd, w, h);
  _Update_Part();
  delay(GxGDEH0213B72_PU_DELAY);
  // update erase buffer
  _writeToWindow(0x26, xs, ys, xd, yd, w, h);
  delay(GxGDEH0213B72_PU_DELAY);
}

void GxGDEH0213B72::powerDown()
{
  _using_partial_mode = false;
  _PowerOff();
}

void GxGDEH0213B72::_writeCommand(uint8_t command)
{
  if (digitalRead(_busy))
  {
    String str = String("command 0x") + String(command, HEX);
    _waitWhileBusy(str.c_str());
  }
  IO.writeCommandTransaction(command);
}

void GxGDEH0213B72::_writeData(uint8_t data)
{
  IO.writeDataTransaction(data);
}

void GxGDEH0213B72::_writeData(const uint8_t* data, uint16_t n)
{
  IO.startTransaction();
  for (uint16_t i = 0; i < n; i++)
  {
    IO.writeData(*data++);
  }
  IO.endTransaction();
}

void GxGDEH0213B72::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  {
    if (!digitalRead(_busy)) break;
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

void GxGDEH0213B72::_setRamDataEntryMode(uint8_t em)
{
  const uint16_t xPixelsPar = GxGDEH0213B72_X_PIXELS - 1;
  const uint16_t yPixelsPar = GxGDEH0213B72_Y_PIXELS - 1;
  em = gx_uint16_min(em, 0x03);
  _writeCommand(0x11);
  _writeData(em);
  switch (em)
  {
    case 0x00: // x decrease, y decrease
      _SetRamArea(xPixelsPar / 8, 0x00, yPixelsPar % 256, yPixelsPar / 256, 0x00, 0x00);  // X-source area,Y-gate area
      _SetRamPointer(xPixelsPar / 8, yPixelsPar % 256, yPixelsPar / 256); // set ram
      break;
    case 0x01: // x increase, y decrease : as in demo code
      _SetRamArea(0x00, xPixelsPar / 8, yPixelsPar % 256, yPixelsPar / 256, 0x00, 0x00);  // X-source area,Y-gate area
      _SetRamPointer(0x00, yPixelsPar % 256, yPixelsPar / 256); // set ram
      break;
    case 0x02: // x decrease, y increase
      _SetRamArea(xPixelsPar / 8, 0x00, 0x00, 0x00, yPixelsPar % 256, yPixelsPar / 256);  // X-source area,Y-gate area
      _SetRamPointer(xPixelsPar / 8, 0x00, 0x00); // set ram
      break;
    case 0x03: // x increase, y increase : normal mode
      _SetRamArea(0x00, xPixelsPar / 8, 0x00, 0x00, yPixelsPar % 256, yPixelsPar / 256);  // X-source area,Y-gate area
      _SetRamPointer(0x00, 0x00, 0x00); // set ram
      break;
  }
}

void GxGDEH0213B72::_SetRamArea(uint8_t Xstart, uint8_t Xend, uint8_t Ystart, uint8_t Ystart1, uint8_t Yend, uint8_t Yend1)
{
  _writeCommand(0x44);
  _writeData(Xstart);
  _writeData(Xend);
  _writeCommand(0x45);
  _writeData(Ystart);
  _writeData(Ystart1);
  _writeData(Yend);
  _writeData(Yend1);
}

void GxGDEH0213B72::_SetRamPointer(uint8_t addrX, uint8_t addrY, uint8_t addrY1)
{
  _writeCommand(0x4e);
  _writeData(addrX);
  _writeCommand(0x4f);
  _writeData(addrY);
  _writeData(addrY1);
}

void GxGDEH0213B72::_PowerOn(void)
{
  _writeCommand(0x22);
  _writeData(0xc0);
  _writeCommand(0x20);
  _waitWhileBusy("_PowerOn");
}

void GxGDEH0213B72::_PowerOff(void)
{
  _writeCommand(0x22);
  _writeData(0xc3);
  _writeCommand(0x20);
  _waitWhileBusy("_PowerOff");
}

void GxGDEH0213B72::_InitDisplay(uint8_t em)
{
  _writeCommand(0x74); //set analog block control
  _writeData(0x54);
  _writeCommand(0x7E); //set digital block control
  _writeData(0x3B);
  _writeCommand(0x01); //Driver output control
  _writeData(0xF9);    // (HEIGHT - 1) % 256
  _writeData(0x00);    // (HEIGHT - 1) / 256
  _writeData(0x00);
  _writeCommand(0x3C); //BorderWavefrom
  _writeData(0x03);
  _writeCommand(0x2C); //VCOM Voltage
  _writeData(0x70);    // NA ??
  _writeCommand(0x03); //Gate Driving voltage Control
  _writeData(0x15);    // 19V
  _writeCommand(0x04); //Source Driving voltage Control
  _writeData(0x41);    // VSH1 15V
  _writeData(0xA8);    // VSH2 5V
  _writeData(0x32);    // VSL -15V
  _writeCommand(0x3A); //Dummy Line
  _writeData(0x30);
  _writeCommand(0x3B); //Gate time
  _writeData(0x0A);
  _setRamDataEntryMode(em);
}

void GxGDEH0213B72::_Init_Full(uint8_t em)
{
  _InitDisplay(em);
  _writeCommand(0x32);
  _writeData(LUT_DATA_full, sizeof(LUT_DATA_full));
  _PowerOn();
}

void GxGDEH0213B72::_Init_Part(uint8_t em)
{
  _InitDisplay(em);
  _writeCommand(0x2C); //VCOM Voltage
  _writeData(0x26);    // NA ??
  _writeCommand(0x32);
  _writeData(LUT_DATA_part, sizeof(LUT_DATA_part));
  _PowerOn();
}

void GxGDEH0213B72::_Update_Full(void)
{
  _writeCommand(0x22);
  _writeData(0xc4);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Full");
}

void GxGDEH0213B72::_Update_Part(void)
{
  _writeCommand(0x22);
  _writeData(0x04);
  _writeCommand(0x20);
  _waitWhileBusy("_Update_Part");
}

void GxGDEH0213B72::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback();
      for (int16_t y1 = 0; y1 < GxGDEH0213B72_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEH0213B72_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Full();
  }
  _PowerOff();
}

void GxGDEH0213B72::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      for (int16_t y1 = 0; y1 < GxGDEH0213B72_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEH0213B72_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Full();
  }
  _PowerOff();
}

void GxGDEH0213B72::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p);
      for (int16_t y1 = 0; y1 < GxGDEH0213B72_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEH0213B72_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Full();
  }
  _PowerOff();
}

void GxGDEH0213B72::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    _writeCommand(command);
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      fillScreen(GxEPD_WHITE);
      drawCallback(p1, p2);
      for (int16_t y1 = 0; y1 < GxGDEH0213B72_PAGE_HEIGHT; y1++)
      {
        for (int16_t x1 = 0; x1 < GxGDEH0213B72_WIDTH / 8; x1++)
        {
          uint16_t idx = y1 * (GxGDEH0213B72_WIDTH / 8) + x1;
          uint8_t data = (idx < sizeof(_buffer)) ? _buffer[idx] : 0x00;
          _writeData(~data);
        }
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Full();
  }
  _PowerOff();
}

void GxGDEH0213B72::_rotate(uint16_t& x, uint16_t& y, uint16_t& w, uint16_t& h)
{
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      swap(w, h);
      x = GxGDEH0213B72_WIDTH - x - w - 1;
      break;
    case 2:
      x = GxGDEH0213B72_WIDTH - x - w - 1;
      y = GxGDEH0213B72_HEIGHT - y - h - 1;
      break;
    case 3:
      swap(x, y);
      swap(w, h);
      y = GxGDEH0213B72_HEIGHT - y - h - 1;
      break;
  }
}

void GxGDEH0213B72::drawPagedToWindow(void (*drawCallback)(void), uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  _Init_Part(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEH0213B72_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEH0213B72_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback();
        uint16_t ys = yds % GxGDEH0213B72_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Part();
  }
}

void GxGDEH0213B72::drawPagedToWindow(void (*drawCallback)(uint32_t), uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  _Init_Part(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEH0213B72_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEH0213B72_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p);
        uint16_t ys = yds % GxGDEH0213B72_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Part();
  }
}

void GxGDEH0213B72::drawPagedToWindow(void (*drawCallback)(const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  _Init_Part(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEH0213B72_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEH0213B72_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p);
        uint16_t ys = yds % GxGDEH0213B72_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Part();
  }
}

void GxGDEH0213B72::drawPagedToWindow(void (*drawCallback)(const void*, const void*), uint16_t x, uint16_t y, uint16_t w, uint16_t h, const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _rotate(x, y, w, h);
  if (!_using_partial_mode)
  {
    eraseDisplay(false);
    eraseDisplay(true);
  }
  _using_partial_mode = true;
  _Init_Part(0x03);
  for (uint8_t command = 0x24; true; command = 0x26)
  { // leave both controller buffers equal
    for (_current_page = 0; _current_page < GxGDEH0213B72_PAGES; _current_page++)
    {
      uint16_t yds = gx_uint16_max(y, _current_page * GxGDEH0213B72_PAGE_HEIGHT);
      uint16_t yde = gx_uint16_min(y + h, (_current_page + 1) * GxGDEH0213B72_PAGE_HEIGHT);
      if (yde > yds)
      {
        fillScreen(GxEPD_WHITE);
        drawCallback(p1, p2);
        uint16_t ys = yds % GxGDEH0213B72_PAGE_HEIGHT;
        _writeToWindow(command, x, ys, x, yds, w, yde - yds);
      }
    }
    _current_page = -1;
    if (command == 0x26) break;
    _Update_Part();
  }
}

void GxGDEH0213B72::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _using_partial_mode = false;
  _Init_Full(em);
  _writeCommand(0x24);
  for (uint32_t y = 0; y < GxGDEH0213B72_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEH0213B72_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEH0213B72_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEH0213B72_WIDTH / 8 - 4) && (y > GxGDEH0213B72_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEH0213B72_HEIGHT - 33)) data = 0x00;
      _writeData(data);
    }
  }
  _Update_Full();
  _PowerOff();
}
