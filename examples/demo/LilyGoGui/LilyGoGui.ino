/**
 * @file      LilyGoGui.ino
 * @author    zhanguichen
 * @license   MIT
 * @copyright Copyright (c) 2023  Shenzhen Xinyuan Electronic Technology Co., Ltd
 * @note      Arduino Setting
 *            Tools ->
 *                  Board:"ESP32S3 Dev Module"
 *                  USB CDC On Boot:"Enable"
 *                  USB DFU On Boot:"Disable"
 *                  Flash Size : "16MB(128Mb)"
 *                  Flash Mode"QIO 80MHz
 *                  Partition Scheme:"16M Flash(3M APP/9.9MB FATFS)"
 *                  PSRAM:"OPI PSRAM"
 *                  Upload Mode:"UART0/Hardware CDC"
 *                  USB Mode:"Hardware CDC and JTAG"
 */

//! Two transceivers, SX1262 and SX1280, are defined based on the actual model
#define USE_RADIO_SX1280
// #define USE_RADIO_SX1262


#define ENABLE_PLAYER
#define ENABLE_IR_SENDER

#include <LilyGoLib.h>
#include <LV_Helper.h>
#include <WiFi.h>
#ifdef ENABLE_IR_SENDER
#include <IRsend.h>
IRsend irsend(BOARD_IR_PIN);
#endif
#include <driver/i2s.h>
#include <esp_vad.h>
#ifdef ENABLE_PLAYER
#include <AudioFileSourcePROGMEM.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourceFATFS.h>
#include <AudioFileSourceFunction.h>
#include <AudioGeneratorFLAC.h>
#include <AudioGeneratorAAC.h>
#include "esp_sntp.h"
#include "Wire.h"
#include <WiFiClientSecure.h>
#include "global_flags.h"
#include "ui.h"
#include <driver/gpio.h>



#ifdef USE_RADIO_SX1262
SX1262 radio = newModule();
#elif  defined(USE_RADIO_SX1280)
SX1280 radio = newModule();
#endif


extern const unsigned char mp3_array[16509];
extern unsigned char mp3_ring_setup[86144];
AudioFileSourceFATFS   *file_fs;
AudioGeneratorWAV       *wav = NULL;
AudioFileSourcePROGMEM  *file = NULL;
AudioOutputI2S          *out = NULL;
AudioGeneratorMP3       *mp3 = NULL;
AudioFileSourceID3      *id3 = NULL;
AudioGeneratorFLAC      *flac = NULL;
AudioGeneratorAAC       *aac = NULL;

extern lv_obj_t *step_counter_label;
extern lv_obj_t *batt_voltage_label;
extern lv_obj_t *chart;

extern void set_text_radio_ta(const char *txt);

void suspend_playMP3Handler(void);
void resume_playMP3Handler(void);

//lvgl event handler btn index
bool is_pause = false;
// Play task handler
TaskHandle_t playMP3Handler;
TaskHandle_t playWAVHandler;
TaskHandle_t playFLACHandler;
TaskHandle_t playACCHandler;

uint8_t boot_music[4365];
#endif

#define BG_COLOR 0xffffff

#define LVGL_MESSAGE_PROGRESS_CHANGED_ID        (88)
#define DEFAULT_RECORD_FILENAME                 "/rec.wav"
#define AUDIO_DATA                              boot_music
#define RADIO_TRANSMIT_PAGE_ID                  9
#define WIFI_SCAN_PAGE_ID                       8
#define MIC_IR_PAGE_ID                          11

#define DEFAULT_SCREEN_TIMEOUT                  15*1000
#define DEFAULT_COLOR                           (lv_color_make(252, 218, 72))
#define VAD_FRAME_LENGTH_MS                     30
#define VAD_BUFFER_LENGTH                       (VAD_FRAME_LENGTH_MS * MIC_I2S_SAMPLE_RATE / 1000)

#define WIFI_SSID             "Your WiFi SSID"
#define WIFI_PASSWORD         "Your WiFi PASSWORD"

#define WIFI_CONNECT_WAIT_MAX (30 * 1000)

#define NTP_SERVER1           "pool.ntp.org"
#define NTP_SERVER2           "time.nist.gov"
#define GMT_OFFSET_SEC        0
#define DAY_LIGHT_OFFSET_SEC  0
#define GET_TIMEZONE_API      "https://ipapi.co/timezone/"

LV_IMG_DECLARE(arrow_left_png);
LV_IMG_DECLARE(arrow_right_png);
LV_FONT_DECLARE(fn1_32);
LV_FONT_DECLARE(robot_ightItalic_16);
LV_FONT_DECLARE(liquidCrystal_nor_64);
LV_FONT_DECLARE(liquidCrystal_nor_32);
LV_FONT_DECLARE(liquidCrystal_nor_24);
LV_FONT_DECLARE(hansans_cn_24);
LV_FONT_DECLARE(digital_play_st_48);
LV_FONT_DECLARE(quostige_16);
LV_FONT_DECLARE(digital_play_st_24);
LV_FONT_DECLARE(gracetians_32);
LV_FONT_DECLARE(exninja_22);


#ifdef USE_RADIO_SX1262
const char *radio_freq_list =
    "433MHz\n"
    "470MHz\n"
    "868MHz\n"
    "915MHz\n"
    "923MHz";
const float radio_freq_args_list[] = {433.0, 470.0, 868.0, 915.0, 923.0};

const char *radio_bandwidth_list =
    "125KHz\n"
    "250KHz\n"
    "500KHz";
const float radio_bandwidth_args_list[] = {125.0, 250.0, 500.0};

const char *radio_power_level_list =
    "2dBm\n"
    "5dBm\n"
    "10dBm\n"
    "12dBm\n"
    "17dBm\n"
    "20dBm\n"
    "22dBm";
const float radio_power_args_list[] = {2, 5, 10, 12, 17, 20, 22};

#define RADIO_DEFAULT_FREQ          868.0
#define RADIO_DEFAULT_BW            250.0
#define RADIO_DEFAULT_SF            10
#define RADIO_DEFAULT_CR            6
#define RADIO_DEFAULT_CUR_LIMIT     140
#define RADIO_DEFAULT_POWER_LEVEL   22

#elif  defined(USE_RADIO_SX1280)

#define RADIO_DEFAULT_FREQ          2492.0
#define RADIO_DEFAULT_BW            203.125
#define RADIO_DEFAULT_SF            10
#define RADIO_DEFAULT_CR            6
#define RADIO_DEFAULT_POWER_LEVEL   13

const char *radio_freq_list =
    "2400MHz\n"
    "2412MHz\n"
    "2422MHz\n"
    "2432MHz\n"
    "2442MHz\n"
    "2452MHz\n"
    "2462MHz\n"
    "2472MHz\n"
    "2482MHz\n"
    "2492MHz\n"
    "2498MHz\n"
    "2500MHz";
const float radio_freq_args_list[] = {2400.0,
                                      2412.0, 2422.0, 2432.0,
                                      2442.0, 2452.0, 2462.0,
                                      2472.0, 2482.0, 2492.0,
                                      2500.0
                                     };

const char *radio_bandwidth_list =
    "203.125KHz\n"
    "406.25KHz\n"
    "812.5KHz\n"
    "1625.0KHz";
const float radio_bandwidth_args_list[] = {203.125, 406.25, 812.5, 1625.0};
const char *radio_power_level_list =

    /* "-18dBm\n"
     "-17dBm\n"
     "-16dBm\n"
     "-15dBm\n"
     "-14dBm\n"
     "-13dBm\n"
     "-12dBm\n"
     "-11dBm\n"
     "-10dBm\n"
     "-9dBm\n"
     "-8dBm\n"
     "-7dBm\n"
     "-6dBm\n"
     "-5dBm\n"
     "-4dBm\n"
     "-3dBm\n"
     "-2dBm\n"
     "-1dBm\n"
     */
    "0dBm\n"
    "1dBm\n"
    "2dBm\n"
    "3dBm\n"
    "4dBm\n"
    "5dBm\n"
    "6dBm\n"
    "7dBm\n"
    "8dBm\n"
    "9dBm\n"
    "10dBm\n"
    "11dBm\n"
    "12dBm\n"
    "13dBm";

const float radio_power_args_list[] = {
    // -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8,
    //     -7, -6, -5, -4, -3, -2, -1,
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13
};
#endif

char standby_en = 1;
LV_FONT_DECLARE(font_siegra);

void radioTask(lv_timer_t *parent);

void my_print(const char *buf)
{
    Serial.printf(buf);
    Serial.flush();
}

lv_obj_t *wifi_test_obj = NULL;
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;
struct tm timeinfo;
struct tm show_timeinfo;
struct tm show_timeinfo_old;




static lv_obj_t *wifi_table_list;
static lv_obj_t *charge_cont;

extern lv_timer_t *transmitTask;

static TaskHandle_t playerTaskHandler;
static TaskHandle_t vadTaskHandler;

// Save the ID of the current page
static uint8_t pageId = 0;
// Flag used to indicate whether to use light sleep, currently unavailable
static bool lightSleep = false;
// Flag used for acceleration interrupt status
static bool sportsIrq = false;
// Flag used to indicate whether recording is enabled
static bool recordFlag = false;
// Flag used for PMU interrupt trigger status
static bool pmuIrq = false;
// Flag used to select whether to turn off the screen
static bool canScreenOff = true;
// Flag used to detect USB insertion status
static bool usbPlugIn = false;
// Flag to indicate that a packet was sent or received
static bool radioTransmitFlag = false;
// Save transmission states between loops
static int transmissionState = RADIOLIB_ERR_NONE;
// Flag to indicate transmission or reception state
static bool transmitFlag = false;
// Save pedometer steps
static uint32_t stepCounter = 0;
// Save brightness value
static RTC_DATA_ATTR int brightnessLevel = 0;
// Vad detecte values
static int16_t *vad_buff = NULL;
static vad_handle_t vad_inst = NULL;

const size_t vad_buffer_size = VAD_BUFFER_LENGTH * sizeof(short);

typedef  struct _lv_datetime {
    lv_obj_t *obj;
    const char *name;
    uint16_t minVal;
    uint16_t maxVal;
    uint16_t defaultVal;
    uint8_t digitFormat;
} lv_datetime_t;

#define LV_DELAY(x)                                                                                                                                  \
  do {                                                                                                                                               \
    uint32_t t = x;                                                                                                                                  \
    while (t--) {                                                                                                                                    \
      lv_timer_handler();                                                                                                                            \
      delay(1);                                                                                                                                      \
    }                                                                                                                                                \
  } while (0);


const char *cn_week[7] = { "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
const char *cn_month[12] = { "Jan", "Feb", "Mar", "Ari", "May", "Jun", "Jul", "Aut", "Sep", "Oct", "Nov", "Dec"};
const char *cn_state[5] = {"a.m.", "mid", "p.m.", "night", "night"};

typedef bool (*player_cb_t)(void);
static player_cb_t player_task_cb = NULL;
//static bool playWAV();
static bool playMP3();

lv_obj_t *setupGUI(void);
EventGroupHandle_t global_event_group;
QueueHandle_t led_setting_queue;
QueueHandle_t led_flicker_queue;
QueueHandle_t play_music_queue;
QueueHandle_t play_time_queue;
static EventGroupHandle_t lv_input_event;

void factory_ui();
void wifi_test(void);
void wifiscan(lv_obj_t *parent);

void settingPMU();
void settingSensor();
void settingRadio();
void settingPlayer();
void settingIRRemote();

void PMUHandler();
void lowPowerEnergyHandler();
void destoryChargeUI();

void radioTask(lv_timer_t *parent);
static void PDM_Record(const char *song_name, uint32_t duration);
static bool CreateWAV(const char *song_name, uint32_t duration, uint16_t num_channels, const uint32_t sampling_rate, uint16_t bits_per_sample);

void WiFiScanDone(WiFiEvent_t event, WiFiEventInfo_t info)
{
    Serial.println("WiFiScanDone");
    if (pageId != WIFI_SCAN_PAGE_ID) {
        canScreenOff = true;
        WiFi.removeEvent(WiFiEvent_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
        WiFi.mode(WIFI_OFF);
        return;
    }

    int16_t counter =  WiFi.scanComplete();

    lv_table_set_row_cnt(wifi_table_list, counter);

    for (int i = 0; i < counter; ++i) {

        lv_table_set_cell_value_fmt(wifi_table_list, i, 0,
                                    LV_SYMBOL_WIFI"[%4d]"" %-10.10s",
                                    WiFi.RSSI(i),
                                    WiFi.SSID(i).c_str()
                                   );

        // Print SSID and RSSI for each network found
        Serial.printf("%2d", i + 1);
        Serial.print(" | ");
        Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
        Serial.print(" | ");
        Serial.printf("%4d", WiFi.RSSI(i));
        Serial.print(" | ");
        Serial.printf("%2d", WiFi.channel(i));
        Serial.print(" | ");
        switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
            Serial.print("open");
            break;
        case WIFI_AUTH_WEP:
            Serial.print("WEP");
            break;
        case WIFI_AUTH_WPA_PSK:
            Serial.print("WPA");
            break;
        case WIFI_AUTH_WPA2_PSK:
            Serial.print("WPA2");
            break;
        case WIFI_AUTH_WPA_WPA2_PSK:
            Serial.print("WPA+WPA2");
            break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
            Serial.print("WPA2-EAP");
            break;
        case WIFI_AUTH_WPA3_PSK:
            Serial.print("WPA3");
            break;
        case WIFI_AUTH_WPA2_WPA3_PSK:
            Serial.print("WPA2+WPA3");
            break;
        case WIFI_AUTH_WAPI_PSK:
            Serial.print("WAPI");
            break;
        default:
            Serial.print("unknown");
        }
        Serial.println();
    }
    //Keep scan
    WiFi.scanNetworks(true);
    canScreenOff = false;
}

void setup()
{
    // Stop wifi
    WiFi.mode(WIFI_MODE_NULL);

    btStop();

    setCpuFrequencyMhz(160);

    Serial.begin(115200);

    watch.begin();

    watch.initMicrophone();

    settingPMU();

    settingSensor();

    settingRadio();

    settingPlayer();

    settingIRRemote();

    beginLvglHelper(false);

    //settingButtonStyle();
    transmitTask =  lv_timer_create(radioTask, 200, NULL);
    lv_timer_pause(transmitTask);
    //factory_ui();
    global_event_group = xEventGroupCreate();
    led_setting_queue = xQueueCreate(5, sizeof(uint16_t));
    led_flicker_queue = xQueueCreate(5, sizeof(uint16_t));
    play_music_queue = xQueueCreate(5, sizeof(String));
    play_time_queue = xQueueCreate(5, sizeof(uint32_t));
    lv_input_event = xEventGroupCreate();

    usbPlugIn =  watch.isVbusIn();
    sntp_servermode_dhcp(1);    // (optional)
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);
    // wifi_test();

    // while (show_timeinfo.tm_year <= 0) {
    //     printLocalTime();
    // }

    setupGUI();
    // if (wifi_test_obj != NULL)
    //     lv_obj_del_delayed(wifi_test_obj, 1);

    // xTaskCreate(playMP3Task, "mp3", 8192, NULL, 10, &playMP3Handler);
    // xTaskCreatePinnedToCore(wav_task, "wav_task", 1024 * 4, NULL, 2, NULL, 0);
}

void suspend_playMP3Handler(void)
{
    vTaskSuspend(playMP3Handler);
}

void resume_playMP3Handler(void)
{
    vTaskResume(playMP3Handler);
}

#define LV_COLOR_WHITE LV_COLOR_MAKE(0xFF, 0xFF, 0xFF)
#define LV_COLOR_SILVER LV_COLOR_MAKE(0xC0, 0xC0, 0xC0)
#define LV_COLOR_GRAY LV_COLOR_MAKE(0x80, 0x80, 0x80)
#define LV_COLOR_BLACK LV_COLOR_MAKE(0x00, 0x00, 0x00)
#define LV_COLOR_RED LV_COLOR_MAKE(0xFF, 0x00, 0x00)
#define LV_COLOR_MAROON LV_COLOR_MAKE(0x80, 0x00, 0x00)
#define LV_COLOR_YELLOW LV_COLOR_MAKE(0xFF, 0xFF, 0x00)
#define LV_COLOR_OLIVE LV_COLOR_MAKE(0x80, 0x80, 0x00)
#define LV_COLOR_LIME LV_COLOR_MAKE(0x00, 0xFF, 0x00)
#define LV_COLOR_GREEN LV_COLOR_MAKE(0x00, 0x80, 0x00)
#define LV_COLOR_CYAN LV_COLOR_MAKE(0x00, 0xFF, 0xFF)
#define LV_COLOR_AQUA LV_COLOR_CYAN
#define LV_COLOR_TEAL LV_COLOR_MAKE(0x00, 0x80, 0x80)
#define LV_COLOR_BLUE LV_COLOR_MAKE(0x00, 0x00, 0xFF)
#define LV_COLOR_NAVY LV_COLOR_MAKE(0x00, 0x00, 0x80)
#define LV_COLOR_MAGENTA LV_COLOR_MAKE(0xFF, 0x00, 0xFF)
#define LV_COLOR_PURPLE LV_COLOR_MAKE(0x80, 0x00, 0x80)
#define LV_COLOR_ORANGE LV_COLOR_MAKE(0xFF, 0xA5, 0x00)
lv_obj_t *mainGUI(void);
lv_obj_t *hour = NULL;
lv_obj_t *minute = NULL;
lv_obj_t *second = NULL;
lv_obj_t *year = NULL;
lv_obj_t *month = NULL;
lv_obj_t *week = NULL;
lv_obj_t *day = NULL;
lv_obj_t *state = NULL;

static void but_implement(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *obj = lv_obj_get_parent(btn);

    switch ((int)event->user_data) {
    case 1: {
        mainGUI();
        lv_obj_del_delayed(obj, 1);
        second = NULL;
    }
    break;
    case 2: {
        lv_obj_del_delayed(obj, 1);
        mainGUI();
        second = NULL;
    }
    break;
    case 3: {
        lv_obj_del_delayed(obj, 1);
        mainGUI();
        second = NULL;
    }
    break;
    case 4: {
        lv_obj_del_delayed(obj, 1);
    }
    break;
    case 5: {
        lv_obj_del_delayed(obj, 1);
        lv_example_calendar_1(NULL);
    }
    break;
    case 6: {
        lv_obj_del_delayed(obj, 1);
        setupGUI();
    }
    break;
    case 10: {
        lv_obj_del_delayed(obj, 1);
        mainGUI();
    }
    break;
    default:
        break;
    }
}

static void event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_current_target(e);

    if (code == LV_EVENT_VALUE_CHANGED) {
        lv_calendar_date_t date;
        if (lv_calendar_get_pressed_date(obj, &date)) {
            LV_LOG_USER("Clicked date: %02d.%02d.%d", date.day, date.month, date.year);
        }
    }
}

void lv_example_calendar_1(lv_obj_t *parent)
{
    //lv_obj_t * obj = lv_obj_create(lv_layer_top());
    lv_obj_t *obj = lv_obj_create(parent);
    lv_obj_set_size(obj, 240, 240);
    lv_obj_align(obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x000000), 0);
    lv_obj_set_style_border_width(obj, 0, 0);
    lv_obj_set_style_pad_all(obj, 0, 0);

    lv_obj_t   *calendar = lv_calendar_create(obj);
    lv_obj_set_size(calendar, 240, 185);
    lv_obj_align(calendar, LV_ALIGN_CENTER, 0, 25);
    lv_obj_add_event_cb(calendar, event_handler, LV_EVENT_ALL, NULL);

    lv_calendar_set_today_date(calendar, 2021, 02, 23);
    lv_calendar_set_showed_date(calendar, 2021, 02);

    /*Highlight a few days*/
    static lv_calendar_date_t highlighted_days[3];       /*Only its pointer will be saved so should be static*/
    highlighted_days[0].year = 2021;
    highlighted_days[0].month = 02;
    highlighted_days[0].day = 6;

    highlighted_days[1].year = 2021;
    highlighted_days[1].month = 02;
    highlighted_days[1].day = 11;

    highlighted_days[2].year = 2022;
    highlighted_days[2].month = 02;
    highlighted_days[2].day = 22;

    lv_calendar_set_highlighted_dates(calendar, highlighted_days, 3);

#if LV_USE_CALENDAR_HEADER_DROPDOWN
    lv_calendar_header_dropdown_create(calendar);
#elif LV_USE_CALENDAR_HEADER_ARROW
    lv_calendar_header_arrow_create(calendar);
#endif

    lv_calendar_set_showed_date(calendar, show_timeinfo.tm_year + 1900, show_timeinfo.tm_mon + 1);
}

lv_obj_t *dot = NULL;
lv_obj_t *setupGUI()
{
    static lv_style_t cont_style;
    lv_style_init(&cont_style);
    lv_style_init(&cont_style);
    lv_style_set_radius(&cont_style, 12);
    lv_style_set_bg_color(&cont_style, LV_COLOR_WHITE);
    lv_style_set_bg_opa(&cont_style, LV_OPA_COVER);
    lv_style_set_border_width(&cont_style, 0);

    lv_obj_t *view = lv_obj_create(lv_scr_act());
    lv_obj_set_style_pad_all(view, 0, 0);
    lv_obj_set_size(view, 240, 240);
    lv_obj_set_style_bg_color(view, lv_color_hex(0xffffff), 0);
    //lv_obj_add_style(view, &cont_style, 0);

    static lv_style_t onestyle;
    lv_style_init(&onestyle);
    lv_style_set_text_color(&onestyle, LV_COLOR_BLACK);
    // lv_style_set_text_font(&onestyle, &fn1_32);  //Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&onestyle, &lv_font_montserrat_24);

    //Upper left corner logo
    lv_obj_t *casio = lv_label_create(view);
    lv_obj_add_style(casio, &onestyle, 0);
    lv_label_set_text(casio, "LilyGo");
    lv_obj_align(casio, LV_ALIGN_TOP_LEFT, 10, 10);

    //Upper right corner model
    static lv_style_t model_style;
    lv_style_init(&model_style);
    lv_style_set_text_color(&model_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&model_style, &robot_ightItalic_16); //Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&onestyle, &lv_font_montserrat_28);

    lv_obj_t *model = lv_label_create(view);
    lv_obj_add_style(model,  &model_style, 0);
    lv_label_set_text(model, "ESP32-S3");
    lv_obj_align(model, LV_ALIGN_TOP_RIGHT, -10, 15);

    //Line style
    static lv_style_t line_style;
    lv_style_init(&line_style);
    lv_style_set_line_color(&line_style, LV_COLOR_BLACK);
    lv_style_set_line_width(&line_style, 2);
    lv_style_set_line_rounded(&line_style, 1);

    //Top horizontal line
    static lv_point_t line_points[] = { {10, 0}, {230, 0} };
    lv_obj_t *line1;
    line1 = lv_line_create(view);
    lv_line_set_points(line1, line_points, 2);     /*Set the points*/
    lv_obj_add_style(line1, &line_style, 0);
    lv_obj_align(line1, LV_ALIGN_TOP_MID, 0, 45);

    //Bottom horizontal line left
    static lv_point_t line_points1[] = { {0, 0}, {75, 0} };
    lv_obj_t *line2;
    line2 = lv_line_create(view);
    lv_line_set_points(line2, line_points1, 2);     /*Set the points*/
    lv_obj_add_style(line2,  &line_style, 0);
    lv_obj_align(line2, LV_ALIGN_BOTTOM_LEFT, 10, -30);

    //! Bottom line right
    static lv_point_t line_points2[] = { {0, 0}, {75, 0} };
    lv_obj_t *line3;
    line3 = lv_line_create(view);
    lv_line_set_points(line3, line_points2, 2);     /*Set the points*/
    lv_obj_add_style(line3, &line_style, 0);
    lv_obj_align(line3, LV_ALIGN_BOTTOM_RIGHT, -10, -30);

    // Below the horizontal line in the upper left corner
    static lv_style_t text_style;
    lv_style_init(&text_style);
    lv_style_set_text_color(&text_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&text_style, &robot_ightItalic_16);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&text_style, &lv_font_montserrat_16);

    //! light
    lv_obj_t *light_text = lv_label_create(view);
    lv_obj_add_style(light_text, &text_style, 0);
    lv_label_set_text(light_text, "LIGHT");
    lv_obj_align_to(light_text, line1, LV_ALIGN_OUT_BOTTOM_RIGHT, -30, 5);

    //! arrow -> right
    lv_obj_t *img1 = lv_img_create(view);
    lv_img_set_src(img1, &arrow_right_png);
    lv_obj_align_to(img1, line1, LV_ALIGN_OUT_BOTTOM_RIGHT, -10, 5);

    //! arrow down -> left
    lv_obj_t *img2 = lv_img_create(view);
    lv_img_set_src(img2, &arrow_left_png);
    lv_obj_align_to(img2, line2, LV_ALIGN_OUT_TOP_LEFT, 0, -5);

    //! arrow down -> right
    lv_obj_t *img3 = lv_img_create(view);
    lv_img_set_src(img3, &arrow_right_png);
    lv_obj_align_to(img3, line3, LV_ALIGN_OUT_TOP_RIGHT, 0, -5);

    //Intermediate clock time division font
    static lv_style_t time_style;
    lv_style_init(&time_style);
    lv_style_set_text_color(&time_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&time_style,  &digital_play_st_48);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&time_style, &lv_font_montserrat_48);
    //lv_style_set_text_align(&time_style, LV_ALIGN_RIGHT_MID);
    lv_style_set_text_letter_space(&time_style, 5);

    hour = lv_label_create(view);
    lv_obj_add_style(hour, &time_style, 0);
    char hour_t[30] = { 0 };
    sprintf(hour_t, "%02d", show_timeinfo.tm_hour + 6 >= 24 ? show_timeinfo.tm_hour + 6 - 24 : show_timeinfo.tm_hour + 6);
    lv_label_set_text(hour, hour_t);
    //lv_obj_align_to(hour, view, LV_ALIGN_CENTER, -50, 10);
    lv_obj_set_pos(hour, 8, 90);
    lv_obj_set_size(hour, 100, 50);
    lv_obj_set_style_text_align(hour, LV_TEXT_ALIGN_RIGHT, 0);
    //semicolon
    static lv_style_t dot_style;
    lv_style_init(&dot_style);
    lv_style_set_text_color(&dot_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&dot_style, &liquidCrystal_nor_64);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&dot_style, &lv_font_montserrat_48);

    dot = lv_label_create(view);
    lv_obj_add_style(dot, &dot_style, 0);
    lv_label_set_text(dot, ":");
    lv_obj_align_to(dot, hour, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    //minute
    minute = lv_label_create(view);
    lv_obj_add_style(minute, &time_style, 0);
    char minute_t[30] = { 0 };
    sprintf(minute_t, "%02d", show_timeinfo.tm_min);
    lv_label_set_text(minute, minute_t);
    lv_obj_set_size(minute, 90, 50);
    lv_obj_align_to(minute, dot, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_text_align(minute, LV_TEXT_ALIGN_LEFT, 0);

    //Intermediate clock second digit
    static lv_style_t second_style;
    lv_style_init(&second_style);
    lv_style_set_text_color(&second_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&second_style, &liquidCrystal_nor_32);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&dot_style, &lv_font_montserrat_32);

    second = lv_label_create(view);
    lv_obj_add_style(second, &second_style, 0);
    char second_t[30] = { 0 };
    sprintf(second_t, "%02d", show_timeinfo.tm_sec);
    lv_label_set_text(second, second_t);
    lv_obj_set_size(second, 32, 32);
    lv_obj_align_to(second, minute, LV_ALIGN_OUT_RIGHT_BOTTOM, -12, 13);

    //date
    static lv_style_t year_style;
    lv_style_init(&year_style);
    lv_style_set_text_color(&year_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&year_style, &liquidCrystal_nor_24);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&year_style, &lv_font_montserrat_24);

    year = lv_label_create(view);
    lv_obj_add_style(year, &year_style, 0);
    char year_t[30] = { 0 };
    sprintf(year_t, "%04d", show_timeinfo.tm_year + 1900);
    lv_label_set_text(year, year_t);
    lv_obj_align_to(year, view, LV_ALIGN_CENTER, 0, 55);

    //Chinese font
    static lv_style_t chinese_style;
    lv_style_init(&chinese_style);
    lv_style_set_text_color(&chinese_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&chinese_style, &hansans_cn_24);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&year_style, &lv_font_montserrat_24);

    month = lv_label_create(view);
    lv_obj_add_style(month, &chinese_style, 0);
    lv_label_set_text(month, cn_month[show_timeinfo.tm_mon]);
    lv_obj_align_to(month, year, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    state = lv_label_create(view);
    lv_obj_add_style(state, &chinese_style, 0);
    int hour_temp = show_timeinfo.tm_hour + 6 >= 24 ? show_timeinfo.tm_hour + 6 - 24 : show_timeinfo.tm_hour + 6;
    if (hour_temp >= 8 && hour_temp < 11) {
        lv_label_set_text(state, "a.m.");
    } else if (hour_temp >= 11 && hour_temp <= 13) {
        lv_label_set_text(state, "mid");
    } else if (hour_temp >= 14 && hour_temp <= 17) {
        lv_label_set_text(state, "p.m.");
    } else {
        lv_label_set_text(state, "night");
    }
    lv_obj_align_to(state, year, LV_ALIGN_OUT_RIGHT_MID, 5, 0);

    week = lv_label_create(view);
    lv_obj_add_style(week, &chinese_style, 0);
    lv_label_set_text(week, cn_week[show_timeinfo.tm_wday - 1 >= 0 ? show_timeinfo.tm_wday - 1 : 0]);
    lv_obj_align_to(week, view, LV_ALIGN_CENTER, 45, -35);

    day = lv_label_create(view);
    lv_obj_add_style(day, &year_style, 0);
    char date_t[30] = { 0 };
    sprintf(date_t, "%02d", show_timeinfo.tm_mday);
    lv_label_set_text(day, date_t);
    lv_obj_align_to(day, week, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    //! Battery string
    lv_obj_t *bat_text = lv_label_create(view);
    lv_obj_add_style(bat_text, &text_style, 0);
    lv_label_set_text(bat_text, "BAT");
    lv_obj_align_to(bat_text, view, LV_ALIGN_LEFT_MID, 20, -40);

    // temperature
    static lv_style_t temp_style;
    lv_style_init(&temp_style);
    lv_style_set_text_color(&temp_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&temp_style, &quostige_16);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&temp_style, &lv_font_montserrat_16);


    lv_obj_t *temp_text = lv_label_create(view);
    lv_obj_add_style(temp_text, &temp_style, 0);
    lv_label_set_text(temp_text, "24*C");
    lv_obj_align_to(temp_text, bat_text, LV_ALIGN_OUT_BOTTOM_MID, 0, -5);

    //Power
    static lv_style_t bat_style;
    lv_style_init(&bat_style);
    lv_style_set_text_color(&bat_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&bat_style, &digital_play_st_24);//Due to upgrading the lvgl version, the font is invalid and replaced with ordinary fonts.
    lv_style_set_text_font(&bat_style, &lv_font_montserrat_16);

    lv_obj_t *bat = lv_label_create(view);
    lv_obj_add_style(bat, &bat_style, 0);
    lv_label_set_text(bat, "100%");
    lv_obj_align_to(bat, view, LV_ALIGN_CENTER, -35, -35);


    static lv_point_t line_points3[] = {
        {0, 0}, {50, 0},
        {50, 30}, {50, 30},
        {50, 35}, {35, 39},
        {35, 39}, {15, 39},
        {15, 39}, {0, 35},
        {0, 35}, {0, 0},
    };

    lv_obj_t *line4;
    line4 = lv_line_create(view);
    lv_line_set_points(line4, line_points3, sizeof(line_points3) / sizeof(line_points3[0]));
    lv_obj_add_style(line4, &line_style, 0);
    lv_obj_align(line4, LV_ALIGN_BOTTOM_MID, 0, -2);

    // Bottom Go
    static lv_style_t key_style;
    lv_style_init(&key_style);
    lv_style_set_text_color(&key_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&key_style, &gracetians_32);
    lv_style_set_text_font(&key_style, &lv_font_montserrat_32);

    lv_obj_t *key = lv_label_create(view);
    lv_obj_add_style(key, &key_style, 0);
    lv_label_set_text(key, "Go");
    lv_obj_add_flag(key, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(key, but_implement, LV_EVENT_CLICKED, (void *)1);

    lv_obj_align(key, LV_ALIGN_BOTTOM_MID, 0, -10);

    static lv_style_t bot_style;
    lv_style_init(&bot_style);
    lv_style_set_text_color(&bot_style, LV_COLOR_BLACK);
    // lv_style_set_text_font(&bot_style, &exninja_22);
    lv_style_set_text_font(&bot_style, &lv_font_montserrat_22);

    lv_obj_t *str1 = lv_label_create(view);
    lv_obj_add_style(str1, &bot_style, 0);
    lv_label_set_text(str1, "WATER");
    lv_obj_align(str1, LV_ALIGN_BOTTOM_LEFT, 10, -2);

    lv_obj_t *str2 = lv_label_create(view);
    lv_obj_add_style(str2, &bot_style, 0);
    lv_label_set_text(str2, "RESIST");
    lv_obj_align(str2, LV_ALIGN_BOTTOM_RIGHT, -10, -2);

    return view;
}

void wav_task(void *param)
{
    String music_path;

    uint32_t time_pos;


    while (1) {
        EventBits_t bit = xEventGroupGetBits(global_event_group);
        if (bit) {
            if (bit & RING_PAUSE) {
                xEventGroupClearBits(global_event_group, RING_PAUSE);
                is_pause = !is_pause;
            }
            if (bit & RING_STOP) {
                xEventGroupClearBits(global_event_group, RING_STOP);
                mp3->stop();
                suspend_playMP3Handler();
                is_pause = false;
            }
            if (bit & WAV_RING_1) {
                xEventGroupClearBits(global_event_group, WAV_RING_1);
                // if (!audio->isRunning()) {
                mp3->stop();
                suspend_playMP3Handler();
                //mp3->connecttoFS(FFat, "/ring_1.mp3");
                // Serial.println("play \"/ring_1.mp3\"");
                is_pause = false;
                // }
            }
        }
        if (xQueueReceive(play_music_queue, &music_path, 0)) {
            Serial.print("play ");
            Serial.println(music_path.c_str());
            mp3->stop();
            if (!strcmp("ring_1.mp3", music_path.c_str())) {
                resume_playMP3Handler();
                //my_print("xQueueReceive if\n");
                //vTaskSuspend(playWAVHandler);
                vTaskSuspend(playMP3Handler);
                //vTaskSuspend(playFLACHandler);
                //vTaskSuspend(playACCHandler);
                //file->open(mp3_ring_1, mp3_ring_1_len);
                file->open(mp3_array, sizeof(mp3_array) / sizeof(mp3_array[0]));
                //file->open(AUDIO_DATA, sizeof(AUDIO_DATA));
                mp3->begin(id3, out);
                vTaskResume(playMP3Handler);
            } else {
                resume_playMP3Handler();
                //my_print("xQueueReceive else\n");
                //vTaskSuspend(playWAVHandler);
                vTaskSuspend(playMP3Handler);
                // vTaskSuspend(playFLACHandler);
                //vTaskSuspend(playACCHandler);
                file->open(mp3_ring_setup, sizeof(mp3_ring_setup) / sizeof(mp3_ring_setup[0]));
                mp3->begin(id3, out);
                vTaskResume(playMP3Handler);
            }

            //audio->connecttoFS(SD_MMC, music_path.c_str());
            is_pause = false;
        }
        if (xQueueReceive(play_time_queue, &time_pos, 0)) {
            //audio->setAudioPlayPosition(time_pos);
        }
        /*if (audio->isRunning() && Millis - millis() > 100) {
            music_time = audio->getAudioCurrentTime();
            lv_msg_send(MSG_MUSIC_TIME_ID, &music_time);

            //end_time = mp3->getTotalPlayingTime();
            lv_msg_send(MSG_MUSIC_TIME_END_ID, &end_time);
            Millis = millis();
        }*/
        /*if (!is_pause)
        {
            //my_print("!is_pause\n");
            mp3->loop();
        }*/
        delay(1);
    }
}

lv_obj_t *mainGUI(void)
{
    ui_init();

    return NULL;
}

void PMUHandler()
{
    if (pmuIrq) {
        pmuIrq = false;
        watch.readPMU();
        if (watch.isVbusInsertIrq()) {
            Serial.println("isVbusInsert");
            watch.incrementalBrightness(brightnessLevel);
            usbPlugIn = true;
        }
        if (watch.isVbusRemoveIrq()) {
            Serial.println("isVbusRemove");
            destoryChargeUI();
            watch.incrementalBrightness(brightnessLevel);
            usbPlugIn = false;
        }
        if (watch.isBatChagerDoneIrq()) {
            Serial.println("isBatChagerDone");
        }
        if (watch.isBatChagerStartIrq()) {
            Serial.println("isBatChagerStart");
        }
        // Clear watch Interrupt Status Register
        watch.clearPMU();
    }
}

void SensorHandler()
{
    if (sportsIrq) {
        sportsIrq = false;
        // The interrupt status must be read after an interrupt is detected
        uint16_t status = watch.readBMA();
        Serial.printf("Accelerometer interrupt mask : 0x%x\n", status);

        if (watch.isPedometer()) {
            stepCounter = watch.getPedometerCounter();
            Serial.printf("Step count interrupt,step Counter:%u\n", stepCounter);

            if (step_counter_label != NULL) {
                lv_label_set_text_fmt(step_counter_label, "%u", stepCounter);
            }
        }
        if (watch.isActivity()) {
            Serial.println("Activity interrupt");
        }
        if (watch.isTilt()) {
            Serial.println("Tilt interrupt");
        }
        if (watch.isDoubleTap()) {
            Serial.println("DoubleTap interrupt");
        }
        if (watch.isAnyNoMotion()) {
            Serial.println("Any motion / no motion interrupt");
        }
    }
}

void setSportsFlag()
{
    sportsIrq = true;
}

void settingSensor()
{
    //Default 4G ,200HZ
    watch.configAccelerometer();

    watch.enableAccelerometer();

    watch.enablePedometer();

    watch.configInterrupt();

    watch.enableFeature(SensorBMA423::FEATURE_STEP_CNTR |
                        SensorBMA423::FEATURE_ANY_MOTION |
                        SensorBMA423::FEATURE_NO_MOTION |
                        SensorBMA423::FEATURE_ACTIVITY |
                        SensorBMA423::FEATURE_TILT |
                        SensorBMA423::FEATURE_WAKEUP,
                        true);

    watch.enablePedometerIRQ();
    watch.enableTiltIRQ();
    watch.enableWakeupIRQ();
    watch.enableAnyNoMotionIRQ();
    watch.enableActivityIRQ();

    watch.attachBMA(setSportsFlag);
}

void setPMUFlag()
{
    pmuIrq = true;
}

void settingPMU()
{
    watch.clearPMU();

    watch.disableIRQ(XPOWERS_AXP2101_ALL_IRQ);
    // Enable the required interrupt function
    watch.enableIRQ(
        // XPOWERS_AXP2101_BAT_INSERT_IRQ    | XPOWERS_AXP2101_BAT_REMOVE_IRQ      |   //BATTERY
        XPOWERS_AXP2101_VBUS_INSERT_IRQ   | XPOWERS_AXP2101_VBUS_REMOVE_IRQ     |   //VBUS
        XPOWERS_AXP2101_PKEY_SHORT_IRQ    | XPOWERS_AXP2101_PKEY_LONG_IRQ       |  //POWER KEY
        XPOWERS_AXP2101_BAT_CHG_DONE_IRQ  | XPOWERS_AXP2101_BAT_CHG_START_IRQ       //CHARGE
        // XPOWERS_AXP2101_PKEY_NEGATIVE_IRQ | XPOWERS_AXP2101_PKEY_POSITIVE_IRQ   |   //POWER KEY
    );
    watch.attachPMU(setPMUFlag);
}

void setRadioFlag(void)
{
    radioTransmitFlag = true;
}


void settingRadio()
{
#ifdef USING_TWATCH_S3
    Serial.print(F("[SX1280] Initializing ... "));
    int state = radio.begin();
    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
        return;
    }

    // set carrier frequency
    if (radio.setFrequency(RADIO_DEFAULT_FREQ) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
    }
    // set bandwidth
    if (radio.setBandwidth(RADIO_DEFAULT_BW) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
    }

    // set spreading factor
    if (radio.setSpreadingFactor(RADIO_DEFAULT_SF) == RADIOLIB_ERR_INVALID_SPREADING_FACTOR) {
        Serial.println(F("Selected spreading factor is invalid for this module!"));
    }

    // set coding rate
    if (radio.setCodingRate(RADIO_DEFAULT_CR) == RADIOLIB_ERR_INVALID_CODING_RATE) {
        Serial.println(F("Selected coding rate is invalid for this module!"));
    }

    // set LoRa sync word to 0xAB
    if (radio.setSyncWord(0xAB) != RADIOLIB_ERR_NONE) {
        Serial.println(F("Unable to set sync word!"));
    }

    // set output power
    if (radio.setOutputPower(RADIO_DEFAULT_POWER_LEVEL) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
    }

#ifdef RADIO_DEFAULT_CUR_LIMIT
    // set over current protection limit to 140 mA (accepted range is 45 - 140 mA)
    // NOTE: set value to 0 to disable overcurrent protection
    if (radio.setCurrentLimit(RADIO_DEFAULT_CUR_LIMIT) == RADIOLIB_ERR_INVALID_CURRENT_LIMIT) {
        Serial.println(F("Selected current limit is invalid for this module!"));
    }
#endif

    // set LoRa preamble length to 15 symbols (accepted range is 0 - 65535)
    if (radio.setPreambleLength(15) == RADIOLIB_ERR_INVALID_PREAMBLE_LENGTH) {
        Serial.println(F("Selected preamble length is invalid for this module!"));
    }

    // disable CRC
    if (radio.setCRC(false) == RADIOLIB_ERR_INVALID_CRC_CONFIGURATION) {
        Serial.println(F("Selected CRC is invalid for this module!"));
    }

    // set the function that will be called
    // when new packet is received
    radio.setDio1Action(setRadioFlag);
#endif

}

void playerTask(void *params)
{
#ifdef ENABLE_PLAYER
    while (1) {
        if (player_task_cb) {
            if (!player_task_cb()) {
                vTaskSuspend(NULL);
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(5);
    }
#endif
}

static bool playMP3()
{
#ifdef ENABLE_PLAYER
    if (mp3->isRunning()) {
        if (!mp3->loop()) {
            mp3->stop();
            return false;
        }
    }
    return true;
#endif
}
/*
static bool playWAV()
{
#ifdef ENABLE_PLAYER
    if (wav->isRunning()) {
        if (!wav->loop()) {
            wav->stop();
            return false;
        }
    }
    return true;
#endif
}*/

void suspend_vadTaskHandler(void)
{
    vTaskSuspend(vadTaskHandler);
}

void resume_vadTaskHandler(void)
{
    vTaskResume(vadTaskHandler);
}

void vadTask(void *params)
{
#ifdef ENABLE_PLAYER
    vTaskSuspend(vadTaskHandler);
    while (1) {
        size_t read_len = 0;
        if (watch.readMicrophone((char *) vad_buff, vad_buffer_size, &read_len)) {
            /*           // Feed samples to the VAD process and get the result
            #if   ESP_IDF_VERSION_VAL(4,4,1) == ESP_IDF_VERSION
                       vad_state_t vad_state = vad_process(vad_inst, vad_buff);
            #elif ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4,4,1)
                       vad_state_t vad_state = vad_process(vad_inst, vad_buff, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
            #else
            #error "No support this version."
            #endif*/
            if (chart != NULL) {
                lv_chart_series_t *ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);
                lv_chart_set_ext_y_array(chart, ser1, (lv_coord_t *)vad_buff);
            }

            /*if (vad_state == VAD_SPEECH) {
                Serial.print(millis());
                Serial.println(" Noise detected!!!");
                if (vad_btn_label) {
                    //lv_label_set_text_fmt(vad_btn_label, "Noise detected %u", vad_detected_counter++);
                }
            }*/
        }
        delay(5);
    }
#endif
}

void settingPlayer()
{
#ifdef ENABLE_PLAYER
    file = new AudioFileSourcePROGMEM(AUDIO_DATA, sizeof(AUDIO_DATA));
    //file = new AudioFileSourcePROGMEM();
    id3 = new AudioFileSourceID3(file);
    out = new AudioOutputI2S(1, AudioOutputI2S::EXTERNAL_I2S);
    out->SetPinout(BOARD_DAC_IIS_BCK, BOARD_DAC_IIS_WS, BOARD_DAC_IIS_DOUT);
    out->SetGain(0.2);
    mp3 = new AudioGeneratorMP3();
    mp3->begin(id3, out);

    file_fs = new AudioFileSourceFATFS();
    wav = new AudioGeneratorWAV();
    player_task_cb = playMP3;

    xTaskCreate(playerTask, "player", 8 * 1024, NULL, 12, &playerTaskHandler);
#endif

    // Initialize esp-sr vad detected
#if ESP_IDF_VERSION_VAL(4,4,1) == ESP_IDF_VERSION
    vad_inst = vad_create(VAD_MODE_0, MIC_I2S_SAMPLE_RATE, VAD_FRAME_LENGTH_MS);
#elif  ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4,4,1)
    vad_inst = vad_create(VAD_MODE_0);
#else
#error "No support this version."
#endif
    vad_buff = (int16_t *)ps_malloc(vad_buffer_size);
    if (vad_buff == NULL) {
        while (1) {
            Serial.println("Memory allocation failed!");
            delay(1000);
        }
    }
    xTaskCreate(vadTask, "vad", 8 * 1024, NULL, 12, &vadTaskHandler);
}

void settingIRRemote()
{
#ifdef ENABLE_IR_SENDER
    irsend.begin();
#endif
}

// Create a file and add wav header to it so we can play it from PC later
static bool CreateWAV(const char *song_name, uint32_t duration, uint16_t num_channels, const uint32_t sampling_rate, uint16_t bits_per_sample)
{
    // data size in bytes - > this amount of data should be recorded from microphone later
    uint32_t data_size = sampling_rate * num_channels * bits_per_sample * duration / 8;

    File new_audio_file = FFat.open(song_name, FILE_WRITE);
    if (!new_audio_file) {
        Serial.println("Failed to create file");
        return false;
    }

    /* *************** ADD ".WAV" HEADER *************** */
    uint8_t CHUNK_ID[4] = {'R', 'I', 'F', 'F'};
    new_audio_file.write(CHUNK_ID, 4);

    uint32_t chunk_size = data_size + 36;
    uint8_t CHUNK_SIZE[4] = {
        (uint8_t)(chunk_size),
        (uint8_t)(chunk_size >> 8),
        (uint8_t)(chunk_size >> 16),
        (uint8_t)(chunk_size >> 24)
    };
    new_audio_file.write(CHUNK_SIZE, 4);

    uint8_t FORMAT[4] = {'W', 'A', 'V', 'E'};
    new_audio_file.write(FORMAT, 4);

    uint8_t SUBCHUNK_1_ID[4] = {'f', 'm', 't', ' '};
    new_audio_file.write(SUBCHUNK_1_ID, 4);

    uint8_t SUBCHUNK_1_SIZE[4] = {0x10, 0x00, 0x00, 0x00};
    new_audio_file.write(SUBCHUNK_1_SIZE, 4);

    uint8_t AUDIO_FORMAT[2] = {0x01, 0x00};
    new_audio_file.write(AUDIO_FORMAT, 2);

    uint8_t NUM_CHANNELS[2] = {
        (uint8_t)num_channels,
        (uint8_t)(num_channels >> 8)
    };
    new_audio_file.write(NUM_CHANNELS, 2);

    uint8_t SAMPLING_RATE[4] = {
        (uint8_t)(sampling_rate),
        (uint8_t)(sampling_rate >> 8),
        (uint8_t)(sampling_rate >> 16),
        (uint8_t)(sampling_rate >> 24)
    };
    new_audio_file.write(SAMPLING_RATE, 4);

    uint32_t byte_rate = num_channels * sampling_rate * bits_per_sample / 8;
    uint8_t BYTE_RATE[4] = {
        (uint8_t)byte_rate,
        (uint8_t)(byte_rate >> 8),
        (uint8_t)(byte_rate >> 16),
        (uint8_t)(byte_rate >> 24)
    };
    new_audio_file.write(BYTE_RATE, 4);

    uint16_t block_align = num_channels * bits_per_sample / 8;
    uint8_t BLOCK_ALIGN[2] = {
        (uint8_t)block_align,
        (uint8_t)(block_align >> 8)
    };
    new_audio_file.write(BLOCK_ALIGN, 2);

    uint8_t BITS_PER_SAMPLE[2] = {
        (uint8_t)bits_per_sample,
        (uint8_t)(bits_per_sample >> 8)
    };
    new_audio_file.write(BITS_PER_SAMPLE, 2);

    uint8_t SUBCHUNK_2_ID[4] = {'d', 'a', 't', 'a'};
    new_audio_file.write(SUBCHUNK_2_ID, 4);

    uint8_t SUBCHUNK_2_SIZE[4] = {
        (uint8_t)(data_size),
        (uint8_t)(data_size >> 8),
        (uint8_t)(data_size >> 16),
        (uint8_t)(data_size >> 24)
    };
    new_audio_file.write(SUBCHUNK_2_SIZE, 4);

    new_audio_file.close();
    return true;
}

void printLocalTime()
{
    if (!getLocalTime(&timeinfo)) {
        Serial.println("No time available (yet)");
        return;
    }
    show_timeinfo.tm_year = timeinfo.tm_year;
    show_timeinfo.tm_hour = timeinfo.tm_hour;
    show_timeinfo.tm_mon = timeinfo.tm_mon;
    show_timeinfo.tm_min = timeinfo.tm_min;
    show_timeinfo.tm_wday = timeinfo.tm_wday;
    show_timeinfo.tm_mday = timeinfo.tm_mday;
    show_timeinfo.tm_sec = timeinfo.tm_sec;
    Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
    Serial.println("Got time adjustment from NTP!");
    printLocalTime();
    WiFi.disconnect();
}

void wifi_test(void)
{
    String text;

    lv_obj_t *wifi_test_obj = lv_obj_create(lv_scr_act());
    lv_obj_align(wifi_test_obj, LV_ALIGN_CENTER, 0, 0);

    lv_obj_set_size(wifi_test_obj, 240, 240);
    lv_obj_set_style_bg_color(wifi_test_obj, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_border_opa(wifi_test_obj, LV_OPA_0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(wifi_test_obj, 0, 0);
    lv_obj_set_style_radius(wifi_test_obj, 0, 0);

    lv_obj_t *log_label = lv_label_create(wifi_test_obj);
    lv_obj_align(log_label, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_width(log_label, LV_PCT(100));
    lv_label_set_long_mode(log_label, LV_LABEL_LONG_SCROLL);
    lv_label_set_recolor(log_label, true);
    lv_obj_set_style_text_color(log_label, lv_color_hex(0xffffff), 0);
    lv_label_set_text(log_label, "Scan WiFi");
    LV_DELAY(1);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    LV_DELAY(100);
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        text = "no networks found";
    } else {
        text = n;
        text += " networks found\n";
        for (int i = 0; i < n; ++i) {
            text += (i + 1);
            text += ": ";
            text += WiFi.SSID(i);
            text += " (";
            text += WiFi.RSSI(i);
            text += ")";
            text += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " \n" : "*\n";
            delay(10);
        }
    }
    lv_label_set_text(log_label, text.c_str());
    Serial.println(text);
    LV_DELAY(2000);
    text = "Connecting to ";
    Serial.print("Connecting to ");
    text += WIFI_SSID;
    text += "\n";
    Serial.print(WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    uint32_t last_tick = millis();

    bool is_smartconfig_connect = false;
    lv_label_set_long_mode(log_label, LV_LABEL_LONG_WRAP);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        text += ".";
        lv_label_set_text(log_label, text.c_str());
        LV_DELAY(100);
        if (millis() - last_tick > WIFI_CONNECT_WAIT_MAX) {
            /* Automatically start smartconfig when connection times out */
            text += "\nConnection timed out, start smartconfig";
            lv_label_set_text(log_label, text.c_str());
            LV_DELAY(100);
            is_smartconfig_connect = true;
            WiFi.mode(WIFI_AP_STA);
            Serial.println("\r\n wait for smartconfig....");
            text += "\r\n wait for smartconfig....";
            text += "\nPlease use #ff0000 EspTouch# Apps to connect to the "
                    "distribution network";
            lv_label_set_text(log_label, text.c_str());
            WiFi.beginSmartConfig();
            while (1) {
                LV_DELAY(100);
                if (WiFi.smartConfigDone()) {
                    Serial.println("\r\nSmartConfig Success\r\n");
                    Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
                    Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
                    text += "\nSmartConfig Success";
                    text += "\nSSID:";
                    text += WiFi.SSID().c_str();
                    text += "\nPSW:";
                    text += WiFi.psk().c_str();
                    lv_label_set_text(log_label, text.c_str());
                    LV_DELAY(1000);
                    last_tick = millis();
                    break;
                }
            }
        }
    }
    if (!is_smartconfig_connect) {
        text += "\nCONNECTED \nTakes ";
        Serial.print("\n CONNECTED \nTakes ");
        text += millis() - last_tick;
        Serial.print(millis() - last_tick);
        text += " ms\n";
        Serial.println(" millseconds");
        lv_label_set_text(log_label, text.c_str());
    }
    LV_DELAY(2000);
}

int printLocalTime_cont = 0;

void renew_ui_time(void)
{
    char temp[30] = { 0 };

    if (show_timeinfo_old.tm_sec != show_timeinfo.tm_sec) {
        show_timeinfo_old.tm_sec = show_timeinfo.tm_sec;
        sprintf(temp, "%02d", show_timeinfo.tm_sec);
        lv_label_set_text(second, temp);
    }

    if (show_timeinfo_old.tm_min != show_timeinfo.tm_min) {
        show_timeinfo_old.tm_min = show_timeinfo.tm_min;
        sprintf(temp, "%02d", show_timeinfo.tm_min);
        lv_label_set_text(minute, temp);
        lv_obj_align_to(second, minute, LV_ALIGN_OUT_RIGHT_BOTTOM, -12, 13);
    }

    if (show_timeinfo_old.tm_hour != show_timeinfo.tm_hour) {
        int hour_temp = show_timeinfo.tm_hour + 6 >= 24 ? show_timeinfo.tm_hour + 6 - 24 : show_timeinfo.tm_hour + 6;
        show_timeinfo_old.tm_hour = show_timeinfo.tm_hour;
        sprintf(temp, "%02d", hour_temp);
        lv_label_set_text(hour, temp);
        if (hour_temp >= 8 && hour_temp < 11) {
            lv_label_set_text(state, "a.m.");
        } else if (hour_temp >= 11 && hour_temp <= 13) {
            lv_label_set_text(state, "mid");
        } else if (hour_temp >= 14 && hour_temp <= 17) {
            lv_label_set_text(state, "p.m.");
        } else {
            lv_label_set_text(state, "night");
        }
    }

    if (show_timeinfo_old.tm_mday != show_timeinfo.tm_mday) {
        sprintf(temp, "%04d", show_timeinfo.tm_year + 1900);
        lv_label_set_text(year, temp);

        lv_label_set_text(month, cn_month[show_timeinfo.tm_mon]);

        lv_label_set_text(week, cn_week[show_timeinfo.tm_wday - 1 >= 0 ? show_timeinfo.tm_wday - 1 : 0]);

        sprintf(temp, "%02d", show_timeinfo.tm_mday);
        lv_label_set_text(day, temp);
    }
}

void destoryChargeUI()
{
    if (!charge_cont) {
        return;
    }
    lv_obj_del(charge_cont);
    charge_cont = NULL;
}

static void PDM_Record(const char *song_name, uint32_t duration)
{
    // Add wav header to the file so we can play it from PC later
    if (!CreateWAV(song_name, duration, 1, MIC_I2S_SAMPLE_RATE, MIC_I2S_BITS_PER_SAMPLE)) {
        Serial.println("Error during wav header creation");
        return;
    }

    Serial.println("Create wav file succssed!");

    // Buffer to receive data from microphone
    const size_t BUFFER_SIZE = 500;
    uint8_t *buf = (uint8_t *)malloc(BUFFER_SIZE);
    if (!buf) {
        Serial.println("Failed to alloc memory");
        return;
    }

    // Open created .wav file in append+binary mode to add PCM data
    File audio_file = FFat.open(song_name, FILE_APPEND);
    if (!audio_file) {
        Serial.println("Failed to create file");
        return;
    }

    // data size in bytes - > this amount of data should be recorded from microphone
    uint32_t data_size = MIC_I2S_SAMPLE_RATE * MIC_I2S_BITS_PER_SAMPLE * duration / 8;

    // Record until "file_size" bytes have been read from mic.
    uint32_t counter = 0;
    size_t bytes_written = 0;
    Serial.println("Recording started");
    int percentage = 0;

    while (counter != data_size) {

        percentage = ((float)counter / (float)data_size) * 100;
        Serial.print(percentage);
        Serial.println("%");

        lv_msg_send(LVGL_MESSAGE_PROGRESS_CHANGED_ID, &percentage);

        // Check for file size overflow
        if (counter > data_size) {
            Serial.println("File is corrupted. data_size must be multiple of BUFFER_SIZE. Please modify BUFFER_SIZE");
            break;
        }

        // Read data from microphone
        if (!watch.readMicrophone(buf, BUFFER_SIZE, &bytes_written)) {
            Serial.println("readMicrophone() error");
        }

        if (bytes_written != BUFFER_SIZE) {
            Serial.println("Bytes written error");
        }
        // Save data to FFat
        audio_file.write( buf, BUFFER_SIZE);
        // Increment the counter
        counter += BUFFER_SIZE;

        lv_task_handler();
    }
    percentage = 100;
    lv_msg_send(LVGL_MESSAGE_PROGRESS_CHANGED_ID, &percentage);
    Serial.println("Recording finished");

    audio_file.close();
    free(buf);
}

void lowPowerEnergyHandler()
{
    Serial.println("Enter light sleep mode!");
    brightnessLevel = watch.getBrightness();
    watch.decrementBrightness(0);

    watch.clearPMU();

    watch.configreFeatureInterrupt(
        SensorBMA423::INT_STEP_CNTR |   // Pedometer interrupt
        SensorBMA423::INT_ACTIVITY |    // Activity interruption
        SensorBMA423::INT_TILT |        // Tilt interrupt
        // SensorBMA423::INT_WAKEUP |      // DoubleTap interrupt
        SensorBMA423::INT_ANY_NO_MOTION,// Any  motion / no motion interrupt
        false);

    sportsIrq = false;
    pmuIrq = false;
    //lv_timer_pause(transmitTask);

    //TODO: Low power consumption not debugged
    if (lightSleep) {
        esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
        // esp_sleep_enable_ext1_wakeup(1ULL << BOARD_BMA423_INT1, ESP_EXT1_WAKEUP_ANY_HIGH);
        // esp_sleep_enable_ext1_wakeup(1ULL << BOARD_PMU_INT, ESP_EXT1_WAKEUP_ALL_LOW);

        gpio_wakeup_enable ((gpio_num_t)BOARD_PMU_INT, GPIO_INTR_LOW_LEVEL);
        gpio_wakeup_enable ((gpio_num_t)BOARD_BMA423_INT1, GPIO_INTR_HIGH_LEVEL);
        esp_sleep_enable_gpio_wakeup ();
        esp_light_sleep_start();
    } else {

        // setCpuFrequencyMhz(10);
        // setCpuFrequencyMhz(80);
        //my_print("=========esp_light_sleep_start=========\n");
        while (!pmuIrq && !sportsIrq && !watch.getTouched()) {
            delay(300);
            // gpio_wakeup_enable ((gpio_num_t)BOARD_TOUCH_INT, GPIO_INTR_LOW_LEVEL);
            // esp_sleep_enable_timer_wakeup(3 * 1000);
            // esp_light_sleep_start();
        }
        //my_print("=========esp_light_sleep_end=========\n");
        // setCpuFrequencyMhz(240);
    }

    // Clear Interrupts in Loop
    // watch.readBMA();
    // watch.clearPMU();

    watch.configreFeatureInterrupt(
        SensorBMA423::INT_STEP_CNTR |   // Pedometer interrupt
        SensorBMA423::INT_ACTIVITY |    // Activity interruption
        SensorBMA423::INT_TILT |        // Tilt interrupt
        // SensorBMA423::INT_WAKEUP |      // DoubleTap interrupt
        SensorBMA423::INT_ANY_NO_MOTION,// Any  motion / no motion interrupt
        true);

    lv_disp_trig_activity(NULL);

    watch.incrementalBrightness(brightnessLevel);
}

void loop()
{
    lv_task_handler();
    delay(5);
    // SensorHandler();
    // PMUHandler();

    // get_BattVoltage();

    // if (standby_en) {
    //     printLocalTime_cont++;
    //     if (printLocalTime_cont >= 20 && second != NULL) {
    //         printLocalTime_cont = 0;
    //         printLocalTime();
    //         renew_ui_time();
    //     }

    //     if (recordFlag) {
    //         recordFlag = false;
    //         PDM_Record(DEFAULT_RECORD_FILENAME, 8);
    //         canScreenOff = true;
    //         lv_disp_trig_activity(NULL);
    //     }
    // }

    // if (((lv_disp_get_inactive_time(NULL) >= 10000) && standby_en)) {
    //     lowPowerEnergyHandler();
    //     standby_en = 0;
    // } else if ((lv_disp_get_inactive_time(NULL) < 10000) && !standby_en) {
    //     standby_en = 1;
    // }
}

uint32_t lastMillis;
static const char *chg_status[] = {
    "Tri ",
    "Pre",
    "Constant current",
    "Constant voltage",
    "Charge done",
    "No charge"
};
void get_BattVoltage(void)
{
    if (batt_voltage_label != NULL) {
        if (lastMillis < millis()) {
            uint8_t charge_status = watch.getChargerStatus();
            lv_label_set_text_fmt(batt_voltage_label, "Charging:%s\nDischarge:%s\nUSB PlugIn:%s\nCHG state:%s\nBattery Voltage:%u mV\nUSB Voltage:%u mV\nSYS Voltage:%u mV\nBattery Percent:%d%%",
                                  watch.isCharging() ? "#00ff00 YES" : "#ff0000 NO",
                                  watch.isDischarge() ? "#00ff00 YES" : "#ff0000 NO",
                                  watch.isVbusIn() ? "#00ff00 YES" : "#ff0000 NO",
                                  chg_status[charge_status],
                                  watch.getBattVoltage(),
                                  watch.getVbusVoltage(),
                                  watch.getSystemVoltage(),
                                  watch.getBatteryPercent()
                                 );
            lastMillis = millis() + 1000;
        }
    }
}

void playWavTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (wav->isRunning()) {
            if (!wav->loop()) {
                wav->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void playMP3Task(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (!is_pause) {
            if (mp3->isRunning()) {
                if (!mp3->loop()) {
                    mp3->stop();
                    suspend_playMP3Handler();
                }
            } else {
                vTaskSuspend(NULL);
            }
            delay(2);
        }
    }
    vTaskDelete(NULL);
}

void playFLACTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (flac->isRunning()) {
            if (!flac->loop()) {
                flac->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

void playACCTask(void *prarms)
{
    vTaskSuspend(NULL);
    while (1) {
        if (aac->isRunning()) {
            if (!aac->loop()) {
                aac->stop();
            }
        } else {
            vTaskSuspend(NULL);
        }
        delay(2);
    }
    vTaskDelete(NULL);
}

#define BUFFER_SIZE (2*1024)
uint8_t buffer[BUFFER_SIZE] = {0};

lv_chart_series_t *ser1;

void radio_power_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    uint32_t id = lv_dropdown_get_selected(obj);
    Serial.printf("Option: %s id:%u\n", buf, id);

    bool isRunning = !transmitTask->paused;
    if (isRunning) {
        lv_timer_pause(transmitTask);
        radio.standby();
    }
    /*
    * SX1280 Power level -18 ~ 13dBm
    * SX1262 Power level   2 ~ 22dBm
    * * * * */
    if (id > sizeof(radio_power_args_list) / sizeof(radio_power_args_list[0])) {
        Serial.println("invalid dBm params!");
        return;
    }
    // set output power (accepted range is - 17 - 22 dBm)
    if (radio.setOutputPower(radio_power_args_list[id]) == RADIOLIB_ERR_INVALID_OUTPUT_POWER) {
        Serial.println(F("Selected output power is invalid for this module!"));
    }

    if (transmitFlag) {
        radio.startTransmit("");
    } else {
        radio.startReceive();
    }

    if (isRunning) {
        lv_timer_resume(transmitTask);
    }
}

void radioTask(lv_timer_t *parent)
{
    char buf[256];
    // check if the previous operation finished
    if (radioTransmitFlag) {
        // reset flag
        radioTransmitFlag = false;

        if (transmitFlag) {
            //TX
            // the previous operation was transmission, listen for response
            // print the result
            if (transmissionState == RADIOLIB_ERR_NONE) {
                // packet was successfully sent
                Serial.println(F("transmission finished!"));
            } else {
                Serial.print(F("failed, code "));
                Serial.println(transmissionState);
            }

            lv_snprintf(buf, 256, "[%u]:Tx %s", lv_tick_get() / 1000, transmissionState == RADIOLIB_ERR_NONE ? "Successed" : "Failed");
            set_text_radio_ta(buf);
            transmissionState = radio.startTransmit("Hello World!");
        } else {
            // RX
            // the previous operation was reception
            // print data and send another packet
            String str;
            int state = radio.readData(str);

            if (state == RADIOLIB_ERR_NONE) {
                // packet was successfully received
                Serial.println(F("[SX1262] Received packet!"));

                // print data of the packet
                Serial.print(F("[SX1262] Data:\t\t"));
                Serial.println(str);

                // print RSSI (Received Signal Strength Indicator)
                Serial.print(F("[SX1262] RSSI:\t\t"));
                Serial.print(radio.getRSSI());
                Serial.println(F(" dBm"));

                Serial.print(F("[SX1262] SNR:\t\t"));
                Serial.print(radio.getSNR());
                Serial.println(F(" dB"));

                lv_snprintf(buf, 256, "[%u]:Rx %s \nRSSI:%.2f", lv_tick_get() / 1000, str.c_str(), radio.getRSSI());
                set_text_radio_ta(buf);
            }

            radio.startReceive();
        }
    }
}

void radio_rxtx_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    uint32_t id = lv_dropdown_get_selected(obj);
    Serial.printf("Option: %s id:%u\n", buf, id);
    switch (id) {
    case 0:
        lv_timer_resume(transmitTask);
        // TX
        // send the first packet on this node
        Serial.print(F("[Radio] Sending first packet ... "));
        transmissionState = radio.startTransmit("Hello World!");
        transmitFlag = true;

        break;
    case 1:
        lv_timer_resume(transmitTask);
        // RX
        Serial.print(F("[Radio] Starting to listen ... "));
        if (radio.startReceive() == RADIOLIB_ERR_NONE) {
            Serial.println(F("success!"));
        } else {
            Serial.println(F("failed "));
        }
        transmitFlag = false;
        set_text_radio_ta("[RX]:Listening.");

        break;
    case 2:
        if (!transmitTask->paused) {
            set_text_radio_ta("Radio has disable.");
            lv_timer_pause(transmitTask);
            radio.standby();
        }
        break;
    default:
        break;
    }
}

void radio_bandwidth_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    uint32_t id = lv_dropdown_get_selected(obj);
    Serial.printf("Option: %s id:%u\n", buf, id);

    // set carrier bandwidth
    if (id > sizeof(radio_bandwidth_args_list) / sizeof(radio_bandwidth_args_list[0])) {
        Serial.println("invalid bandwidth params!");
        return;
    }

    bool isRunning = !transmitTask->paused;
    if (isRunning) {
        lv_timer_pause(transmitTask);
        radio.standby();
    }

    // set bandwidth
    if (radio.setBandwidth(radio_bandwidth_args_list[id]) == RADIOLIB_ERR_INVALID_BANDWIDTH) {
        Serial.println(F("Selected bandwidth is invalid for this module!"));
    }

    if (transmitFlag) {
        radio.startTransmit("");
    } else {
        radio.startReceive();
    }

    if (isRunning) {
        lv_timer_resume(transmitTask);
    }
}

void radio_freq_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    char buf[32];
    lv_dropdown_get_selected_str(obj, buf, sizeof(buf));
    uint32_t id = lv_dropdown_get_selected(obj);
    Serial.printf("Option: %s id:%u\n", buf, id);

    // set carrier frequency
    if (id > sizeof(radio_freq_args_list) / sizeof(radio_freq_args_list[0])) {
        Serial.println("invalid params!");
        return;
    }

    bool isRunning = !transmitTask->paused;
    if (isRunning) {
        lv_timer_pause(transmitTask);
    }

    if (radio.setFrequency(radio_freq_args_list[id]) == RADIOLIB_ERR_INVALID_FREQUENCY) {
        Serial.println(F("Selected frequency is invalid for this module!"));
    }

    if (transmitFlag) {
        radio.startTransmit("");
    } else {
        radio.startReceive();
    }

    if (isRunning) {
        lv_timer_resume(transmitTask);
    }
}
