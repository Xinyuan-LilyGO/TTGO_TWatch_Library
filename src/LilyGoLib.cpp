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
{
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

void LilyGoLib::disableBootDisplay()
{
    bootDisplay = false;
}


uint32_t LilyGoLib::getDeviceProbe()
{
    return devices_probe;
}

bool LilyGoLib::begin(Stream *stream)
{
    bool res;

    devices_probe = 0x00;

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
        devices_probe |= WATCH_PMU_ONLINE;
        log_println("Initializing PMU succeeded");
    }

    log_println("Init LEDC");
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,0,0)
    ledcAttach(BOARD_TFT_BL, LEDC_BACKLIGHT_FREQ, LEDC_BACKLIGHT_BIT_WIDTH);
#else
    ledcSetup(LEDC_BACKLIGHT_CHANNEL, LEDC_BACKLIGHT_FREQ, LEDC_BACKLIGHT_BIT_WIDTH);
    ledcAttachPin(BOARD_TFT_BL, LEDC_BACKLIGHT_CHANNEL);
#endif


    log_println("Init TFT");
    TFT_eSPI::init();
    setRotation(2);
    setTextDatum(MC_DATUM);
    setTextFont(2);
    fillScreen(TFT_BLACK);

    log_println("Init FFat");
    if (!FFat.begin()) {

        if (bootDisplay) {
            setBrightness(50);
            drawString("Format FFat...", 120, 120);
        }
        FFat.format();
    }

    if (bootDisplay) {
        fillScreen(TFT_BLACK);
        drawString("Hello T-Watch", 120, 120);
        setBrightness(50);
    }

    log_println("Init Touch");
    res = TouchDrvFT6X36::begin(Wire1, FT6X36_SLAVE_ADDRESS, BOARD_TOUCH_SDA, BOARD_TOUCH_SCL);
    if (!res) {
        log_println("Failed to find FT6X36 - check your wiring!");
    } else {
        log_println("Initializing FT6X36 succeeded");
        interruptTrigger(); //enable Interrupt
        devices_probe |= WATCH_TOUCH_ONLINE;
    }

    log_println("Init BMA423");
    res = SensorBMA423::init(Wire);
    if (!res) {
        log_println("Failed to find BMA423 - check your wiring!");
    } else {
        log_println("Initializing BMA423 succeeded");
        setReampAxes(REMAP_BOTTOM_LAYER_TOP_RIGHT_CORNER);
        setStepCounterWatermark(1);
        devices_probe |= WATCH_BMA_ONLINE;
    }

    log_println("Init PCF8563 RTC");
    res = SensorPCF8563::init(Wire);
    if (!res) {
        log_println("Failed to find PCF8563 - check your wiring!");
    } else {
        log_println("Initializing PCF8563 succeeded");
        disableCLK();   //Disable clock output ， Conserve Backup Battery Current Consumption
        hwClockRead();  //Synchronize RTC clock to system clock
        devices_probe |= WATCH_RTC_ONLINE;
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
        devices_probe |= WATCH_DRV_ONLINE;
    }

    log_println("Init GPS");
    res = initGPS();
    if (res) {
        log_println("UBlox GPS init succeeded, using UBlox GPS Module\n");
        devices_probe |= WATCH_GPS_ONLINE;
    } else {
        log_println("Warning: Failed to find UBlox GPS Module\n");
        // if not detect gps , turn off dc3
        disableDC3();
    }

#ifdef USING_TWATCH_S3
    log_println("Init Radio SPI Bus");
    radioBus.begin(BOARD_RADIO_SCK,
                   BOARD_RADIO_MISO,
                   BOARD_RADIO_MOSI);
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
    if (!brightness && level != 0) {
        enableALDO2();
        writecommand(0x11);  //display wakeup
    }
    brightness = level;
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5,0,0)
    ledcWrite(BOARD_TFT_BL, brightness);
#else
    ledcWrite(LEDC_BACKLIGHT_CHANNEL, brightness);
#endif
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
    setVbusCurrentLimit(XPOWERS_AXP2101_VBUS_CUR_LIM_900MA);

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

    //! GPS Power
    setDC3Voltage(3300);
    enableDC3();

    //! No use
    disableDC2();
    // disableDC3();
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


    // Enable the required interrupt function
    enableIRQ(
        XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |   //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
    );

    // Clear all interrupt flags
    XPowersAXP2101::clearIrqStatus();

    // Set the precharge charging current
    setPrechargeCurr(XPOWERS_AXP2101_PRECHARGE_50MA);
    // Set constant current charge current limit
    setChargerConstantCurr(XPOWERS_AXP2101_CHG_CUR_300MA);
    // Set stop charging termination current
    setChargerTerminationCurr(XPOWERS_AXP2101_CHG_ITERM_25MA);

    // Set charge cut-off voltage
    setChargeTargetVoltage(XPOWERS_AXP2101_CHG_VOL_4V35);

    // Set RTC Battery voltage to 3.3V
    setButtonBatteryChargeVoltage(3300);

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
    // disableDC3();    //! GPS Power
}

void LilyGoLib::highPower()
{
    // enableALDO1();  //! RTC VBAT
    // enableALDO2();  //! TFT BACKLIGHT   VDD
    // enableALDO3();  //! Screen touch VDD
    // enableALDO4();  //! Radio VDD
    // enableBLDO2();  //! drv2605 enable
    // enableDC3();    //! GPS Power
}

void LilyGoLib::powerIoctl(enum PowerCtrlChannel ch, bool enable)
{
    switch (ch) {
    case WATCH_POWER_DISPLAY_BL:
        enable ? enableALDO2() : disableALDO2();
        break;
    case WATCH_POWER_TOUCH_DISP:
        enable ? enableALDO3() : disableALDO3();
        break;
    case WATCH_POWER_RADIO:
        enable ? enableALDO4() : disableALDO4();
        break;
    case WATCH_POWER_DRV2605:
        enable ? enableBLDO2() : disableBLDO2();
        break;
    case WATCH_POWER_GPS:
        enable ? enableDC3() : disableDC3();
        break;
    default:
        break;
    }
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

void LilyGoLib::sleepLora(bool config)
{
#ifdef USING_TWATCH_S3
    // SX126x::sleep(config);
#endif
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

struct uBloxGnssModelInfo { // Structure to hold the module info (uses 341 bytes of RAM)
    char softVersion[30];
    char hardwareVersion[10];
    uint8_t extensionNo = 0;
    char extension[10][30];
} ;


bool LilyGoLib::gpsProbe()
{
    uint8_t buffer[256];
    bool legacy_ubx_message = true;
    struct uBloxGnssModelInfo info ;
    //  Get UBlox GPS module version
    uint8_t cfg_get_hw[] =  {0xB5, 0x62, 0x0A, 0x04, 0x00, 0x00, 0x0E, 0x34};
    GPSSerial.write(cfg_get_hw, sizeof(cfg_get_hw));

    uint16_t len = getAck(buffer, 256, 0x0A, 0x04);
    if (len) {
        memset(&info, 0, sizeof(info));
        uint16_t position = 0;
        for (int i = 0; i < 30; i++) {
            info.softVersion[i] = buffer[position];
            position++;
        }
        for (int i = 0; i < 10; i++) {
            info.hardwareVersion[i] = buffer[position];
            position++;
        }

        while (len >= position + 30) {
            for (int i = 0; i < 30; i++) {
                info.extension[info.extensionNo][i] = buffer[position];
                position++;
            }
            info.extensionNo++;
            if (info.extensionNo > 9)
                break;
        }

        log_i("Module Info : ");
        log_i("Soft version: %s", info.softVersion);
        log_i("Hard version: %s", info.hardwareVersion);
        log_i("Extensions: %d", info.extensionNo);
        for (int i = 0; i < info.extensionNo; i++) {
            log_i("%s", info.extension[i]);
        }
        log_i("Model:%s", info.extension[2]);

        for (int i = 0; i < info.extensionNo; ++i) {
            if (!strncmp(info.extension[i], "OD=", 3)) {
                strcpy((char *)buffer, &(info.extension[i][3]));
                log_i("GPS Model: %s", (char *)buffer);
            }
        }
        return true;
    }
    return false;
}


int LilyGoLib::getAck(uint8_t *buffer, uint16_t size, uint8_t requestedClass, uint8_t requestedID)
{
    uint16_t    ubxFrameCounter = 0;
    bool        ubxFrame = 0;
    uint32_t    startTime = millis();
    uint16_t    needRead;

    while (millis() - startTime < 800) {
        while (GPSSerial.available()) {
            int c = GPSSerial.read();
            switch (ubxFrameCounter) {
            case 0:
                if (c == 0xB5) {
                    ubxFrameCounter++;
                }
                break;
            case 1:
                if (c == 0x62) {
                    ubxFrameCounter++;
                } else {
                    ubxFrameCounter = 0;
                }
                break;
            case 2:
                if (c == requestedClass) {
                    ubxFrameCounter++;
                } else {
                    ubxFrameCounter = 0;
                }
                break;
            case 3:
                if (c == requestedID) {
                    ubxFrameCounter++;
                } else {
                    ubxFrameCounter = 0;
                }
                break;
            case 4:
                needRead = c;
                ubxFrameCounter++;
                break;
            case 5:
                needRead |=  (c << 8);
                ubxFrameCounter++;
                break;
            case 6:
                if (needRead >= size) {
                    ubxFrameCounter = 0;
                    break;
                }
                if (GPSSerial.readBytes(buffer, needRead) != needRead) {
                    ubxFrameCounter = 0;
                } else {
                    return needRead;
                }
                break;

            default:
                break;
            }
        }
    }
    return 0;
}

bool LilyGoLib::initGPS()
{
    GPSSerial.begin(38400, SERIAL_8N1, SHIELD_GPS_RX, SHIELD_GPS_TX);
    return gpsProbe();
}

bool LilyGoLib::factoryGPS()
{
    uint8_t buffer[256];
    // Revert module Clear, save and load configurations
    // B5 62 06 09 0D 00 FF FB 00 00 00 00 00 00  FF FF 00 00 17 2B 7E
    uint8_t _legacy_message_reset[] = { 0xB5, 0x62, 0x06, 0x09, 0x0D, 0x00, 0xFF, 0xFB, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0xFF, 0xFF, 0x00, 0x00, 0x17, 0x2B, 0x7E };
    GPSSerial.write(_legacy_message_reset, sizeof(_legacy_message_reset));
    if (!getAck(buffer, 256, 0x05, 0x01)) {
        return false;
    }
    delay(50);

    // UBX-CFG-RATE, Size 8, 'Navigation/measurement rate settings'
    uint8_t cfg_rate[] = {0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30};
    GPSSerial.write(cfg_rate, sizeof(cfg_rate));
    if (!getAck(buffer, 256, 0x06, 0x08)) {
        return false;
    }
    log_i("GPS reset successes!");
    return true;
}



LilyGoLib watch;



















