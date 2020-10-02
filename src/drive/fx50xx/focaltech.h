/////////////////////////////////////////////////////////////////
/*
MIT License

Copyright (c) 2019 lewis he

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

focaltech.h - Arduino library for focaltech chip , support FT5206,FT6236,FT5336,FT6436L,FT6436
Created by Lewis on April 17, 2019.
github:https://github.com/lewisxhe/FocalTech_Library
*/
/////////////////////////////////////////////////////////////////
#pragma once

#ifdef ARDUINO
#include <Arduino.h>
#include <Wire.h>
#else
#include <stdint.h>
#include <string.h>
#endif

#define FOCALTECH_SLAVE_ADDRESS    (0x38)
#define GT9XX_SLAVE_ADDRESS        (0x14)

typedef enum {
    FOCALTECH_NO_GESTRUE,
    FOCALTECH_MOVE_UP,
    FOCALTECH_MOVE_LEFT,
    FOCALTECH_MOVE_DOWN,
    FOCALTECH_MOVE_RIGHT,
    FOCALTECH_ZOOM_IN,
    FOCALTECH_ZOOM_OUT,
} GesTrue_t;

typedef enum {
    FOCALTECH_EVENT_PUT_DOWN,
    FOCALTECH_EVENT_PUT_UP,
    FOCALTECH_EVENT_CONTACT,
    FOCALTECH_EVENT_NONE,
} EventFlag_t;

typedef enum {
    FOCALTECH_PMODE_ACTIVE = 0,         // ~4mA
    FOCALTECH_PMODE_MONITOR = 1,        // ~3mA
    FOCALTECH_PMODE_DEEPSLEEP = 3,      // ~100uA  The reset pin must be pulled down to wake up
} PowerMode_t;

typedef uint8_t (*iic_com_fptr_t)(int dev_addr, uint16_t reg_addr, uint8_t *data, int len);
typedef uint8_t (*iic_com_fptr_u8_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);


class GT9xx_Class
{
public:
#ifdef ARDUINO
    bool begin(TwoWire &port = Wire, uint8_t addr = GT9XX_SLAVE_ADDRESS);
#endif
    bool begin(iic_com_fptr_t read_cb, iic_com_fptr_t write_cb, uint8_t addr = GT9XX_SLAVE_ADDRESS);

    void softReset(void);
    void setPins(int rst, int interrupt);

    uint8_t scanPoint();
    void  getPoint(uint16_t &x, uint16_t &y, uint8_t index);
private:

    typedef struct {
        uint16_t x;
        uint16_t y;
    } GT9xx_t;

    bool probe(void);

    uint8_t readRegister( uint16_t reg);
    void writeRegister( uint16_t reg, uint8_t value);
    bool readBytes( uint16_t reg, uint8_t *data, int nbytes);
    bool writeBytes( uint16_t reg, uint8_t *data, int nbytes);

    GT9xx_t data[5];
    uint8_t _address;
    bool initialization = false;
    iic_com_fptr_t _readCallbackFunc = nullptr;
    iic_com_fptr_t _writeCallbackFunc = nullptr;
    int _rst = -1;
    int _interrupt = -1;
#ifdef ARDUINO
    TwoWire *_i2cPort;
#endif
};



class FocalTech_Class
{
public:

#ifdef ARDUINO
    bool begin(TwoWire &port = Wire, uint8_t addr = FOCALTECH_SLAVE_ADDRESS);
#endif

    bool begin(iic_com_fptr_u8_t read_cb, iic_com_fptr_u8_t write_cb, uint8_t addr = FOCALTECH_SLAVE_ADDRESS);

    void    setTheshold(uint8_t value);
    uint8_t getThreshold(void);

    uint8_t getMonitorTime(void);
    void    setMonitorTime(uint8_t sec);
    uint8_t getActivePeriod(void);
    void    setActivePeriod(uint8_t rate);
    uint8_t getMonitorPeriod(void);
    void    setMonitorPeriod(uint8_t rate);

    void    enableAutoCalibration(void);
    void    disableAutoCalibration(void);

    void    getLibraryVersion(uint16_t &version);

    void    setPowerMode(PowerMode_t m);
    PowerMode_t    getPowerMode(void);

    uint8_t getVendorID(void);
    uint8_t getVendor1ID(void);
    uint8_t getErrorCode(void);

    void    enableINT(void);
    void    disableINT(void);
    uint8_t getINTMode(void);

    bool    getPoint(uint16_t &x, uint16_t &y);
    // bool    getPoint(uint8_t *x, uint8_t *y);

    uint8_t getTouched(void);

    uint8_t     getControl(void);
    uint8_t     getDeviceMode(void);
    GesTrue_t   getGesture(void);

    EventFlag_t event;
private:
    bool probe(void);

    uint8_t readRegister8( uint8_t reg);
    void writeRegister8( uint8_t reg, uint8_t value);
    bool readBytes( uint8_t reg, uint8_t *data, uint8_t nbytes);
    bool writeBytes( uint8_t reg, uint8_t *data, uint8_t nbytes);


    uint8_t _address;
    bool initialization = false;
    iic_com_fptr_u8_t _readCallbackFunc = nullptr;
    iic_com_fptr_u8_t _writeCallbackFunc = nullptr;

#ifdef ARDUINO
    TwoWire *_i2cPort;
#endif
};