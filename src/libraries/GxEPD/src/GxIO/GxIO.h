// created by Jean-Marc Zingg to be the GxIO io base class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#ifndef _GxIO_H_
#define _GxIO_H_

#include <Arduino.h>

class GxIO
{
  public:
    GxIO() {};
    const char* name = "GxIO";
    virtual void reset();
    virtual void init();
    virtual void setFrequency(uint32_t freq){}; // for SPI
    virtual void setClockDivider(uint32_t clockDiv){}; // for SPI
    virtual uint8_t transferTransaction(uint8_t d);
    virtual uint16_t transfer16Transaction(uint16_t d);
    virtual uint8_t readDataTransaction();
    virtual uint16_t readData16Transaction();
    virtual uint8_t readData();
    virtual uint16_t readData16();
    virtual uint32_t readRawData32(uint8_t part); // debug purpose
    virtual void writeCommandTransaction(uint8_t c);
    virtual void writeDataTransaction(uint8_t d);
    virtual void writeData16Transaction(uint16_t d, uint32_t num = 1);
    virtual void writeCommand(uint8_t c);
    virtual void writeData(uint8_t d);
    virtual void writeData(uint8_t* d, uint32_t num);
    virtual void writeData16(uint16_t d, uint32_t num = 1);
    virtual void writeAddrMSBfirst(uint16_t d);
    virtual void startTransaction();
    virtual void endTransaction();
    virtual void selectRegister(bool rs_low) {}; // for generalized readData & writeData (RA8875)
    virtual void setBackLight(bool lit);
};

#endif

