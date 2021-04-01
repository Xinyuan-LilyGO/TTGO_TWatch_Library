/**
 * @file lilygo main.cpp
 *
 */
/*********************
 *      INCLUDES
 *********************/
#include "config.h"
#include <WiFi.h>

#if defined(LILYGO_WATCH_2020_V1)||defined(LILYGO_WATCH_2020_V3)
#include <driver/i2s.h>
#include "HTTPClient.h"
#include "AudioFileSourcePROGMEM.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "image.h"
#include "image1.h"
#endif

/**********************
 *      MACROS
 **********************/
#define SerialMon                   Serial

#define MAX30105_ADDRESS            0x57
#define PCF8563_ADDRESS             0x51
#define BMA423_ADDRESS              0x19
#define AXP202_ADDRESS              0x35
#define FT6206_ADDRESS              0X38
#define MPU6050_ADDRESS             0x68
#define MPR121_ADDRESS              0x5A
#define DRV2605_ADDRESS             0x5A

/**********************
 *  STATIC VARIABLES
 **********************/
static TTGOClass        *ttgo = nullptr;
static TFT_eSPI         *tft = nullptr;
static BMA              *bma = nullptr;
static PCF8563_Class    *rtc = nullptr;
static CapacitiveTouch  *tp = nullptr;
static AXP20X_Class     *pmu = nullptr;
static Buzzer           *buzzer = nullptr ;

#ifdef LILYGO_WATCH_2020_V2
static Adafruit_DRV2605 *drv;
static TinyGPSPlus *gps;
#endif


static bool find_pwr, find_bma, find_pcf, find_mpu, find_max, find_tp, find_mpr, find_drv;
static bool ret, bma_irq, pmu_irq;
static uint32_t last = 0;
static char buf[256];
static uint32_t montiorMillis = 0;
static uint16_t x;
static uint16_t y;

#define BUFFER_SIZE (2 * TFT_WIDTH)
uint8_t buffer[BUFFER_SIZE] = {0};
uint16_t *ptr = nullptr;
uint16_t prevX[TFT_WIDTH];
uint16_t prevY[TFT_WIDTH];

/**********************
 *   STATIC FUNCTIONS
 **********************/
static void display_info(void);
static void devScan(TwoWire &w);
static void resultOutput(String str, bool ret);
static void wifiScan(void);

#ifdef LILYGO_WATCH_2020_V3

//!  PDM MIC BOARD
#define MIC_DATA            2
#define MIC_CLOCK           0

void setupMic()
{
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM),
        .sample_rate =  44100,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),//I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 2,
        .dma_buf_len = 128,
    };

    i2s_pin_config_t i2s_cfg;
    i2s_cfg.bck_io_num   = I2S_PIN_NO_CHANGE;
    i2s_cfg.ws_io_num    = MIC_CLOCK;
    i2s_cfg.data_out_num = I2S_PIN_NO_CHANGE;
    i2s_cfg.data_in_num  = MIC_DATA;

    i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    i2s_set_pin(I2S_NUM_0, &i2s_cfg);
    i2s_set_clk(I2S_NUM_0, 44100, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}



void showSignal()
{
    int x = 0, y = 0;
    for (int i = 0; i < 240; i++) {
        x = i;
        y = map(ptr[i], 0, 65535, 0, 240);
        tft->drawPixel(prevX[i], prevY[i], TFT_WHITE);
        tft->drawPixel(x, y, TFT_BLACK);
        prevX[i] = x;
        prevY[i] = y;
    }
}
#endif


bool pmuIRQ()
{
    bool rlst = false;
    if (pmu_irq) {
        pmu_irq = false;
        pmu->readIRQ();
        if (pmu->isPEKShortPressIRQ()) {
            rlst = true;
        }
        pmu->clearIRQ();
    }
    return rlst;
}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
void setup()
{
    SerialMon.begin(115200);

    setCpuFrequencyMhz(240);

    ttgo = TTGOClass::getWatch();

    ttgo->begin();

    ttgo->openBL();

    tft = ttgo->tft;

    tp = ttgo->touch;

#ifdef LILYGO_WATCH_2020_V2
    gps = ttgo->gps;
    drv = ttgo->drv;
#endif

    bma = ttgo->bma;

    rtc = ttgo->rtc;

    pmu = ttgo->power;

    //! i2c device scan
    devScan(Wire);
    devScan(Wire1);

    if (find_pwr) {
        Serial.println("OK");
        pinMode(AXP202_INT, INPUT);
        attachInterrupt(AXP202_INT, [] {
            pmu_irq = true;
        }, FALLING);

        ttgo->power->adc1Enable(AXP202_BATT_CUR_ADC1 | AXP202_BATT_VOL_ADC1 |
                                AXP202_VBUS_VOL_ADC1 | AXP202_VBUS_CUR_ADC1,
                                true);
        ttgo->power->enableIRQ(AXP202_VBUS_REMOVED_IRQ | AXP202_VBUS_CONNECT_IRQ |
                               AXP202_BATT_REMOVED_IRQ | AXP202_BATT_CONNECT_IRQ |
                               AXP202_PEK_LONGPRESS_IRQ | AXP202_PEK_SHORTPRESS_IRQ,
                               true);
        ttgo->power->clearIRQ();
    }
    tft->fillScreen(TFT_BLACK);

#ifdef LILYGO_WATCH_2020_V3
    setupMic();

    size_t read_len = 0;

    while (1) {
        if (pmuIRQ()) {
            Serial.println("PMU IRQ");
            break;
        }
        i2s_read(I2S_NUM_0, (char *) buffer, BUFFER_SIZE, &read_len, 100 / portTICK_RATE_MS);
        ptr = (uint16_t *)buffer;
        showSignal();
        delay(20);
    }
    i2s_driver_uninstall(I2S_NUM_0);
#endif

#if defined(LILYGO_WATCH_2020_V1)||defined(LILYGO_WATCH_2020_V3)
    tft->fillScreen(TFT_BLACK);
    tft->setCursor(0, 0);
    tft->println("Play Audio!");

    ttgo->enableAudio();

    ttgo->motor_begin();
    int i = 3;
    do {
        ttgo->motor->onec();
        delay(500);
    } while (i--);

    AudioGeneratorMP3 *mp3;
    AudioFileSourcePROGMEM *file;
    AudioOutputI2S *out;
    AudioFileSourceID3 *id3;
    // file = new AudioFileSourcePROGMEM(image, sizeof(image));
    file = new AudioFileSourcePROGMEM(image1, sizeof(image1));
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S();
    out->SetPinout(TWATCH_DAC_IIS_BCK, TWATCH_DAC_IIS_WS, TWATCH_DAC_IIS_DOUT);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);
    while (1) {
        if (mp3->isRunning()) {
            if (!mp3->loop()) {
                mp3->stop();
            }
        } else {
            Serial.printf("MP3 done\n");
            break;
        }
    }
    delete file;
    delete id3;
    delete out;
    delete mp3;

    ttgo->disableAudio();
#endif

    //! Screen test
    tft->fillScreen(TFT_BLACK);
    delay(500);
    tft->fillScreen(TFT_RED);
    delay(500);
    tft->fillScreen(TFT_GREEN);
    delay(500);
    tft->fillScreen(TFT_BLUE);
    delay(500);
    tft->fillScreen(TFT_BLACK);
    delay(500);

    wifiScan();

    delay(5000);
    tft->fillScreen(TFT_BLACK);
    tft->setTextFont(2);
    tft->setCursor(0, 0);
    tft->println("Touch test, click the button to skip");
    while (1) {
        if (pmuIRQ()) {
            break;
        }
        if (tp->getPoint(x, y)) {
            tft->fillCircle(x, y, 2, TFT_GREEN);
        }
    }

    tft->fillScreen(TFT_BLACK);
    tft->setTextColor(TFT_WHITE);
    tft->setCursor(0, 0);


    resultOutput("Find AXP202 Chip", find_pwr);
    resultOutput("Find BMA423 Chip", find_bma);
    resultOutput("Find PCF8563 Chip", find_pcf);
    resultOutput("Find TP62XX Chip", find_tp);

    if (find_bma) {
        ret = ttgo->bma->begin();
        resultOutput("Setup BMA423", ret);
        bma->attachInterrupt();
        pinMode(BMA423_INT1, INPUT);
        attachInterrupt(BMA423_INT1, [] {
            bma_irq = true;
        }, RISING);
    }

    if (find_pcf) {
        int yy = 2020, mm = 3, dd = 15, h = 2, m = 10, s = 0;
        rtc->setDateTime(yy, mm, dd, h, m, s);
        RTC_Date dt = rtc->getDateTime();
        ret = dt.year != yy || dt.month != mm || dt.day != dd || dt.hour != h || dt.minute != m;
        resultOutput("Setup PCF8563 ", !ret);
    }

#ifdef LILYGO_WATCH_2020_V2
    if (!ttgo->sdcard_begin()) {
        resultOutput("Setup SDCard ", false);
    } else {
        resultOutput("Setup SDCard ", true);
    }
#endif

    tft->setTextColor(TFT_GREEN);
    tft->drawString("Step Count:", 0, tft->height() - 20);

#if defined(LILYGO_WATCH_2020_V2)
    ttgo->trunOnGPS();

    ttgo->gps_begin();

    gps =  ttgo->gps;

    if (find_drv) {
        ttgo->enableDrv2650(true);
        drv->selectLibrary(1);
        drv->setMode(DRV2605_MODE_INTTRIG);
    }
#elif defined(LILYGO_WATCH_2019_WITH_TOUCH)
    ttgo->enableLDO3();
    ttgo->motor_begin();
    buzzer = new Buzzer(25);
    buzzer->begin();
#endif
}

void loop()
{

#ifdef LILYGO_WATCH_2020_V2
    while (ttgo->hwSerial->available()) {
        int r = ttgo->hwSerial->read();
        Serial.write(r);
        ttgo->gps->encode(r);
    }
    display_info();
#endif

    if (millis() - montiorMillis > 1000 && find_pwr) {
        if (ttgo->power->isBatteryConnect()) {
            snprintf(buf, sizeof(buf), "B:%.2fV/%.2fmA",
                     ttgo->power->getBattVoltage() / 1000.0,
                     ttgo->power->isChargeing() ? ttgo->power->getBattChargeCurrent() : ttgo->power->getBattDischargeCurrent());
        } else {
            snprintf(buf, sizeof(buf), "V:%.2fV/%.2fmA",
                     ttgo->power->getVbusVoltage() / 1000.0,
                     ttgo->power->getVbusCurrent());
        }

        tft->setTextDatum(MC_DATUM);
        x = tft->width() / 2;
        y = tft->height() - 20;

        tft->fillRect(60, y - 50, x, 16, TFT_BLACK);
        tft->drawString(buf, x, y - 40, tft->textfont);
        montiorMillis = millis();
        tft->setTextDatum(TL_DATUM);
    }

    if (pmu_irq) {
        pmu_irq = false;
        ttgo->power->readIRQ();
        if (ttgo->power->isChargingIRQ()) {
            ttgo->setBrightness(255);
        } else {
        }
        if (ttgo->power->isVbusRemoveIRQ()) {
            ttgo->setBrightness(80);
        }
        if (ttgo->power->isPEKLongtPressIRQ()) {
            SerialMon.println("isPEKLongtPressIRQ");
        }
        if (ttgo->power->isPEKShortPressIRQ()) {

#if defined(LILYGO_WATCH_2020_V2)
            if (find_drv) {
                // set the effect to play
                drv->setWaveform(0, 75);  // play effect
                drv->setWaveform(1, 0);       // end waveform
                // play the effect!
                drv->go();
            }
#elif defined(LILYGO_WATCH_2019_WITH_TOUCH)
            ttgo->motor->onec();
            buzzer->onec();
#endif

            if (ttgo->bl->isOn()) {
                ttgo->closeBL();
            } else {
                ttgo->openBL();
            }
        }
        ttgo->power->clearIRQ();
    }

    if (tp->getPoint(x, y)) {
        snprintf(buf, sizeof(buf), "x:%d y:%d", x, y);
        tft->fillRect(tft->width() / 2, tft->height() - 20, tft->width() / 2, tft->height() - 20, TFT_BLACK);
        tft->drawRightString(buf, tft->width() - 5, tft->height() - 20, tft->textfont);
    }

    if (bma_irq) {
        bma_irq = false;
        do {
            ret =  bma->readInterrupt();
        } while (!ret);

        if (bma->isStepCounter()) {
            static uint32_t prevSetp = 0;
            uint32_t c = bma->getCounter();
            if (prevSetp == c)
                return;
            prevSetp = c;
            String str = "Step Count:" + String(c);
            uint16_t twidth = tft->textWidth(str);
            y = tft->height() - 20;
            tft->fillRect(0,
                          y - 20,
                          twidth + 10,
                          40,
                          TFT_BLACK);
            tft->drawString(str, 0, y);
        }
    }
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

#ifdef LILYGO_WATCH_2020_V2
static void display_info(void)
{
    static uint32_t updatePeriod;
    if (millis() - updatePeriod < 1000) {
        return;
    }
    updatePeriod = millis();

    if (gps->date.isUpdated() && gps->time.isUpdated()) {
        tft->fillRect(0, 115, 240, 35, TFT_BLACK);
        tft->setCursor(0, 116);
        tft->print("DATE Year:");
        tft->print(gps->date.year());
        tft->print(" Month:");
        tft->print(gps->date.month());
        tft->print(" Day:");
        tft->println(gps->date.day());

        tft->print(F("TIME Hour:"));
        tft->print(gps->time.hour());
        tft->print(F(" Minute:"));
        tft->print(gps->time.minute());
        tft->print(F(" Second:"));
        tft->print(gps->time.second());

        // SerialMon.print(F("DATE       Fix Age="));
        // SerialMon.print(gps->date.age());
        // SerialMon.print(F("ms Raw="));
        // SerialMon.print(gps->date.value());
        // SerialMon.print(F(" Year="));
        // SerialMon.print(gps->date.year());
        // SerialMon.print(F(" Month="));
        // SerialMon.print(gps->date.month());
        // SerialMon.print(F(" Day="));
        // SerialMon.println(gps->date.day());

        // SerialMon.print(F("TIME       Fix Age="));
        // SerialMon.print(gps->time.age());
        // SerialMon.print(F("ms Raw="));
        // SerialMon.print(gps->time.value());
        // SerialMon.print(F(" Hour="));
        // SerialMon.print(gps->time.hour());
        // SerialMon.print(F(" Minute="));
        // SerialMon.print(gps->time.minute());
        // SerialMon.print(F(" Second="));
        // SerialMon.print(gps->time.second());
        // SerialMon.print(F(" Hundredths="));
        // SerialMon.println(gps->time.centisecond());
    }
    if (gps->location.isUpdated()) {
        tft->fillRect(0, 150, 240, 20, TFT_BLACK);
        tft->setCursor(0, 150);
        tft->print(F("LOCATION  Lat:"));
        tft->print(gps->location.lat(), 3);
        tft->print(F(" Long="));
        tft->println(gps->location.lng(), 3);

        // SerialMon.print(F("LOCATION   Fix Age="));
        // SerialMon.print(gps->location.age());
        // SerialMon.print(F("ms Raw Lat="));
        // SerialMon.print(gps->location.rawLat().negative ? "-" : "+");
        // SerialMon.print(gps->location.rawLat().deg);
        // SerialMon.print("[+");
        // SerialMon.print(gps->location.rawLat().billionths);
        // SerialMon.print(F(" billionths],  Raw Long="));
        // SerialMon.print(gps->location.rawLng().negative ? "-" : "+");
        // SerialMon.print(gps->location.rawLng().deg);
        // SerialMon.print("[+");
        // SerialMon.print(gps->location.rawLng().billionths);
        // SerialMon.print(F(" billionths],  Lat="));
        // SerialMon.print(gps->location.lat(), 6);
        // SerialMon.print(F(" Long="));
        // SerialMon.println(gps->location.lng(), 6);

    }
    if (gps->speed.isUpdated()) {

        // SerialMon.print(F("SPEED      Fix Age="));
        // SerialMon.print(gps->speed.age());
        // SerialMon.print(F("ms Raw="));
        // SerialMon.print(gps->speed.value());
        // SerialMon.print(F(" Knots="));
        // SerialMon.print(gps->speed.knots());
        // SerialMon.print(F(" MPH="));
        // SerialMon.print(gps->speed.mph());
        // SerialMon.print(F(" m/s="));
        // SerialMon.print(gps->speed.mps());
        // SerialMon.print(F(" km/h="));
        // SerialMon.println(gps->speed.kmph());

    }
    if (gps->course.isUpdated()) {

        // SerialMon.print(F("COURSE     Fix Age="));
        // SerialMon.print(gps->course.age());
        // SerialMon.print(F("ms Raw="));
        // SerialMon.print(gps->course.value());
        // SerialMon.print(F(" Deg="));
        // SerialMon.println(gps->course.deg());

    }
    if (gps->altitude.isUpdated()) {

        // SerialMon.print(F("ALTITUDE   Fix Age="));
        // SerialMon.print(gps->altitude.age());
        // SerialMon.print(F("ms Raw="));
        // SerialMon.print(gps->altitude.value());
        // SerialMon.print(F(" Meters="));
        // SerialMon.print(gps->altitude.meters());
        // SerialMon.print(F(" Miles="));
        // SerialMon.print(gps->altitude.miles());
        // SerialMon.print(F(" KM="));
        // SerialMon.print(gps->altitude.kilometers());
        // SerialMon.print(F(" Feet="));
        // SerialMon.println(gps->altitude.feet());

    }
    if (gps->satellites.isUpdated()) {

        // SerialMon.print(F("SATELLITES Fix Age="));
        // SerialMon.print(gps->satellites.age());
        // SerialMon.print(F("ms Value="));
        // SerialMon.println(gps->satellites.value());

    }
    if (gps->hdop.isUpdated()) {

        // SerialMon.print(F("HDOP       Fix Age="));
        // SerialMon.print(gps->hdop.age());
        // SerialMon.print(F("ms Value="));
        // SerialMon.println(gps->hdop.value());

    }
    // if (millis() - last > 5000) {
    //     SerialMon.println();
    //     if (gps->charsProcessed() < 10) {
    //         SerialMon.println(F("WARNING: No GPS data.  Check wiring."));
    //     }
    //     last = millis();
    //     SerialMon.println();
    // }
    delay(20);
}
#endif

static void devScan(TwoWire &w)
{
    uint8_t err, addr;
    int nDevices = 0;
    for (addr = 1; addr < 127; addr++) {
        w.beginTransmission(addr);
        err = w.endTransmission();
        if (err == 0) {
            SerialMon.print("I2C device found at address 0x");
            if (addr < 16)
                SerialMon.print("0");
            SerialMon.print(addr, HEX);
            SerialMon.println(" !");
            switch (addr) {
            case MAX30105_ADDRESS:
                find_max = true;
                break;
            case PCF8563_ADDRESS:
                find_pcf = true;
                break;
            case BMA423_ADDRESS:
                find_bma = true;
                break;
            case AXP202_ADDRESS:
                find_pwr = true;
                break;
            case FT6206_ADDRESS:
                find_tp = true;
                break;
            case MPU6050_ADDRESS:
                find_mpu = true;
                break;
            // case MPR121_ADDRESS:
            //     find_mpr = true;
            //     break;
            case DRV2605_ADDRESS:
                find_drv = true;
            default:
                break;
            }
            nDevices++;
        } else if (err == 4) {
            SerialMon.print("Unknow error at address 0x");
            if (addr < 16)
                SerialMon.print("0");
            SerialMon.println(addr, HEX);
        }
    }

}

static void resultOutput(String str, bool ret)
{
    tft->setTextColor(TFT_WHITE);
    tft->drawString(str, 0, tft->getCursorY());
    tft->setTextColor(ret ? TFT_GREEN : TFT_RED);
    tft->drawRightString(ret ?  "[OK]" :   "[FAIL]", tft->width() - 5, tft->getCursorY(), tft->textfont);
    tft->println();
}

static void wifiScan(void)
{
    char buff[128];
    tft->setCursor(0, 0);
    tft->setTextColor(TFT_GREEN, TFT_BLACK);
    tft->fillScreen(TFT_BLACK);
    tft->setTextDatum(MC_DATUM);
    tft->setTextSize(1);
    tft->drawString("Scan Network", tft->width() / 2, tft->height() / 2);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    int16_t n = WiFi.scanNetworks();
    tft->fillScreen(TFT_BLACK);

    if (n == 0) {
        SerialMon.println("No Search WiFi Signal");
        tft->drawString("No Search WiFi Signal", tft->width() / 2, tft->height() / 2);
    } else {

        tft->setTextDatum(TL_DATUM);
        tft->setCursor(0, 0);
        bool utf8 = false;
        SerialMon.printf("Search [%d] WiFi Signal\n", n);
        uint16_t count = 0;
        for (int i = 0; i < n; ++i) {
            SerialMon.println(WiFi.SSID(i));
            char *ssid = (char *)WiFi.SSID(i).c_str();
            for (int j = 0; j < strlen(ssid); j++) {
                if (ssid[j] > 0x7F) {
                    utf8 = true;
                    break;
                };
            }
            if (!utf8) {
                sprintf(buff,
                        "\t[%d]:%s(%d)",
                        count++,
                        WiFi.SSID(i).c_str(),
                        WiFi.RSSI(i));
                tft->println(buff);
            }
            utf8 = false;
        }
    }
    WiFi.mode(WIFI_OFF);
}













