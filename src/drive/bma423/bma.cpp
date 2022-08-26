#include "bma.h"
#include <Arduino.h>

I2CBus *BMA::_bus = nullptr;

BMA::BMA(I2CBus &bus)
{
    _bus = &bus;
    _init = false;
}

BMA::~BMA()
{
}

uint16_t BMA::read(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    return _bus->readBytes(addr, reg, data, len);
}

uint16_t BMA::write(uint8_t addr, uint8_t reg, uint8_t *data, uint16_t len)
{
    return _bus->writeBytes(addr, reg, data, len);
}

bool BMA::begin()
{
    uint8_t data;
    if (_init) {
        return true;
    }

    _dev.dev_addr        = BMA4_I2C_ADDR_SECONDARY;
    _dev.interface       = BMA4_I2C_INTERFACE;
    _dev.bus_read        = read;
    _dev.bus_write       = write;
    _dev.delay           = delay;
    _dev.read_write_len  = 32;
    _dev.resolution      = 12;
    _dev.feature_len     = BMA423_FEATURE_SIZE;

    reset();
    delay(20);

    if (bma423_init(&_dev) != BMA4_OK) {
        log_e("bma423_init FAIL");
        return false;
    }

    read(BMA4_I2C_ADDR_SECONDARY, BMA4_INTERNAL_STAT, &data, 1);
    if (data != BMA4_ASIC_INITIALIZED ) {
        log_i("bma423 reconfig!");
        if (bma423_write_config_file(&_dev) != BMA4_OK) {
            log_e("bma423_write_config_file FAIL");
            return false;
        }
    }

    _init = true;

    struct bma4_int_pin_config config ;
    config.edge_ctrl = BMA4_LEVEL_TRIGGER;
    config.lvl = BMA4_ACTIVE_HIGH;
    config.od = BMA4_PUSH_PULL;
    config.output_en = BMA4_OUTPUT_ENABLE;
    config.input_en = BMA4_INPUT_DISABLE;
    return BMA4_OK == bma4_set_int_pin_config(&config, BMA4_INTR1_MAP, &_dev);
}

void BMA::reset()
{
    uint8_t reg = 0xB6;
    _bus->writeBytes(BMA4_I2C_ADDR_SECONDARY, 0x7E, &reg, 1);
}

bool BMA::getAccel(Accel &acc)
{
    memset(&acc, 0, sizeof(acc));
    if (bma4_read_accel_xyz(&acc, &_dev) != BMA4_OK) {
        return false;
    }
    return true;
}

uint8_t BMA::direction()
{
    Accel acc;
    if (bma4_read_accel_xyz(&acc, &_dev) != BMA4_OK) {
        return 0;
    }
    uint16_t absX = abs(acc.x);
    uint16_t absY = abs(acc.y);
    uint16_t absZ = abs(acc.z);

    if ((absZ > absX) && (absZ > absY)) {
        if (acc.z > 0) {
            return  DIRECTION_DISP_DOWN;
        } else {
            return DIRECTION_DISP_UP;
        }
    } else if ((absY > absX) && (absY > absZ)) {
        if (acc.y > 0) {
            return DIRECTION_BOTTOM_EDGE;
        } else {
            return  DIRECTION_TOP_EDGE;
        }
    } else {
        if (acc.x < 0) {
            return  DIRECTION_RIGHT_EDGE;
        } else {
            return DIRECTION_LEFT_EDGE;
        }
    }
}

float BMA::temperature()
{
    int32_t data = 0;
    bma4_get_temperature(&data, BMA4_DEG, &_dev);
    float res = (float)data / (float)BMA4_SCALE_TEMP;
    /* 0x80 - temp read from the register and 23 is the ambient temp added.
     * If the temp read from register is 0x80, it means no valid
     * information is available */
    if (((data - 23) / BMA4_SCALE_TEMP) == 0x80) {
        res = 0;
    }
    return res;
}

bool BMA::disableAccel()
{
    return enableAccel(false);
}

bool BMA::enableAccel(bool en)
{
    /*
    if (bma4_set_accel_enable(BMA4_ENABLE, &_dev)) {
        return;
    }
    Acfg cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    cfg.range = BMA4_ACCEL_RANGE_2G;
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    if (bma4_set_accel_config(&cfg, &_dev)) {
        Serial.println("[bma4] set accel config fail");
        return;
    }
    */
    return (BMA4_OK == bma4_set_accel_enable(en ? BMA4_ENABLE : BMA4_DISABLE, &_dev));
}

bool BMA::accelConfig(Acfg &cfg)
{
    return (BMA4_OK == bma4_set_accel_config(&cfg, &_dev));
}

bool BMA::disalbeIrq(uint16_t int_map)
{
    return (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_DISABLE, &_dev));
}

bool BMA::enableIrq(uint16_t int_map)
{
    return (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, int_map, BMA4_ENABLE, &_dev));
}

bool BMA::enableFeature(uint8_t feature, uint8_t enable)
{
    if ((feature & BMA423_STEP_CNTR) == BMA423_STEP_CNTR) {
        bma423_step_detector_enable(enable ? BMA4_ENABLE : BMA4_DISABLE, &_dev);
    }

    return (BMA4_OK == bma423_feature_enable(feature, enable, &_dev));
}

bool BMA::resetStepCounter()
{
    return  BMA4_OK == bma423_reset_step_counter(&_dev) ;
}

// Reserved for the SimpleWatch example
// Reserved for the SimpleWatch example
// Reserved for the SimpleWatch example
void BMA::attachInterrupt()
{
    uint16_t rslt = BMA4_OK;
    if (bma4_set_accel_enable(BMA4_ENABLE, &_dev)) {
        return;
    }
    Acfg cfg;
    cfg.odr = BMA4_OUTPUT_DATA_RATE_100HZ;
    cfg.range = BMA4_ACCEL_RANGE_2G;
    cfg.bandwidth = BMA4_ACCEL_NORMAL_AVG4;
    cfg.perf_mode = BMA4_CONTINUOUS_MODE;

    if (bma4_set_accel_config(&cfg, &_dev)) {
        log_e("[bma4] set accel config fail");
        return;
    }
    // // rslt |= bma423_reset_step_counter(&_dev);
    rslt |= bma423_step_detector_enable(BMA4_ENABLE, &_dev);
    rslt |= bma423_feature_enable(BMA423_STEP_CNTR, BMA4_ENABLE, &_dev);
    rslt |= bma423_feature_enable(BMA423_WAKEUP, BMA4_ENABLE, &_dev);
    rslt |= bma423_feature_enable(BMA423_TILT, BMA4_ENABLE, &_dev);
    rslt |= bma423_step_counter_set_watermark(100, &_dev);

    // rslt |= bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_STEP_CNTR_INT | BMA423_WAKEUP_INT, BMA4_ENABLE, &_dev);
    rslt |= bma423_map_interrupt(BMA4_INTR1_MAP,  BMA423_STEP_CNTR_INT, BMA4_ENABLE, &_dev);
    rslt |= bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_TILT_INT, BMA4_ENABLE, &_dev);

    bma423_anymotion_enable_axis(BMA423_ALL_AXIS_DIS, &_dev);
}

bool BMA::set_remap_axes(struct bma423_axes_remap *remap_data)
{
    return (BMA4_OK == bma423_set_remap_axes(remap_data, &_dev));
}

bool BMA::readInterrupt()
{
    return bma423_read_int_status(&_irqStatus, &_dev) == BMA4_OK;
}

uint8_t BMA::getIrqStatus()
{
    return _irqStatus;
}

uint32_t BMA::getCounter()
{
    uint32_t stepCount;
    if (bma423_step_counter_output(&stepCount, &_dev) == BMA4_OK) {
        return stepCount;
    }
    return 0;
}

bool BMA::isStepCounter()
{
    return (bool)(BMA423_STEP_CNTR_INT & _irqStatus);
}

bool BMA::isDoubleClick()
{
    return (bool)(BMA423_WAKEUP_INT & _irqStatus);
}


bool BMA::isTilt()
{
    return (bool)(BMA423_TILT_INT & _irqStatus);
}


bool BMA::isActivity()
{
    return (bool)(BMA423_ACTIVITY_INT & _irqStatus);
}

bool BMA::isAnyNoMotion()
{
    return (bool)(BMA423_ANY_NO_MOTION_INT & _irqStatus);
}

const char *BMA::getActivity()
{
    uint8_t activity;
    bma423_activity_output(&activity, &_dev);
    if (activity & BMA423_USER_STATIONARY) {
        return "BMA423_USER_STATIONARY";
    } else if (activity & BMA423_USER_WALKING) {
        return "BMA423_USER_WALKING";
    } else if (activity & BMA423_USER_RUNNING) {
        return "BMA423_USER_RUNNING";
    } else if (activity & BMA423_STATE_INVALID) {
        return "BMA423_STATE_INVALID";
    }
    return "None";
}

bool BMA::enableStepCountInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP,  BMA423_STEP_CNTR_INT, en, &_dev));
}

bool BMA::enableTiltInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_TILT_INT, en, &_dev));
}

bool BMA::enableWakeupInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_WAKEUP_INT, en, &_dev));
}

bool BMA::enableAnyNoMotionInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_ANY_NO_MOTION_INT, en, &_dev));
}

bool BMA::enableActivityInterrupt(bool en)
{
    return  (BMA4_OK == bma423_map_interrupt(BMA4_INTR1_MAP, BMA423_ACTIVITY_INT, en, &_dev));
}

