// created by Jean-Marc Zingg to be the GxIO_SPI io class for the GxEPD library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GxIO_SPI.h"

#if defined(PARTICLE)
// transfer16 is not used by GxEPD
#define transfer16 transfer
#endif

GxIO_SPI::GxIO_SPI(SPIClass& spi, int8_t cs, int8_t dc, int8_t rst, int8_t bl) :
  _spi(spi), _spi_settings(4000000, MSBFIRST, SPI_MODE0),
  _cs(cs), _dc(dc), _rst(rst), _bl(bl)
{
}

void GxIO_SPI::reset()
{
  if (_rst >= 0)
  {
    delay(20);
    digitalWrite(_rst, LOW);
    delay(20);
    digitalWrite(_rst, HIGH);
    delay(200);
  }
}

void GxIO_SPI::init()
{
  if (_cs >= 0)
  {
    digitalWrite(_cs, HIGH);
    pinMode(_cs, OUTPUT);
  }
  if (_dc >= 0)
  {
    digitalWrite(_dc, HIGH);
    pinMode(_dc, OUTPUT);
  }
  if (_rst >= 0)
  {
    digitalWrite(_rst, HIGH);
    pinMode(_rst, OUTPUT);
  }
  if (_bl >= 0)
  {
    digitalWrite(_bl, HIGH);
    pinMode(_bl, OUTPUT);
  }
  reset();
  _spi.begin();
}

void GxIO_SPI::setFrequency(uint32_t freq)
{
  _spi_settings = SPISettings(freq, MSBFIRST, SPI_MODE0);
}

uint8_t GxIO_SPI::transferTransaction(uint8_t d)
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = _spi.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
  return rv;
}

uint16_t GxIO_SPI::transfer16Transaction(uint16_t d)
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = _spi.transfer16(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
  return rv;
}

uint8_t GxIO_SPI::readDataTransaction()
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint8_t rv = _spi.transfer(0xFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
  return rv;
}

uint16_t GxIO_SPI::readData16Transaction()
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  uint16_t rv = _spi.transfer16(0xFFFF);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
  return rv;
}

uint8_t GxIO_SPI::readData()
{
  uint8_t rv = _spi.transfer(0xFF);
  return rv;
}

uint16_t GxIO_SPI::readData16()
{
  uint16_t rv = _spi.transfer16(0xFFFF);
  return rv;
}

void GxIO_SPI::writeCommandTransaction(uint8_t c)
{
  _spi.beginTransaction(_spi_settings);
  if (_dc >= 0) digitalWrite(_dc, LOW);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _spi.transfer(c);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
  _spi.endTransaction();
}

void GxIO_SPI::writeDataTransaction(uint8_t d)
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  _spi.transfer(d);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
}

void GxIO_SPI::writeData16Transaction(uint16_t d, uint32_t num)
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
  writeData16(d, num);
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
}

void GxIO_SPI::writeCommand(uint8_t c)
{
  if (_dc >= 0) digitalWrite(_dc, LOW);
  _spi.transfer(c);
  if (_dc >= 0) digitalWrite(_dc, HIGH);
}

void GxIO_SPI::writeData(uint8_t d)
{
  _spi.transfer(d);
}

void GxIO_SPI::writeData(uint8_t* d, uint32_t num)
{
#if defined(ESP8266) || defined(ESP32)
  _spi.writeBytes(d, num);
#else
  while (num > 0)
  {
    _spi.transfer(*d);
    d++;
    num--;
  }
#endif
}

void GxIO_SPI::writeData16(uint16_t d, uint32_t num)
{
#if defined(ESP8266) || defined(ESP32)
  uint8_t b[2] = {uint8_t(d >> 8), uint8_t(d)};
  _spi.writePattern(b, 2, num);
#else
  while (num > 0)
  {
    _spi.transfer16(d);
    num--;
  }
#endif
}

void GxIO_SPI::writeAddrMSBfirst(uint16_t d)
{
  _spi.transfer(d >> 8);
  _spi.transfer(d & 0xFF);
}

void GxIO_SPI::startTransaction()
{
  _spi.beginTransaction(_spi_settings);
  if (_cs >= 0) digitalWrite(_cs, LOW);
}

void GxIO_SPI::endTransaction()
{
  if (_cs >= 0) digitalWrite(_cs, HIGH);
  _spi.endTransaction();
}

void GxIO_SPI::selectRegister(bool rs_low)
{
  if (_dc >= 0) digitalWrite(_dc, (rs_low ? LOW : HIGH));
}

void GxIO_SPI::setBackLight(bool lit)
{
  if (_bl >= 0) digitalWrite(_bl, (lit ? HIGH : LOW));
}
