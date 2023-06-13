/**
 * @file      LilyGoLib.cpp
 * @author    Lewis He (lewishe@outlook.com)
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @date      2023-04-28
 *
 */

#include "LilyGoLib.h"

#ifdef USING_TWATCH_S3
SPIClass radioBus =  SPIClass(HSPI);
#endif


void deviceScan(TwoWire *_port, Stream *stream)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        _port->beginTransmission(addr);
        err = _port->endTransmission();
        if (err == 0) {
            stream->print("I2C device found at address 0x");
            if (addr < 16)
                stream->print("0");
            stream->print(addr, HEX);
            stream->println(" !");
            nDevices++;
        } else if (err == 4) {
            stream->print("Unknow error at address 0x");
            if (addr < 16)
                stream->print("0");
            stream->println(addr, HEX);
        }
    }
    if (nDevices == 0)
        stream->println("No I2C devices found\n");
    else
        stream->println("Done\n");
}


LilyGoLib::LilyGoLib()

#ifdef USING_TWATCH_S3
    : SX1262(new Module(BOARD_RADIO_SS,
                        BOARD_RADIO_DI01,
                        BOARD_RADIO_RST,
                        BOARD_RADIO_BUSY,
                        radioBus))
#endif
{
    //Default use SX1262
}

LilyGoLib::~LilyGoLib()
{

}

void LilyGoLib::log_println(const char *message)
{
    if (stream) {
        stream->println(message);
    }
}


bool LilyGoLib::begin(Stream *stream)
{
    bool res;

#ifdef LILYGO_LIB_DEBUG
    this->stream = &Serial;
#else
    this->stream = stream;
#endif
    pinMode(BOARD_TOUCH_INT, INPUT);

    Wire.begin(BOARD_I2C_SDA, BOARD_I2C_SCL);
    if (stream) {
        deviceScan(&Wire, stream);
    }

    Wire1.begin(BOARD_TOUCH_SDA, BOARD_TOUCH_SCL);
    if (stream) {
        deviceScan(&Wire1, stream);
    }

    log_println("Init PMU");
    if (!beginPower()) {
        log_println("Failed to find PMU - check your wiring!");
        return false;
    } else {
        log_println("Initializing PMU succeeded");
    }

    log_println("Init LEDC");
    ledcSetup(LEDC_BACKLIGHT_CHANNEL, LEDC_BACKLIGHT_FREQ, LEDC_BACKLIGHT_BIT_WIDTH);
    ledcAttachPin(BOARD_TFT_BL, LEDC_BACKLIGHT_CHANNEL);

    log_println("Init TFT");
    TFT_eSPI::init();
    setRotation(2);
    setTextDatum(MC_DATUM);
    setTextFont(2);

    log_println("Init SPIFFS");
    if (!SPIFFS.begin()) {
        fillScreen(TFT_BLACK);
        drawString("Format SPIFFS...", 120, 120);
        SPIFFS.format();
    }

    fillScreen(TFT_BLACK);
    drawString("Hello T-Watch", 120, 120);

    setBrightness(50);

    log_println("Init Touch");
    res = TouchDrvFT6X36::init(Wire1, BOARD_TOUCH_SDA, BOARD_TOUCH_SCL);
    if (!res) {
        log_println("Failed to find FT6X36 - check your wiring!");
    } else {
        log_println("Initializing FT6X36 succeeded");
        interruptTrigger(); //enable Interrupt
    }

    log_println("Init BMA423");
    res = SensorBMA423::init(Wire);
    if (!res) {
        log_println("Failed to find BMA423 - check your wiring!");
    } else {
        log_println("Initializing BMA423 succeeded");
        setReampAxes(REMAP_BOTTOM_LAYER_TOP_RIGHT_CORNER);
        setStepCounterWatermark(1);
    }

    log_println("Init PCF8563 RTC");
    res = SensorPCF8563::init(Wire);
    if (!res) {
        log_println("Failed to find PCF8563 - check your wiring!");
    } else {
        log_println("Initializing PCF8563 succeeded");
        hwClockRead();  //Synchronize RTC clock to system clock
    }

    log_println("Init DRV2605");
    res = SensorDRV2605::init(Wire);
    if (!res) {
        log_println("Failed to find DRV2605 - check your wiring!");
    } else {
        log_println("Initializing DRV2605 succeeded");
        SensorDRV2605::selectLibrary(1);
        SensorDRV2605::setMode(DRV2605_MODE_INTTRIG);
        SensorDRV2605::useERM();
        SensorDRV2605::setWaveform(0, 15);  // play effect
        SensorDRV2605::setWaveform(1, 0);  // end waveform
        SensorDRV2605::run();
    }

#ifdef USING_TWATCH_S3
    log_println("Init Radio");
    radioBus.begin(BOARD_RADIO_SCK,
                   BOARD_RADIO_MISO,
                   BOARD_RADIO_MOSI);

    //Using default params，433Mhz,bw:125k,sf:9,cr:7,syncword:18,txpower:10
    if (SX1262::begin() == RADIOLIB_ERR_NONE) {
        log_println("Initializing Radio succeeded");
    } else {
        log_println("Failed to find Radio - check your wiring!");
    }
#endif

    beginCore();

    delay(1000);

    return true;
}

void LilyGoLib::beginCore()
{
    // https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.4/esp32s3/api-reference/peripherals/temp_sensor.html
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    temp_sensor_config_t temp_sensor = {
        .dac_offset = TSENS_DAC_L2,
        .clk_div = 6,
    };
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
#else
    // https://docs.espressif.com/projects/esp-idf/zh_CN/v5.0.1/esp32s3/api-reference/peripherals/temp_sensor.html
    static temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT(10, 50);
    temperature_sensor_install(&temp_sensor_config, &temp_sensor);
    temperature_sensor_enable(temp_sensor);
#endif
}



void LilyGoLib::attachPMU(void(*cb)(void))
{
    pinMode(BOARD_PMU_INT, INPUT_PULLUP);
    attachInterrupt(BOARD_PMU_INT, cb, FALLING);
}

void LilyGoLib::attachBMA(void(*cb)(void))
{
    pinMode(BOARD_BMA423_INT1, INPUT_PULLUP);
    attachInterrupt(BOARD_BMA423_INT1, cb, RISING);
}

void LilyGoLib::attachRTC(void(*cb)(void))
{
    pinMode(BOARD_RTC_INT_PIN, INPUT_PULLUP);
    attachInterrupt(BOARD_RTC_INT_PIN, cb, RISING);
}

uint16_t LilyGoLib::readBMA()
{
    return SensorBMA423::readIrqStatus();
}

uint64_t LilyGoLib::readPMU()
{
#ifdef USING_TWATCH_S3
    return XPowersAXP2101::getIrqStatus();
#else
    return XPowersAXP202::getIrqStatus();
#endif
}

void LilyGoLib::clearPMU()
{
#ifdef USING_TWATCH_S3
    XPowersAXP2101::clearIrqStatus();
#else
    XPowersAXP202::clearIrqStatus();
#endif
}

bool LilyGoLib::readRTC()
{
    return digitalRead(BOARD_RTC_INT_PIN) == LOW;
}

float LilyGoLib::readAccelTemp()
{
    return  SensorBMA423::getTemperature(SensorBMA423::TEMP_DEG);
}

float LilyGoLib::readCoreTemp()
{
    float tsens_value;
    // https://docs.espressif.com/projects/esp-idf/zh_CN/v4.4.4/esp32s3/api-reference/peripherals/temp_sensor.html
#if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5,0,0)
    temp_sensor_read_celsius(&tsens_value);
#else
    // https://docs.espressif.com/projects/esp-idf/zh_CN/v5.0.1/esp32s3/api-reference/peripherals/temp_sensor.html
    temperature_sensor_get_celsius(temp_sensor, &tsens_value);
#endif
    return tsens_value;
}


bool LilyGoLib::getTouched()
{
    return digitalRead(BOARD_TOUCH_INT) == LOW;
}


uint8_t LilyGoLib::getBrightness()
{
    return brightness;
}


void LilyGoLib::setBrightness(uint8_t level)
{
    if (!level) {
        disableALDO2();
        writecommand(0x10);  //display sleep
        setPowerMode(PMODE_MONITOR);
    }
    if (!brightness) {
        enableALDO2();
        writecommand(0x11);  //display wakeup
    }
    brightness = level;
    ledcWrite(LEDC_BACKLIGHT_CHANNEL, brightness);
}


void LilyGoLib::decrementBrightness(uint8_t target_level, uint32_t delay_ms)
{
    if (target_level > brightness)
        return;
    for (int i = brightness; i >= target_level; i--) {
        setBrightness(i);
        delay(delay_ms);
    }
}

void LilyGoLib::incrementalBrightness(uint8_t target_level, uint32_t delay_ms)
{
    if (target_level < brightness)
        return;
    for (int i = brightness; i < target_level; i++) {
        setBrightness(i);
        delay(delay_ms);
    }
}


bool LilyGoLib::beginPower()
{
#ifdef USING_TWATCH_S3
    bool res =  XPowersAXP2101::init(Wire, BOARD_I2C_SDA, BOARD_I2C_SCL);
    if (!res) {
        return false;
    }

    // Set the minimum common working voltage of the PMU VBUS input,
    // below this value will turn off the PMU
    setVbusVoltageLimit(XPOWERS_AXP2101_VBUS_VOL_LIM_4V36);

    // Set the maximum current of the PMU VBUS input,
    // higher than this value will turn off the PMU
    setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_100MA);

    // Set VSY off voltage as 2600mV , Adjustment range 2600mV ~ 3300mV
    setSysPowerDownVoltage(2600);


    // ! ESP32S3 VDD, Don't change
    // setDC1Voltage(3300);

    //! RTC VBAT , Don't change
    setALDO1Voltage(3300);

    //! TFT BACKLIGHT VDD , Don't change
    setALDO2Voltage(3300);

    //!Screen touch VDD , Don't change
    setALDO3Voltage(3300);

    //! Radio VDD , Don't change
    setALDO4Voltage(3300);

    //!DRV2605 enable
    setBLDO2Voltage(3300);


    //! No use
    disableDC2();
    disableDC3();
    disableDC4();
    disableDC5();
    disableBLDO1();
    disableCPUSLDO();
    disableDLDO1();
    disableDLDO2();



    enableALDO1();  //! RTC VBAT
    enableALDO2();  //! TFT BACKLIGHT   VDD
    enableALDO3();  //! Screen touch VDD
    enableALDO4();  //! Radio VDD
    enableBLDO2();  //! drv2605 enable


    if (stream) {
        log_println("DCDC=======================================================================");
        stream->printf("DC1  : %s   Voltage:%u mV \n", isEnableDC1()  ? "+" : "-", getDC1Voltage());
        stream->printf("DC2  : %s   Voltage:%u mV \n", isEnableDC2()  ? "+" : "-", getDC2Voltage());
        stream->printf("DC3  : %s   Voltage:%u mV \n", isEnableDC3()  ? "+" : "-", getDC3Voltage());
        stream->printf("DC4  : %s   Voltage:%u mV \n", isEnableDC4()  ? "+" : "-", getDC4Voltage());
        stream->printf("DC5  : %s   Voltage:%u mV \n", isEnableDC5()  ? "+" : "-", getDC5Voltage());
        log_println("ALDO=======================================================================");
        stream->printf("ALDO1: %s   Voltage:%u mV\n", isEnableALDO1()  ? "+" : "-", getALDO1Voltage());
        stream->printf("ALDO2: %s   Voltage:%u mV\n", isEnableALDO2()  ? "+" : "-", getALDO2Voltage());
        stream->printf("ALDO3: %s   Voltage:%u mV\n", isEnableALDO3()  ? "+" : "-", getALDO3Voltage());
        stream->printf("ALDO4: %s   Voltage:%u mV\n", isEnableALDO4()  ? "+" : "-", getALDO4Voltage());
        log_println("BLDO=======================================================================");
        stream->printf("BLDO1: %s   Voltage:%u mV\n", isEnableBLDO1()  ? "+" : "-", getBLDO1Voltage());
        stream->printf("BLDO2: %s   Voltage:%u mV\n", isEnableBLDO2()  ? "+" : "-", getBLDO2Voltage());
        log_println("CPUSLDO====================================================================");
        stream->printf("CPUSLDO: %s Voltage:%u mV\n", isEnableCPUSLDO() ? "+" : "-", getCPUSLDOVoltage());
        log_println("DLDO=======================================================================");
        stream->printf("DLDO1: %s   Voltage:%u mV\n", isEnableDLDO1()  ? "+" : "-", getDLDO1Voltage());
        stream->printf("DLDO2: %s   Voltage:%u mV\n", isEnableDLDO2()  ? "+" : "-", getDLDO2Voltage());
        log_println("===========================================================================");
    }


    // Set the time of pressing the button to turn off
    setPowerKeyPressOffTime(XPOWERS_POWEROFF_4S);

    // Set the button power-on press time
    setPowerKeyPressOnTime(XPOWERS_POWERON_128MS);


    // It is necessary to disable the detection function of the TS pin on the board
    // without the battery temperature detection function, otherwise it will cause abnormal charging
    disableTSPinMeasure();


    // Enable internal ADC detection
    enableBattDetection();
    enableVbusVoltageMeasure();
    enableBattVoltageMeasure();
    enableSystemVoltageMeasure();


    //t-watch no chg led
    setChargingLedMode(XPOWERS_CHG_LED_OFF);


    disableIRQ(XPOWERS_AXP2101_ALL_IRQ);

    // Clear all interrupt flags
    XPowersAXP2101::clearIrqStatus();

    // Enable the required interrupt function
    watch.enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
    );

    // Set the precharge charging current
    setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_50MA);
    // Set constant current charge current limit
    setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_100MA);
    // Set stop charging termination current
    setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);

    // Set charge cut-off voltage
    setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V35);

    enableButtonBatteryCharge();
#else

    bool res =  XPowersAXP202::init(Wire);
    if (!res) {
        return false;
    }
    //In the 2020V1 version, the ST7789 chip power supply
    //is shared with the backlight, so LDO2 cannot be turned off
    setLDO2Voltage(3300);
    enableLDO2();

#endif


    return true;
}

void LilyGoLib::lowPower()
{
    // disableALDO1();  //! RTC VBAT
    // disableALDO2();  //! TFT BACKLIGHT   VDD
    // disableALDO3();  //! Screen touch VDD
    // disableALDO4();  //! Radio VDD
    // disableBLDO2();  //! drv2605 enable
}

void LilyGoLib::highPower()
{
    // enableALDO1();  //! RTC VBAT
    // enableALDO2();  //! TFT BACKLIGHT   VDD
    // enableALDO3();  //! Screen touch VDD
    // enableALDO4();  //! Radio VDD
    // enableBLDO2();  //! drv2605 enable

}

bool LilyGoLib::initMicrophone()
{
    static i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate =  MIC_I2S_SAMPLE_RATE,
        .bits_per_sample = MIC_I2S_BITS_PER_SAMPLE,
        .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
        .communication_format = I2S_COMM_FORMAT_STAND_PCM_SHORT,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 6,
        .dma_buf_len = 512,
        .use_apll = true
    };

    static i2s_pin_config_t i2s_cfg = {0};
    i2s_cfg.bck_io_num   = I2S_PIN_NO_CHANGE;
    i2s_cfg.ws_io_num    = BOARD_MIC_CLOCK;
    i2s_cfg.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_cfg.data_in_num  = BOARD_MIC_DATA;
    i2s_cfg.mck_io_num = I2S_PIN_NO_CHANGE;

    if (i2s_driver_install(MIC_I2S_PORT, &i2s_config, 0, NULL) != ESP_OK) {
        log_println("i2s_driver_install error");
        return false;
    }

    if (i2s_set_pin(MIC_I2S_PORT, &i2s_cfg) != ESP_OK) {
        log_println("i2s_set_pin error");
        return false;
    }
    log_println("Microphone init done .");
    return true;
}

bool LilyGoLib::readMicrophone(void *dest, size_t size, size_t *bytes_read, TickType_t ticks_to_wait)
{
    return i2s_read(MIC_I2S_PORT, dest, size, bytes_read, ticks_to_wait) == ESP_OK;
}

void LilyGoLib::setSleepMode(SleepMode_t mode)
{
    sleepMode = mode;
}

void LilyGoLib::sleep(uint32_t second)
{
    // SX126x::sleep();
    // // SensorBMA423::sleep();
    // TouchDrvFT6X36::setPowerMode(PMODE_DEEPSLEEP);

    switch (sleepMode) {
    case PMU_BTN_WAKEUP:
        esp_sleep_enable_ext1_wakeup(_BV(BOARD_PMU_INT), ESP_EXT1_WAKEUP_ALL_LOW);
        break;
    case ESP_TIMER_WAKEUP:
        esp_sleep_enable_timer_wakeup(1000000ULL * second);
        break;
    case SENSOR_WAKEUP:
        esp_sleep_enable_ext1_wakeup(_BV(BOARD_BMA423_INT1), ESP_EXT1_WAKEUP_ANY_HIGH);
        break;
    case RTC_WAKEUP:
        esp_sleep_enable_ext1_wakeup(_BV(BOARD_RTC_INT_PIN), ESP_EXT1_WAKEUP_ALL_LOW);
        break;
    case TOUCH_WAKEUP:
        esp_sleep_enable_ext1_wakeup(_BV(BOARD_TOUCH_INT), ESP_EXT1_WAKEUP_ALL_LOW);
        break;
    default:
        break;
    }
    esp_deep_sleep_start();
}

int16_t LilyGoLib::sleep()
{
    sleep(0);
    return 0;
}








LilyGoLib watch;



















