/*
MPU6050.cpp - Class file for the MPU6050 Triple Axis Gyroscope & Accelerometer Arduino Library.

Version: 1.0.3
(c) 2014-2015 Korneliusz Jarzebski
www.jarzebski.pl

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Last change:  July 27, 2020 by Lewis He
*/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>
#include <math.h>

#include "MPU6050.h"

bool MPU6050::begin(imu_com_fptr_t read_callblack, imu_com_fptr_t write_callblack, mpu6050_dps_t scale, mpu6050_range_t range, int mpua)
{
    // Set Address
    mpuAddress = mpua;
    if (read_callblack == nullptr || write_callblack == nullptr) {
        return false;
    }

    __read = read_callblack;
    __write = write_callblack;


#ifdef DISABLE_I2C_CALLBLACK
    Wire.begin();
#endif


    // Reset calibrate values
    dg.XAxis = 0;
    dg.YAxis = 0;
    dg.ZAxis = 0;
    useCalibrate = false;

    // Reset threshold values
    tg.XAxis = 0;
    tg.YAxis = 0;
    tg.ZAxis = 0;
    actualThreshold = 0;

    // Check MPU6050 Who Am I Register
    if (fastRegister8(MPU6050_REG_WHO_AM_I) != 0x68) {
        return false;
    }

    // Set Clock Source
    setClockSource(MPU6050_CLOCK_PLL_XGYRO);

    // Set Scale & Range
    setScale(scale);
    setRange(range);

    // Disable Sleep Mode
    setSleepEnabled(false);

    return true;
}

void MPU6050::setScale(mpu6050_dps_t scale)
{
    uint8_t value;

    switch (scale) {
    case MPU6050_SCALE_250DPS:
        dpsPerDigit = .007633f;
        break;
    case MPU6050_SCALE_500DPS:
        dpsPerDigit = .015267f;
        break;
    case MPU6050_SCALE_1000DPS:
        dpsPerDigit = .030487f;
        break;
    case MPU6050_SCALE_2000DPS:
        dpsPerDigit = .060975f;
        break;
    default:
        break;
    }

    value = readRegister8(MPU6050_REG_GYRO_CONFIG);
    value &= 0b11100111;
    value |= (scale << 3);
    writeRegister8(MPU6050_REG_GYRO_CONFIG, value);
}

mpu6050_dps_t MPU6050::getScale(void)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_GYRO_CONFIG);
    value &= 0b00011000;
    value >>= 3;
    return (mpu6050_dps_t)value;
}

void MPU6050::setRange(mpu6050_range_t range)
{
    uint8_t value;

    switch (range) {
    case MPU6050_RANGE_2G:
        rangePerDigit = .000061f;
        break;
    case MPU6050_RANGE_4G:
        rangePerDigit = .000122f;
        break;
    case MPU6050_RANGE_8G:
        rangePerDigit = .000244f;
        break;
    case MPU6050_RANGE_16G:
        rangePerDigit = .0004882f;
        break;
    default:
        break;
    }

    value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b11100111;
    value |= (range << 3);
    writeRegister8(MPU6050_REG_ACCEL_CONFIG, value);
}

mpu6050_range_t MPU6050::getRange(void)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b00011000;
    value >>= 3;
    return (mpu6050_range_t)value;
}

void MPU6050::setDHPFMode(mpu6050_dhpf_t dhpf)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_ACCEL_CONFIG);
    value &= 0b11111000;
    value |= dhpf;
    writeRegister8(MPU6050_REG_ACCEL_CONFIG, value);
}

void MPU6050::setDLPFMode(mpu6050_dlpf_t dlpf)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_CONFIG);
    value &= 0b11111000;
    value |= dlpf;
    writeRegister8(MPU6050_REG_CONFIG, value);
}

void MPU6050::setClockSource(mpu6050_clockSource_t source)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_PWR_MGMT_1);
    value &= 0b11111000;
    value |= source;
    writeRegister8(MPU6050_REG_PWR_MGMT_1, value);
}

mpu6050_clockSource_t MPU6050::getClockSource(void)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_PWR_MGMT_1);
    value &= 0b00000111;
    return (mpu6050_clockSource_t)value;
}

bool MPU6050::getSleepEnabled(void)
{
    return readRegisterBit(MPU6050_REG_PWR_MGMT_1, 6);
}

void MPU6050::setSleepEnabled(bool state)
{
    writeRegisterBit(MPU6050_REG_PWR_MGMT_1, 6, state);
}

bool MPU6050::getIntZeroMotionEnabled(void)
{
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 5);
}

void MPU6050::setIntZeroMotionEnabled(bool state)
{
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 5, state);
}

bool MPU6050::getIntMotionEnabled(void)
{
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 6);
}

void MPU6050::setIntMotionEnabled(bool state)
{
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 6, state);
}

bool MPU6050::getIntFreeFallEnabled(void)
{
    return readRegisterBit(MPU6050_REG_INT_ENABLE, 7);
}

void MPU6050::setIntFreeFallEnabled(bool state)
{
    writeRegisterBit(MPU6050_REG_INT_ENABLE, 7, state);
}

uint8_t MPU6050::getMotionDetectionThreshold(void)
{
    return readRegister8(MPU6050_REG_MOT_THRESHOLD);
}

void MPU6050::setMotionDetectionThreshold(uint8_t threshold)
{
    writeRegister8(MPU6050_REG_MOT_THRESHOLD, threshold);
}

uint8_t MPU6050::getMotionDetectionDuration(void)
{
    return readRegister8(MPU6050_REG_MOT_DURATION);
}

void MPU6050::setMotionDetectionDuration(uint8_t duration)
{
    writeRegister8(MPU6050_REG_MOT_DURATION, duration);
}

uint8_t MPU6050::getZeroMotionDetectionThreshold(void)
{
    return readRegister8(MPU6050_REG_ZMOT_THRESHOLD);
}

void MPU6050::setZeroMotionDetectionThreshold(uint8_t threshold)
{
    writeRegister8(MPU6050_REG_ZMOT_THRESHOLD, threshold);
}

uint8_t MPU6050::getZeroMotionDetectionDuration(void)
{
    return readRegister8(MPU6050_REG_ZMOT_DURATION);
}

void MPU6050::setZeroMotionDetectionDuration(uint8_t duration)
{
    writeRegister8(MPU6050_REG_ZMOT_DURATION, duration);
}

uint8_t MPU6050::getFreeFallDetectionThreshold(void)
{
    return readRegister8(MPU6050_REG_FF_THRESHOLD);
}

void MPU6050::setFreeFallDetectionThreshold(uint8_t threshold)
{
    writeRegister8(MPU6050_REG_FF_THRESHOLD, threshold);
}

uint8_t MPU6050::getFreeFallDetectionDuration(void)
{
    return readRegister8(MPU6050_REG_FF_DURATION);
}

void MPU6050::setFreeFallDetectionDuration(uint8_t duration)
{
    writeRegister8(MPU6050_REG_FF_DURATION, duration);
}

bool MPU6050::getI2CMasterModeEnabled(void)
{
    return readRegisterBit(MPU6050_REG_USER_CTRL, 5);
}

void MPU6050::setI2CMasterModeEnabled(bool state)
{
    writeRegisterBit(MPU6050_REG_USER_CTRL, 5, state);
}

void MPU6050::setI2CBypassEnabled(bool state)
{
    return writeRegisterBit(MPU6050_REG_INT_PIN_CFG, 1, state);
}

bool MPU6050::getI2CBypassEnabled(void)
{
    return readRegisterBit(MPU6050_REG_INT_PIN_CFG, 1);
}

void MPU6050::setAccelPowerOnDelay(mpu6050_onDelay_t delay)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_MOT_DETECT_CTRL);
    value &= 0b11001111;
    value |= (delay << 4);
    writeRegister8(MPU6050_REG_MOT_DETECT_CTRL, value);
}

mpu6050_onDelay_t MPU6050::getAccelPowerOnDelay(void)
{
    uint8_t value;
    value = readRegister8(MPU6050_REG_MOT_DETECT_CTRL);
    value &= 0b00110000;
    return (mpu6050_onDelay_t)(value >> 4);
}

uint8_t MPU6050::getIntStatus(void)
{
    return readRegister8(MPU6050_REG_INT_STATUS);
}

Activites MPU6050::readActivites(void)
{
    uint8_t data = readRegister8(MPU6050_REG_INT_STATUS);

    a.isOverflow = ((data >> 4) & 1);
    a.isFreeFall = ((data >> 7) & 1);
    a.isInactivity = ((data >> 5) & 1);
    a.isActivity = ((data >> 6) & 1);
    a.isDataReady = ((data >> 0) & 1);

    data = readRegister8(MPU6050_REG_MOT_DETECT_STATUS);

    a.isNegActivityOnX = ((data >> 7) & 1);
    a.isPosActivityOnX = ((data >> 6) & 1);

    a.isNegActivityOnY = ((data >> 5) & 1);
    a.isPosActivityOnY = ((data >> 4) & 1);

    a.isNegActivityOnZ = ((data >> 3) & 1);
    a.isPosActivityOnZ = ((data >> 2) & 1);

    return a;
}

Vector MPU6050::readRawAccel(void)
{
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(MPU6050_REG_ACCEL_XOUT_H);
#else
    Wire.send(MPU6050_REG_ACCEL_XOUT_H);
#endif
    Wire.endTransmission();

    Wire.requestFrom(mpuAddress, 6);

    while (Wire.available() < 6);

#if ARDUINO >= 100
    uint8_t xha = Wire.read();
    uint8_t xla = Wire.read();
    uint8_t yha = Wire.read();
    uint8_t yla = Wire.read();
    uint8_t zha = Wire.read();
    uint8_t zla = Wire.read();
#else
    uint8_t xha = Wire.receive();
    uint8_t xla = Wire.receive();
    uint8_t yha = Wire.receive();
    uint8_t yla = Wire.receive();
    uint8_t zha = Wire.receive();
    uint8_t zla = Wire.receive();
#endif

    ra.XAxis = (int16_t)(xha << 8 | xla);
    ra.YAxis = (int16_t)(yha << 8 | yla);
    ra.ZAxis = (int16_t)(zha << 8 | zla);
#else
    uint8_t data[6];
    __read(mpuAddress, MPU6050_REG_ACCEL_XOUT_H, data, 6);
    ra.XAxis = (int16_t)(data[0] << 8 | data[1]);
    ra.YAxis = (int16_t)(data[2] << 8 | data[3]);
    ra.ZAxis = (int16_t)(data[4] << 8 | data[5]);
#endif
    return ra;
}

Vector MPU6050::readNormalizeAccel(void)
{
    readRawAccel();

    na.XAxis = ra.XAxis * rangePerDigit * 9.80665f;
    na.YAxis = ra.YAxis * rangePerDigit * 9.80665f;
    na.ZAxis = ra.ZAxis * rangePerDigit * 9.80665f;

    return na;
}

Vector MPU6050::readScaledAccel(void)
{
    readRawAccel();

    na.XAxis = ra.XAxis * rangePerDigit;
    na.YAxis = ra.YAxis * rangePerDigit;
    na.ZAxis = ra.ZAxis * rangePerDigit;

    return na;
}


Vector MPU6050::readRawGyro(void)
{
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(MPU6050_REG_GYRO_XOUT_H);
#else
    Wire.send(MPU6050_REG_GYRO_XOUT_H);
#endif
    Wire.endTransmission();

    Wire.requestFrom(mpuAddress, 6);

    while (Wire.available() < 6);

#if ARDUINO >= 100
    uint8_t xha = Wire.read();
    uint8_t xla = Wire.read();
    uint8_t yha = Wire.read();
    uint8_t yla = Wire.read();
    uint8_t zha = Wire.read();
    uint8_t zla = Wire.read();
#else
    uint8_t xha = Wire.receive();
    uint8_t xla = Wire.receive();
    uint8_t yha = Wire.receive();
    uint8_t yla = Wire.receive();
    uint8_t zha = Wire.receive();
    uint8_t zla = Wire.receive();
#endif
    rg.XAxis = (int16_t)(xha << 8 | xla);
    rg.YAxis = (int16_t)(yha << 8 | yla);
    rg.ZAxis = (int16_t)(zha << 8 | zla);
#else
    uint8_t data[6];
    __read(mpuAddress, MPU6050_REG_GYRO_XOUT_H, data, 6);
    rg.XAxis = (int16_t)(data[0] << 8 | data[1]);
    rg.YAxis = (int16_t)(data[2] << 8 | data[3]);
    rg.ZAxis = (int16_t)(data[4] << 8 | data[5]);
#endif  /*DISABLE_I2C_CALLBLACK*/
    return rg;
}

Vector MPU6050::readNormalizeGyro(void)
{
    readRawGyro();

    if (useCalibrate) {
        ng.XAxis = (rg.XAxis - dg.XAxis) * dpsPerDigit;
        ng.YAxis = (rg.YAxis - dg.YAxis) * dpsPerDigit;
        ng.ZAxis = (rg.ZAxis - dg.ZAxis) * dpsPerDigit;
    } else {
        ng.XAxis = rg.XAxis * dpsPerDigit;
        ng.YAxis = rg.YAxis * dpsPerDigit;
        ng.ZAxis = rg.ZAxis * dpsPerDigit;
    }

    if (actualThreshold) {
        if (abs(ng.XAxis) < tg.XAxis) ng.XAxis = 0;
        if (abs(ng.YAxis) < tg.YAxis) ng.YAxis = 0;
        if (abs(ng.ZAxis) < tg.ZAxis) ng.ZAxis = 0;
    }

    return ng;
}

float MPU6050::readTemperature(void)
{
    int16_t T;
    T = readRegister16(MPU6050_REG_TEMP_OUT_H);
    return (float)T / 340 + 36.53;
}

int16_t MPU6050::getGyroOffsetX(void)
{
    return readRegister16(MPU6050_REG_GYRO_XOFFS_H);
}

int16_t MPU6050::getGyroOffsetY(void)
{
    return readRegister16(MPU6050_REG_GYRO_YOFFS_H);
}

int16_t MPU6050::getGyroOffsetZ(void)
{
    return readRegister16(MPU6050_REG_GYRO_ZOFFS_H);
}

void MPU6050::setGyroOffsetX(int16_t offset)
{
    writeRegister16(MPU6050_REG_GYRO_XOFFS_H, offset);
}

void MPU6050::setGyroOffsetY(int16_t offset)
{
    writeRegister16(MPU6050_REG_GYRO_YOFFS_H, offset);
}

void MPU6050::setGyroOffsetZ(int16_t offset)
{
    writeRegister16(MPU6050_REG_GYRO_ZOFFS_H, offset);
}

int16_t MPU6050::getAccelOffsetX(void)
{
    return readRegister16(MPU6050_REG_ACCEL_XOFFS_H);
}

int16_t MPU6050::getAccelOffsetY(void)
{
    return readRegister16(MPU6050_REG_ACCEL_YOFFS_H);
}

int16_t MPU6050::getAccelOffsetZ(void)
{
    return readRegister16(MPU6050_REG_ACCEL_ZOFFS_H);
}

void MPU6050::setAccelOffsetX(int16_t offset)
{
    writeRegister16(MPU6050_REG_ACCEL_XOFFS_H, offset);
}

void MPU6050::setAccelOffsetY(int16_t offset)
{
    writeRegister16(MPU6050_REG_ACCEL_YOFFS_H, offset);
}

void MPU6050::setAccelOffsetZ(int16_t offset)
{
    writeRegister16(MPU6050_REG_ACCEL_ZOFFS_H, offset);
}

// Calibrate algorithm
void MPU6050::calibrateGyro(uint8_t samples)
{
    // Set calibrate
    useCalibrate = true;

    // Reset values
    float sumX = 0;
    float sumY = 0;
    float sumZ = 0;
    float sigmaX = 0;
    float sigmaY = 0;
    float sigmaZ = 0;

    // Read n-samples
    for (uint8_t i = 0; i < samples; ++i) {
        readRawGyro();
        sumX += rg.XAxis;
        sumY += rg.YAxis;
        sumZ += rg.ZAxis;

        sigmaX += rg.XAxis * rg.XAxis;
        sigmaY += rg.YAxis * rg.YAxis;
        sigmaZ += rg.ZAxis * rg.ZAxis;

        delay(5);
    }

    // Calculate delta vectors
    dg.XAxis = sumX / samples;
    dg.YAxis = sumY / samples;
    dg.ZAxis = sumZ / samples;

    // Calculate threshold vectors
    th.XAxis = sqrt((sigmaX / 50) - (dg.XAxis * dg.XAxis));
    th.YAxis = sqrt((sigmaY / 50) - (dg.YAxis * dg.YAxis));
    th.ZAxis = sqrt((sigmaZ / 50) - (dg.ZAxis * dg.ZAxis));

    // If already set threshold, recalculate threshold vectors
    if (actualThreshold > 0) {
        setThreshold(actualThreshold);
    }
}

// Get current threshold value
uint8_t MPU6050::getThreshold(void)
{
    return actualThreshold;
}

// Set treshold value
void MPU6050::setThreshold(uint8_t multiple)
{
    if (multiple > 0) {
        // If not calibrated, need calibrate
        if (!useCalibrate) {
            calibrateGyro();
        }

        // Calculate threshold vectors
        tg.XAxis = th.XAxis * multiple;
        tg.YAxis = th.YAxis * multiple;
        tg.ZAxis = th.ZAxis * multiple;
    } else {
        // No threshold
        tg.XAxis = 0;
        tg.YAxis = 0;
        tg.ZAxis = 0;
    }

    // Remember old threshold value
    actualThreshold = multiple;
}

// Fast read 8-bit from register
uint8_t MPU6050::fastRegister8(uint8_t reg)
{
    uint8_t value;

#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(reg);
#else
    Wire.send(reg);
#endif
    Wire.endTransmission();

    Wire.requestFrom(mpuAddress, 1);
#if ARDUINO >= 100
    value = Wire.read();
#else
    value = Wire.receive();
#endif;
#else
    __read(mpuAddress, reg, &value, 1);
#endif
    return value;
}

// Read 8-bit from register
uint8_t MPU6050::readRegister8(uint8_t reg)
{
    uint8_t value;
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(reg);
#else
    Wire.send(reg);
#endif
    Wire.endTransmission();

    Wire.requestFrom(mpuAddress, 1);
    while (!Wire.available()) {};
#if ARDUINO >= 100
    value = Wire.read();
#else
    value = Wire.receive();
#endif;
#else
    __read(mpuAddress, reg, &value, 1);
#endif
    return value;
}

// Write 8-bit to register
void MPU6050::writeRegister8(uint8_t reg, uint8_t value)
{
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);

#if ARDUINO >= 100
    Wire.write(reg);
    Wire.write(value);
#else
    Wire.send(reg);
    Wire.send(value);
#endif
    Wire.endTransmission();
#else
    __write(mpuAddress, reg, &value, 1);
#endif
}

int16_t MPU6050::readRegister16(uint8_t reg)
{
    int16_t value ;
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(reg);
#else
    Wire.send(reg);
#endif
    Wire.endTransmission();

    Wire.requestFrom(mpuAddress, 2);
    while (!Wire.available()) {};
#if ARDUINO >= 100
    uint8_t vha = Wire.read();
    uint8_t vla = Wire.read();
#else
    uint8_t vha = Wire.receive();
    uint8_t vla = Wire.receive();
#endif;
    value = vha << 8 | vla;
#else
    uint8_t data[2] = {0};
    __read(mpuAddress, reg, data, 2);
    value = data[0] << 8 | data[1];
#endif
    return value;
}

void MPU6050::writeRegister16(uint8_t reg, int16_t value)
{
#ifdef DISABLE_I2C_CALLBLACK
    Wire.beginTransmission(mpuAddress);
#if ARDUINO >= 100
    Wire.write(reg);
    Wire.write((uint8_t)(value >> 8));
    Wire.write((uint8_t)value);
#else
    Wire.send(reg);
    Wire.send((uint8_t)(value >> 8));
    Wire.send((uint8_t)value);
#endif
    Wire.endTransmission();
#else
    __write(mpuAddress, reg, (uint8_t *)&value, 2);
#endif
}

// Read register bit
bool MPU6050::readRegisterBit(uint8_t reg, uint8_t pos)
{
    uint8_t value;
    value = readRegister8(reg);
    return ((value >> pos) & 1);
}

// Write register bit
void MPU6050::writeRegisterBit(uint8_t reg, uint8_t pos, bool state)
{
    uint8_t value;
    value = readRegister8(reg);

    if (state) {
        value |= (1 << pos);
    } else {
        value &= ~(1 << pos);
    }

    writeRegister8(reg, value);
}
