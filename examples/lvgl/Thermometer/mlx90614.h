#pragma once

#include <TTGO.h>

#define MLX90614_ADDRESS 0x5A
#define MLX90614_REGISTER_TA      0x06
#define MLX90614_REGISTER_TOBJ    0x07
#define MLX90614_REGISTER_SLEEP   0xFF

uint8_t crc8 (uint8_t inCrc, uint8_t inData)
{
    uint8_t i;
    uint8_t data;
    data = inCrc ^ inData;
    for ( i = 0; i < 8; i++ ) {
        if (( data & 0x80 ) != 0 ) {
            data <<= 1;
            data ^= 0x07;
        } else {
            data <<= 1;
        }
    }
    return data;
}

bool mlx90614_probe()
{
    TTGOClass *watch = TTGOClass::getWatch();
    return watch->deviceProbe(MLX90614_ADDRESS);
}

bool mlx90614_read(uint8_t reg, int16_t *dest)
{
    uint8_t raw[3];
    TTGOClass *watch = TTGOClass::getWatch();
    watch->readBytes(MLX90614_ADDRESS, reg, raw, 3);
    uint8_t crc = crc8(0, (MLX90614_ADDRESS << 1));
    crc = crc8(crc, reg);
    crc = crc8(crc, (MLX90614_ADDRESS << 1) + 1);
    crc = crc8(crc, raw[0]);
    crc = crc8(crc, raw[1]);

    if (crc == raw[2]) {
        *dest = (raw[1] << 8) | raw[0];
        return true;
    } else {
        return false;
    }
}

float mlx90614_read_object()
{
    int16_t obj = 0;
    if (mlx90614_read(MLX90614_REGISTER_TOBJ, &obj)) {
        if (obj & 0x8000) {
            return 0;
        }
        return float(obj) * 0.02 - 273.15;
    }
    return 0;
}

float mlx90614_read_ambient()
{
    int16_t ambient = 0;
    if (mlx90614_read(MLX90614_REGISTER_TA, &ambient)) {
        return float(ambient) * 0.02 - 273.15;;
    }
    return 0;
}
