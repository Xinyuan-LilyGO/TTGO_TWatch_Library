/*!
 * @file Adafruit_DRV2605.cpp
 *
 * @mainpage Adafruit DRV2605L Haptic Driver
 *
 * @section intro_sec Introduction
 *
 * This is a library for the Adafruit DRV2605L Haptic Driver ---->
 * http://www.adafruit.com/products/2305
 *
 * Check out the links above for our tutorials and wiring diagrams.
 *
 * This motor/haptic driver uses I2C to communicate.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * @section author Author
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * @section license License
 *
 * MIT license, all text above must be included in any redistribution.
 *
 */
/**************************************************************************/

#include "Arduino.h"
#include "Adafruit_DRV2605.h"
#include <Wire.h>

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
  @brief  Instantiates a new DRV2605 class. I2C, no address adjustments or pins
*/
/**************************************************************************/
Adafruit_DRV2605::Adafruit_DRV2605() {}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
  @brief Setup HW using a specified Wire
  @param theWire Pointer to a TwoWire object, defaults to &Wire
  @return Return value from init()
*/
/**************************************************************************/
bool Adafruit_DRV2605::begin(TwoWire *theWire)
{
    _wire = theWire;
    writeCallback = nullptr;
    readCallback  = nullptr;
    _wire->begin();
    return init();
}

/**
 * @brief Setup HW using a callback
 * @note
 * @param  r: iic read callback
 * @param  w: iic write callback
 * @retval Return value from init()
 */
bool Adafruit_DRV2605::begin(i2c_com_fptr_t r, i2c_com_fptr_t w)
{
    _wire = nullptr;
    writeCallback = w;
    readCallback = r;
    return init();
}
/**************************************************************************/
/*!
  @brief  Setup the HW
  @return Always true
*/
/**************************************************************************/
bool Adafruit_DRV2605::init()
{
    // uint8_t id = readRegister8(DRV2605_REG_STATUS);
    // Serial.print("Status 0x"); Serial.println(id, HEX);

    writeRegister8(DRV2605_REG_MODE, 0x00); // out of standby

    writeRegister8(DRV2605_REG_RTPIN, 0x00); // no real-time-playback

    writeRegister8(DRV2605_REG_WAVESEQ1, 1); // strong click
    writeRegister8(DRV2605_REG_WAVESEQ2, 0); // end sequence

    writeRegister8(DRV2605_REG_OVERDRIVE, 0); // no overdrive

    writeRegister8(DRV2605_REG_SUSTAINPOS, 0);
    writeRegister8(DRV2605_REG_SUSTAINNEG, 0);
    writeRegister8(DRV2605_REG_BREAK, 0);
    writeRegister8(DRV2605_REG_AUDIOMAX, 0x64);

    // ERM open loop

    // turn off N_ERM_LRA
    writeRegister8(DRV2605_REG_FEEDBACK,
                   readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
    // turn on ERM_OPEN_LOOP
    writeRegister8(DRV2605_REG_CONTROL3,
                   readRegister8(DRV2605_REG_CONTROL3) | 0x20);

    return true;
}

/**************************************************************************/
/*!
  @brief Select the haptic waveform to use.
  @param slot The waveform slot to set, from 0 to 7
  @param w The waveform sequence value, refers to an index in the ROM library.

    Playback starts at slot 0 and continues through to slot 7, stopping if it
  encounters a value of 0. A list of available waveforms can be found in
  section 11.2 of the datasheet: http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
void Adafruit_DRV2605::setWaveform(uint8_t slot, uint8_t w)
{
    writeRegister8(DRV2605_REG_WAVESEQ1 + slot, w);
}

/**************************************************************************/
/*!
  @brief Select the waveform library to use.
  @param lib Library to use, 0 = Empty, 1-5 are ERM, 6 is LRA.

    See section 7.6.4 in the datasheet for more details:
  http://www.adafruit.com/datasheets/DRV2605.pdf
*/
/**************************************************************************/
void Adafruit_DRV2605::selectLibrary(uint8_t lib)
{
    writeRegister8(DRV2605_REG_LIBRARY, lib);
}

/**************************************************************************/
/*!
  @brief Start playback of the waveforms (start moving!).
*/
/**************************************************************************/
void Adafruit_DRV2605::go()
{
    writeRegister8(DRV2605_REG_GO, 1);
}

/**************************************************************************/
/*!
  @brief Stop playback.
*/
/**************************************************************************/
void Adafruit_DRV2605::stop()
{
    writeRegister8(DRV2605_REG_GO, 0);
}

/**************************************************************************/
/*!
  @brief Set the device mode.
  @param mode Mode value, see datasheet section 7.6.2:
  http://www.adafruit.com/datasheets/DRV2605.pdf

    0: Internal trigger, call go() to start playback\n
    1: External trigger, rising edge on IN pin starts playback\n
    2: External trigger, playback follows the state of IN pin\n
    3: PWM/analog input\n
    4: Audio\n
    5: Real-time playback\n
    6: Diagnostics\n
    7: Auto calibration
*/
/**************************************************************************/
void Adafruit_DRV2605::setMode(uint8_t mode)
{
    writeRegister8(DRV2605_REG_MODE, mode);
}

/**************************************************************************/
/*!
  @brief Set the realtime value when in RTP mode, used to directly drive the
  haptic motor.
  @param rtp 8-bit drive value.
*/
/**************************************************************************/
void Adafruit_DRV2605::setRealtimeValue(uint8_t rtp)
{
    writeRegister8(DRV2605_REG_RTPIN, rtp);
}

/**************************************************************************/
/*!
  @brief Read an 8-bit register.
  @param reg The register to read.
  @return 8-bit value of the register.
*/
/**************************************************************************/
uint8_t Adafruit_DRV2605::readRegister8(uint8_t reg)
{
    uint8_t x = 0;

    if (readCallback) {
        readCallback(DRV2605_ADDR, reg, &x, 1);
        return x;
    }
    if (_wire) {
        // use i2c
        _wire->beginTransmission(DRV2605_ADDR);
        _wire->write((byte)reg);
        _wire->endTransmission();
        _wire->requestFrom((byte)DRV2605_ADDR, (byte)1);
        x = _wire->read();
    }
    //  Serial.print("$"); Serial.print(reg, HEX);
    //  Serial.print(": 0x"); Serial.println(x, HEX);

    return x;
}

/**************************************************************************/
/*!
  @brief Write an 8-bit register.
  @param reg The register to write.
  @param val The value to write.
*/
/**************************************************************************/
void Adafruit_DRV2605::writeRegister8(uint8_t reg, uint8_t val)
{
    if (writeCallback) {
        writeCallback(DRV2605_ADDR, reg, &val, 1);
        return;
    }
    if (_wire) {
        // use i2c
        _wire->beginTransmission(DRV2605_ADDR);
        _wire->write((byte)reg);
        _wire->write((byte)val);
        _wire->endTransmission();
    }
}

/**************************************************************************/
/*!
  @brief Use ERM (Eccentric Rotating Mass) mode.
*/
/**************************************************************************/
void Adafruit_DRV2605::useERM()
{
    writeRegister8(DRV2605_REG_FEEDBACK,
                   readRegister8(DRV2605_REG_FEEDBACK) & 0x7F);
}

/**************************************************************************/
/*!
  @brief Use LRA (Linear Resonance Actuator) mode.
*/
/**************************************************************************/
void Adafruit_DRV2605::useLRA()
{
    writeRegister8(DRV2605_REG_FEEDBACK,
                   readRegister8(DRV2605_REG_FEEDBACK) | 0x80);
}
