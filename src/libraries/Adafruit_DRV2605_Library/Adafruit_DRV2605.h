/*!
 * @file Adafruit_DRV2605.h
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
 * Written by Limor Fried/Ladyada for Adafruit Industries.
 *
 * MIT license, all text above must be included in any redistribution.
 *
 */
/**************************************************************************/

#include "Arduino.h"
#include <Wire.h>

#define DRV2605_ADDR                0x5A ///< Device I2C address

#define DRV2605_REG_STATUS          0x00       ///< Status register
#define DRV2605_REG_MODE            0x01         ///< Mode register
#define DRV2605_MODE_INTTRIG        0x00     ///< Internal trigger mode
#define DRV2605_MODE_EXTTRIGEDGE    0x01 ///< External edge trigger mode
#define DRV2605_MODE_EXTTRIGLVL     0x02  ///< External level trigger mode
#define DRV2605_MODE_PWMANALOG      0x03   ///< PWM/Analog input mode
#define DRV2605_MODE_AUDIOVIBE      0x04   ///< Audio-to-vibe mode
#define DRV2605_MODE_REALTIME       0x05    ///< Real-time playback (RTP) mode
#define DRV2605_MODE_DIAGNOS        0x06     ///< Diagnostics mode
#define DRV2605_MODE_AUTOCAL        0x07     ///< Auto calibration mode

#define DRV2605_REG_RTPIN           0x02    ///< Real-time playback input register
#define DRV2605_REG_LIBRARY         0x03  ///< Waveform library selection register
#define DRV2605_REG_WAVESEQ1        0x04 ///< Waveform sequence register 1
#define DRV2605_REG_WAVESEQ2        0x05 ///< Waveform sequence register 2
#define DRV2605_REG_WAVESEQ3        0x06 ///< Waveform sequence register 3
#define DRV2605_REG_WAVESEQ4        0x07 ///< Waveform sequence register 4
#define DRV2605_REG_WAVESEQ5        0x08 ///< Waveform sequence register 5
#define DRV2605_REG_WAVESEQ6        0x09 ///< Waveform sequence register 6
#define DRV2605_REG_WAVESEQ7        0x0A ///< Waveform sequence register 7
#define DRV2605_REG_WAVESEQ8        0x0B ///< Waveform sequence register 8

#define DRV2605_REG_GO              0x0C         ///< Go register
#define DRV2605_REG_OVERDRIVE       0x0D  ///< Overdrive time offset register
#define DRV2605_REG_SUSTAINPOS      0x0E ///< Sustain time offset, positive register
#define DRV2605_REG_SUSTAINNEG      0x0F ///< Sustain time offset, negative register
#define DRV2605_REG_BREAK           0x10      ///< Brake time offset register
#define DRV2605_REG_AUDIOCTRL       0x11  ///< Audio-to-vibe control register
#define DRV2605_REG_AUDIOLVL                                                   \
  0x12 ///< Audio-to-vibe minimum input level register
#define DRV2605_REG_AUDIOMAX                                                   \
  0x13 ///< Audio-to-vibe maximum input level register
#define DRV2605_REG_AUDIOOUTMIN                                                \
  0x14 ///< Audio-to-vibe minimum output drive register
#define DRV2605_REG_AUDIOOUTMAX                                                \
  0x15                          ///< Audio-to-vibe maximum output drive register
#define DRV2605_REG_RATEDV 0x16 ///< Rated voltage register
#define DRV2605_REG_CLAMPV 0x17 ///< Overdrive clamp voltage register
#define DRV2605_REG_AUTOCALCOMP                                                \
  0x18 ///< Auto-calibration compensation result register
#define DRV2605_REG_AUTOCALEMP                                                 \
  0x19                            ///< Auto-calibration back-EMF result register
#define DRV2605_REG_FEEDBACK 0x1A ///< Feedback control register
#define DRV2605_REG_CONTROL1 0x1B ///< Control1 Register
#define DRV2605_REG_CONTROL2 0x1C ///< Control2 Register
#define DRV2605_REG_CONTROL3 0x1D ///< Control3 Register
#define DRV2605_REG_CONTROL4 0x1E ///< Control4 Register
#define DRV2605_REG_VBAT 0x21     ///< Vbat voltage-monitor register
#define DRV2605_REG_LRARESON 0x22 ///< LRA resonance-period register

/**************************************************************************/
/*!
  @brief The DRV2605 driver class.
*/
/**************************************************************************/
class Adafruit_DRV2605
{
public:
    typedef uint8_t (*i2c_com_fptr_t)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t len);

    Adafruit_DRV2605(void);
    bool begin(TwoWire *theWire = &Wire);
    bool begin(i2c_com_fptr_t r, i2c_com_fptr_t w);

    bool init();
    void writeRegister8(uint8_t reg, uint8_t val);
    uint8_t readRegister8(uint8_t reg);
    void setWaveform(uint8_t slot, uint8_t w);
    void selectLibrary(uint8_t lib);
    void go(void);
    void stop(void);
    void setMode(uint8_t mode);
    void setRealtimeValue(uint8_t rtp);
    // Select ERM (Eccentric Rotating Mass) or LRA (Linear Resonant Actuator)
    // vibration motor The default is ERM, which is more common
    void useERM();
    void useLRA();

private:
    TwoWire *_wire;
    i2c_com_fptr_t readCallback;
    i2c_com_fptr_t writeCallback;
};
