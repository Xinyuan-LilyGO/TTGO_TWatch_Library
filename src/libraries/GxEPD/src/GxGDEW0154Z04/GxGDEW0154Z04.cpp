// class GxGDEW0154Z04 : Display class for GDEW0154Z04 e-Paper from Dalian Good Display Co., Ltd.: www.e-paper-display.com
//
// based on Demo Example from Good Display, available here: http://www.e-paper-display.com/download_detail/downloadsId=515.html
// Controller: IL0376F : http://www.e-paper-display.com/download_detail/downloadsId=541.html
//
// Author : J-M Zingg
//
// Version : see library.properties
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// Library: https://github.com/ZinggJM/GxEPD

#include "GxGDEW0154Z04.h"

//#define DISABLE_DIAGNOSTIC_OUTPUT

#if defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

const uint8_t GxGDEW0154Z04::lut_vcom0[] PROGMEM = {  0x0E  , 0x14 , 0x01 , 0x0A , 0x06 , 0x04 , 0x0A , 0x0A , 0x0F , 0x03 , 0x03 , 0x0C , 0x06 , 0x0A , 0x00 };
const uint8_t GxGDEW0154Z04::lut_w[] PROGMEM = {  0x0E  , 0x14 , 0x01 , 0x0A , 0x46 , 0x04 , 0x8A , 0x4A , 0x0F , 0x83 , 0x43 , 0x0C , 0x86 , 0x0A , 0x04 };
const uint8_t GxGDEW0154Z04::lut_b[] PROGMEM = {  0x0E  , 0x14 , 0x01 , 0x8A , 0x06 , 0x04 , 0x8A , 0x4A , 0x0F , 0x83 , 0x43 , 0x0C , 0x06 , 0x4A , 0x04 };
const uint8_t GxGDEW0154Z04::lut_g1[] PROGMEM = { 0x8E  , 0x94 , 0x01 , 0x8A , 0x06 , 0x04 , 0x8A , 0x4A , 0x0F , 0x83 , 0x43 , 0x0C , 0x06 , 0x0A , 0x04 };
const uint8_t GxGDEW0154Z04::lut_g2[] PROGMEM = { 0x8E  , 0x94 , 0x01 , 0x8A , 0x06 , 0x04 , 0x8A , 0x4A , 0x0F , 0x83 , 0x43 , 0x0C , 0x06 , 0x0A , 0x04 };
const uint8_t GxGDEW0154Z04::lut_vcom1[] PROGMEM = {  0x03  , 0x1D , 0x01 , 0x01 , 0x08 , 0x23 , 0x37 , 0x37 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
const uint8_t GxGDEW0154Z04::lut_red0[] PROGMEM = { 0x83  , 0x5D , 0x01 , 0x81 , 0x48 , 0x23 , 0x77 , 0x77 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };
const uint8_t GxGDEW0154Z04::lut_red1[] PROGMEM = { 0x03  , 0x1D , 0x01 , 0x01 , 0x08 , 0x23 , 0x37 , 0x37 , 0x01 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 };

const uint8_t GxGDEW0154Z04::bw2grey[] =
{
  0b00000000, 0b00000011, 0b00001100, 0b00001111,
  0b00110000, 0b00110011, 0b00111100, 0b00111111,
  0b11000000, 0b11000011, 0b11001100, 0b11001111,
  0b11110000, 0b11110011, 0b11111100, 0b11111111,
};

GxGDEW0154Z04::GxGDEW0154Z04(GxIO& io, int8_t rst, int8_t busy)
  : GxEPD(GxGDEW0154Z04_WIDTH, GxGDEW0154Z04_HEIGHT), IO(io),
  _current_page(-1), _diag_enabled(false),
  _rst(rst), _busy(busy) 
{
}

void GxGDEW0154Z04::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  // check rotation, move pixel around if necessary
  switch (getRotation())
  {
    case 1:
      swap(x, y);
      x = GxGDEW0154Z04_WIDTH - x - 1;
      break;
    case 2:
      x = GxGDEW0154Z04_WIDTH - x - 1;
      y = GxGDEW0154Z04_HEIGHT - y - 1;
      break;
    case 3:
      swap(x, y);
      y = GxGDEW0154Z04_HEIGHT - y - 1;
      break;
  }
  uint16_t i = x / 8 + y * GxGDEW0154Z04_WIDTH / 8;
  if (_current_page < 1)
  {
    if (i >= sizeof(_black_buffer)) return;
  }
  else
  {
    if (i < GxGDEW0154Z04_PAGE_SIZE * _current_page) return;
    if (i >= GxGDEW0154Z04_PAGE_SIZE * (_current_page + 1)) return;
    i -= GxGDEW0154Z04_PAGE_SIZE * _current_page;
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


void GxGDEW0154Z04::init(uint32_t serial_diag_bitrate)
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
}

void GxGDEW0154Z04::fillScreen(uint16_t color)
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

void GxGDEW0154Z04::update(void)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
    uint8_t data = (i < sizeof(_black_buffer)) ? ~_black_buffer[i] : 0xFF;
    _writeData(bw2grey[(data & 0xF0) >> 4]);
    _writeData(bw2grey[data & 0x0F]);
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
    _writeData((i < sizeof(_red_buffer)) ? ~_red_buffer[i] : 0xFF);
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("update");
  _sleep();
}

void  GxGDEW0154Z04::drawBitmap(const uint8_t *bitmap, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color, int16_t mode)
{
  if (mode & bm_default) mode |= bm_normal; // no change
  drawBitmapBM(bitmap, x, y, w, h, color, mode);
}

void GxGDEW0154Z04::drawExamplePicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE * 2; i++)
  {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
    _writeData((i < black_size) ? pgm_read_byte(&black_bitmap[i]) : 0x00);
#else
    _writeData((i < black_size) ? black_bitmap[i] : 0x00);
#endif
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
    _writeData((i < red_size) ? pgm_read_byte(&red_bitmap[i]) : 0x00);
#else
    _writeData((i < red_size) ? red_bitmap[i] : 0x00);
#endif
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPicture");
  _sleep();
}

void GxGDEW0154Z04::drawPicture(const uint8_t* black_bitmap, const uint8_t* red_bitmap, uint32_t black_size, uint32_t red_size, int16_t mode)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
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
    _writeData(bw2grey[(data & 0xF0) >> 4]);
    _writeData(bw2grey[data & 0x0F]);
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
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

void GxGDEW0154Z04::drawBitmap(const uint8_t* bitmap, uint32_t size, int16_t mode)
{
  if (_current_page != -1) return;
  if (mode & bm_default) mode |= bm_normal; // no change
  uint8_t mask = 0xFF; // black
  //uint8_t mask = 0b0101010101010101; // (light) grey
  //uint8_t mask = 0b1010101010101010; // (dark) grey, same grey
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
#if defined(__AVR) || defined(ESP8266) || defined(ESP32)
    uint8_t data = (i < size) ? pgm_read_byte(&bitmap[i]) : 0x00;
#else
    uint8_t data = (i < size) ? bitmap[i] : 0x00;
#endif
    if (mode & bm_invert) data = ~data;
    _writeData(~(bw2grey[(data & 0xF0) >> 4] & mask));
    _writeData(~(bw2grey[data & 0x0F] & mask));
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
    _writeData(0xFF);
  }
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawBitmap");
  _sleep();
}

void GxGDEW0154Z04::eraseDisplay(bool using_partial_update)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE * 2; i++)
  {
    _writeData(0xFF); // white is 0xFF on device
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
    _writeData(0xFF); // white is 0xFF on device
  }
  _writeCommand(0x12);      //display refresh
  _waitWhileBusy("eraseDisplay");
  _sleep();
}

void GxGDEW0154Z04::powerDown()
{
  _sleep();
}

void GxGDEW0154Z04::_writeCommand(uint8_t command)
{
  IO.writeCommandTransaction(command);
}

void GxGDEW0154Z04::_writeData(uint8_t data)
{
  IO.writeDataTransaction(data);
}

void GxGDEW0154Z04::_waitWhileBusy(const char* comment)
{
  unsigned long start = micros();
  while (1)
  {
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

void GxGDEW0154Z04::_wakeUp()
{
  // reset required for wakeup
  if (_rst >= 0)
  {
    digitalWrite(_rst, 0);
    delay(10);
    digitalWrite(_rst, 1);
    delay(10);
  }

  _writeCommand(0x01);
  _writeData(0x07);
  _writeData(0x00);
  _writeData(0x08);
  _writeData(0x00);
  _writeCommand(0x06);
  _writeData(0x07);
  _writeData(0x07);
  _writeData(0x07);
  _writeCommand(0x04);
  _waitWhileBusy("Power On");
  _writeCommand(0X00);
  _writeData(0xcf);
  _writeCommand(0X50);
  _writeData(0x37);
  _writeCommand(0x30);
  _writeData(0x39);
  _writeCommand(0x61);
  _writeData(0xC8);
  _writeData(0x00);
  _writeData(0xC8);
  _writeCommand(0x82);
  _writeData(0x0E);
  _writeLUT();
}

void GxGDEW0154Z04::_sleep(void)
{
  _writeCommand(0X50);
  _writeData(0x17);    //BD floating
  _writeCommand(0x82);     //to solve Vcom drop
  _writeData(0x00);
  _writeCommand(0x01);     //power setting
  _writeData(0x02);    //gate switch to external
  _writeData(0x00);
  _writeData(0x00);
  _writeData(0x00);
  delay(1500);     //delay 1.5S
  _writeCommand(0X02);     //power off
}

#if defined(__AVR) || defined(ESP8266) || defined(ESP32)

void GxGDEW0154Z04::_writeLUT(void)
{
  unsigned int count;
  // lut_bw
  _writeCommand(0x20);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_vcom0[count]));
  }

  _writeCommand(0x21);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_w[count]));
  }

  _writeCommand(0x22);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_b[count]));
  }

  _writeCommand(0x23);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_g1[count]));
  }

  _writeCommand(0x24);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_g2[count]));
  }
  // lut_red
  _writeCommand(0x25);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_vcom1[count]));
  }

  _writeCommand(0x26);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_red0[count]));
  }

  _writeCommand(0x27);
  for (count = 0; count < 15; count++)
  {
    _writeData(pgm_read_byte(&lut_red1[count]));
  }
}

#else

void GxGDEW0154Z04::_writeLUT(void)
{
  unsigned int count;
  // lut_bw
  _writeCommand(0x20);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_vcom0[count]);
  }

  _writeCommand(0x21);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_w[count]);
  }

  _writeCommand(0x22);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_b[count]);
  }

  _writeCommand(0x23);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_g1[count]);
  }

  _writeCommand(0x24);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_g2[count]);
  }
  // lut_red
  _writeCommand(0x25);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_vcom1[count]);
  }

  _writeCommand(0x26);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_red0[count]);
  }

  _writeCommand(0x27);
  for (count = 0; count < 15; count++)
  {
    _writeData(lut_red1[count]);
  }
}

#endif

void GxGDEW0154Z04::drawPaged(void (*drawCallback)(void))
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~bw2grey[(data & 0xF0) >> 4]);
        _writeData(~bw2grey[data & 0x0F]);
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback();
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data);
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW0154Z04::drawPaged(void (*drawCallback)(uint32_t), uint32_t p)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~bw2grey[(data & 0xF0) >> 4]);
        _writeData(~bw2grey[data & 0x0F]);
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data);
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW0154Z04::drawPaged(void (*drawCallback)(const void*), const void* p)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~bw2grey[(data & 0xF0) >> 4]);
        _writeData(~bw2grey[data & 0x0F]);
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data);
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW0154Z04::drawPaged(void (*drawCallback)(const void*, const void*), const void* p1, const void* p2)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_black_buffer)) ? _black_buffer[idx] : 0x00;
        _writeData(~bw2grey[(data & 0xF0) >> 4]);
        _writeData(~bw2grey[data & 0x0F]);
      }
    }
  }
  _writeCommand(0x13);
  for (_current_page = 0; _current_page < GxGDEW0154Z04_PAGES; _current_page++)
  {
    fillScreen(GxEPD_WHITE);
    drawCallback(p1, p2);
    for (int16_t y1 = 0; y1 < GxGDEW0154Z04_PAGE_HEIGHT; y1++)
    {
      for (int16_t x1 = 0; x1 < GxGDEW0154Z04_WIDTH / 8; x1++)
      {
        uint16_t idx = y1 * (GxGDEW0154Z04_WIDTH / 8) + x1;
        uint8_t data = (idx < sizeof(_red_buffer)) ? _red_buffer[idx] : 0x00;
        _writeData(~data);
      }
    }
  }
  _current_page = -1;
  _writeCommand(0x12); //display refresh
  _waitWhileBusy("drawPaged");
  _sleep();
}

void GxGDEW0154Z04::drawCornerTest(uint8_t em)
{
  if (_current_page != -1) return;
  _wakeUp();
  _writeCommand(0x10);
  for (uint32_t y = 0; y < GxGDEW0154Z04_HEIGHT; y++)
  {
    for (uint32_t x = 0; x < GxGDEW0154Z04_WIDTH / 8; x++)
    {
      uint8_t data = 0xFF;
      if ((x < 1) && (y < 8)) data = 0x00;
      if ((x > GxGDEW0154Z04_WIDTH / 8 - 3) && (y < 16)) data = 0x00;
      if ((x > GxGDEW0154Z04_WIDTH / 8 - 4) && (y > GxGDEW0154Z04_HEIGHT - 25)) data = 0x00;
      if ((x < 4) && (y > GxGDEW0154Z04_HEIGHT - 33)) data = 0x00;
      _writeData(data);
      _writeData(data);
    }
  }
  _writeCommand(0x13);
  for (uint32_t i = 0; i < GxGDEW0154Z04_BUFFER_SIZE; i++)
  {
    _writeData(0xFF);
  }
  _writeCommand(0x12);      //display refresh
  _waitWhileBusy("drawCornerTest");
  _sleep();
}
