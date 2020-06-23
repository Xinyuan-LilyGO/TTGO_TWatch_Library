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

FT5206.cpp - Arduino library for FT5206 chip.
Created by Lewis on April 17, 2019.
github:https://github.com/lewisxhe/FT5206_Library
*/
/////////////////////////////////////////////////////////////////
#include "FT5206.h"

void FT5206_Class::setType(uint8_t type)
{
    _type = type;
}

int FT5206_Class::begin(TwoWire &port, uint8_t addr)
{
    _i2cPort = &port;
    _address = addr;
    return dev_probe();
}

int FT5206_Class::begin(tp_com_fptr_t read_cb, tp_com_fptr_t write_cb, uint8_t addr )
{
    if (read_cb == nullptr || write_cb == nullptr) {
        return false;
    }
    _read_cb = read_cb;
    _write_cb = write_cb;
    _address = addr;
    return dev_probe();
}

int FT5206_Class::dev_probe()
{
#if 0
    if (version == LILYGO_TWATCH_FT62XX) {
        uint8_t val;
        _readByte(FT5206_VENDID_REG, 1, &val);
        if (val != FT5206_VENDID && val != FT5206_VENDID1) {
            return false;
        }
        _type = val;
        _readByte(FT5206_CHIPID_REG, 1, &val);
        if ((val != FT6206_CHIPID) && (val != FT6236_CHIPID) && (val != FT6236U_CHIPID) && (val != FT5206U_CHIPID)) {
            return false;
        }
        _init = true;
    } else if (version == LILYGO_TWATCH_CST026) {
        _i2cPort->beginTransmission(_address);
        _init = (0 == _i2cPort->endTransmission());
        _type = CST026_VENDID;
    }
#else
    uint8_t val;
    if (_read_cb == nullptr || _write_cb == nullptr) {
        _i2cPort->beginTransmission(_address);
        if (0 != _i2cPort->endTransmission()) {
            Serial.println("Not find device");
            return false;
        }
    }
    Serial.println("dev_probe==>");
    _readByte(FT5206_VENDID_REG, 1, &val);
    Serial.print("FT5206_VENDID_REG "); Serial.println(val, HEX);
    if (_type == 0xFF) {
        if (val == FT5206_VENDID || val == FT5206_VENDID1) {
            _type = val;
        } else {
            _type = CST026_VENDID;
        }
    }
    _init = true;
#endif
    return _init;
}

// valid touching detect threshold.
void FT5206_Class::adjustTheshold(uint8_t thresh)
{
    if (!_init)return;
    _writeByte(FT5206_THRESHHOLD_REG, 1, &thresh);
}

TP_Point FT5206_Class::getPoint(uint8_t num, uint8_t rotation)
{
    if (!_init) return TP_Point(0, 0);
    _readRegister();
    if ((_touches == 0) || (num > 1)) {
        return TP_Point(0, 0);
    } else {
        switch (_type) {
        case FT5X0X_VENDID:
            return TP_Point(_x[num], _y[num]);

        case FT5206_VENDID: {
            int16_t x = map(_x[num], 0, 320, 0, 240);
            int16_t y = map(_y[num], 0, 320, 0, 240);
            switch (rotation) {
            case 0:
                return TP_Point(x,  y);
            case 1:
                return TP_Point(y, 240 - x);
            case 2:
                return TP_Point(240 - x, 240 - y);
            case 3:
                return TP_Point( 240 - y, x);
            default:
                return TP_Point(x, y);
            }
        }
        case CST026_VENDID:
        case FT5206_VENDID1: {
            switch (rotation) {
            case 0:
                return TP_Point(240 - _x[num], 240 - _y[num]);
            case 1:
                return TP_Point(240 - _y[num], _x[num]);
            case 2:
                return TP_Point(_x[num], _y[num]);
            case 3:
                return TP_Point(_y[num], 240 - _x[num]);
            default:
                return TP_Point(_x[num], _y[num]);
            }
        }
        default:
            break;
        }
        return TP_Point(_x[num], _y[num]);
    }
}

uint8_t FT5206_Class::touched()
{
    if (!_init)return 0;
    uint8_t val = 0;
    _readByte(FT5206_TOUCHES_REG, 1, &val);
    return val > 2 ? 0 : val;
}

void FT5206_Class::enterSleepMode()
{
    if (!_init)return;
    uint8_t val = FT5206_SLEEP_IN;
    _writeByte(FT5206_POWER_REG, 1, &val);
}

void FT5206_Class::enterMonitorMode()
{
    if (!_init)return;
    uint8_t val = FT5206_MONITOR;
    _writeByte(FT5206_POWER_REG, 1, &val);
}

void FT5206_Class::_readRegister()
{
    _readByte(DEVIDE_MODE, 16, _data);
    _touches = _data[TD_STATUS];
    if ((_touches > 2) || (_touches == 0)) {
        _touches = 0;
        return;
    }
    for (uint8_t i = 0; i < 2; i++) {
        _x[i] = _data[TOUCH1_XH + i * 6] & 0x0F;
        _x[i] <<= 8;
        _x[i] |= _data[TOUCH1_XL + i * 6];
        _y[i] = _data[TOUCH1_YH + i * 6] & 0x0F;
        _y[i] <<= 8;
        _y[i] |= _data[TOUCH1_YL + i * 6];
        _id[i] = _data[TOUCH1_YH + i * 6] >> 4;
    }
}

uint8_t FT5206_Class::getType()
{
    return _type;
}


