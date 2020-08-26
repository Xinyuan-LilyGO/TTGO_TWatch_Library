//     ePaper Driver Lib for Arduino Project
//     Copyright (C) 2019 Michael Kamprath
//
//     This file is part of ePaper Driver Lib for Arduino Project.
//
//     ePaper Driver Lib for Arduino Project is free software: you can
//     redistribute it and/or modify it under the terms of the GNU General Public License
//     as published by the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
//
//     ePaper Driver Lib for Arduino Project is distributed in the hope that
//     it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
//
//     You should have received a copy of the GNU General Public License
//     along with Shift Register LED Matrix Project.  If not, see <http://www.gnu.org/licenses/>.
//
//     This project and its creators are not associated with Crystalfontz, Good display
//     or any other manufacturer, nor is this  project officially endorsed or reviewed for
//     correctness by any ePaper manufacturer.
//
#ifndef __ePaperDriver__
#define __ePaperDriver__
#include "../../Adafruit-GFX-Library/Adafruit_GFX.h"
#include "ePaperCanvas.h"
#include "ePaperDeviceModels.h"



class ePaperDisplay : public ePaperCanvas
{
public:


private:
    const ePaperDeviceModel _model;
    const int _deviceReadyPin;
    const int _deviceResetPin;
    const int _deviceDataCommandPin;
    const int _deviceSelectPin;

    const uint8_t *_configuration;
    const uint8_t _configurationSize;

    void (*_waitCallbackFunc)(void);

    void waitForReady(void) const;
    void resetDriver(void) const;

protected:
    void sendCommand( uint8_t cmd ) const;
    void sendData( const uint8_t *dataArray, uint16_t arraySize, bool isProgMem, bool invertBits = false ) const;
    void sendCommandAndDataSequenceFromProgMem( const uint8_t *dataArray, uint16_t arraySize) const;

    void initializeDevice(void) const;

public:

    ePaperDisplay(
        ePaperDeviceModel model,
        int deviceReadyPin,
        int deviceResetPin,
        int deviceDataCommandPin,
        int deviceSelectPin
    );

    virtual ~ePaperDisplay();

    ePaperDeviceModel model(void) const
    {
        return _model;
    }

    void setWaitCallBackFunction( void (*fp)(void) )
    {
        _waitCallbackFunc = fp;
    }

    //
    //
    //

    void refreshDisplay(void);
    void clearDisplay(void);

};

#endif // __ePaperDriver__
