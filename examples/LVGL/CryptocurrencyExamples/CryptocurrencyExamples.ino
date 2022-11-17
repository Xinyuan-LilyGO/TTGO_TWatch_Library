#pragma mark - Depend CryptocurrencyLib libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/lewisxhe/CryptocurrencyLib.git
*/

#include "config.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "CoinMarketCapApi.h"
#include "alert.h"
#include "info.h"
#include "iota.h"
#include "iota2.h"
#include <esp_wifi.h>
#include "lv_event.h"

// The new api requires a developer key, so you must apply for a key to use
// https://pro.coinmarketcap.com/account
#define APIKEY "YOUR API KEY"

#define uS_TO_S_FACTOR              1000000ULL  /* Conversion factor for micro seconds to seconds */
#define WIFI_CONNECT_TIMEOUT        5000        //Wifi connection timeout in milliseconds

WiFiClientSecure    client;
CoinMarketCapApi    api(client, APIKEY);

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.
unsigned long       api_mtbs = 60;  //Average time between api requests, in seconds
static lv_obj_t     *wifiScanList = NULL;

TFT_eSPI            *tft = nullptr;
TTGOClass           *ttgo = nullptr;
wifi_config_t       current_conf;
EventGroupHandle_t  wifiEvent;
CMCTickerResponse   response;

//---------------------------------------//

// edit this for your network
#define wifi_ssid       "WIFI SSID"
#define wifi_password   "WIFI PASSWORD"

//---------------------------------------//
#define TFT_AQUA  0x04FF
#define TFT_GRAY  0x8410


// wifi event bit
enum {
    EVENT_WIFI_SCAN_START       =  _BV(0),
    EVENT_WIFI_SCAN_DONE        =  _BV(1),
    EVENT_WIFI_CONNECTED        =  _BV(2),
    EVENT_WIFI_CONNECTING        =  _BV(3),
    EVENT_WIFI_DISCONNECTED     =  _BV(4),
    EVENT_WIFI_CONNETC_TIMEOUT  =  _BV(5),
};


void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch (event) {
    case ARDUINO_EVENT_WIFI_READY:
        Serial.println("WiFi interface ready");
        break;
    case ARDUINO_EVENT_WIFI_SCAN_DONE:
        Serial.println("Completed scan for access points");
        xEventGroupSetBits(wifiEvent, EVENT_WIFI_SCAN_DONE);
        break;
    case ARDUINO_EVENT_WIFI_STA_START:
        Serial.println("WiFi client started");
        break;
    case ARDUINO_EVENT_WIFI_STA_STOP:
        Serial.println("WiFi clients stopped");
        xEventGroupClearBits(wifiEvent, EVENT_WIFI_CONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        Serial.println("Connected to access point");
        xEventGroupClearBits(wifiEvent, EVENT_WIFI_DISCONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        Serial.println("Disconnected from WiFi access point");
        xEventGroupClearBits(wifiEvent, EVENT_WIFI_CONNECTED);
        xEventGroupSetBits(wifiEvent, EVENT_WIFI_DISCONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE:
        Serial.println("Authentication mode of access point has changed");
        break;
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        Serial.print("Obtained IP address: ");
        Serial.println(WiFi.localIP());
        xEventGroupSetBits(wifiEvent, EVENT_WIFI_CONNECTED);
        xEventGroupClearBits(wifiEvent, EVENT_WIFI_DISCONNECTED);
        break;
    case ARDUINO_EVENT_WIFI_STA_LOST_IP:
        Serial.println("Lost IP address and IP address is reset to 0");
        break;
    default: break;
    }
}

// Load the default wifi account and password
bool loadWiFiConfig()
{
    if (esp_wifi_get_config((wifi_interface_t)ESP_IF_WIFI_STA, &current_conf) == ESP_OK) {
        if (strlen((const char *)current_conf.sta.ssid) != 0 || strlen((const char *)current_conf.sta.password) !=  0)
            return true;
    }
    Serial.println("config failed");
    return false;
}

void scanEventStart()
{
    WiFi.disconnect();
    WiFi.scanNetworks(true);
    startWindows("Scan Network...", 5000);
}

void scanEventDone()
{
    Serial.println("scanComplete...");
    int16_t len =  WiFi.scanComplete();
    for (int i = 0; i < len; ++i) {
        Serial.print("> SSID:");
        Serial.print(WiFi.SSID(i));
        Serial.print(" RSSI:");
        Serial.println(WiFi.RSSI(i));
    }

    wifiScanList = lv_menu_list_init(lv_scr_act(),
                                     "WLAN",
    [](void *) {
        Serial.println("returnEvent");
    },
    [](void *) {
        Serial.println("refreshEvent");
        scanEventStart();
    });

    for (int i = 0; i < len; ++i) {
        lv_menu_list_add(wifiScanList, LV_SYMBOL_WIFI, WiFi.SSID(i).c_str(), WiFi.RSSI(i));
    }

}

void startConnectTimeoutTimer()
{
    xTimerHandle timerHander = xTimerCreate("timer", pdTICKS_TO_MS(WIFI_CONNECT_TIMEOUT), pdFALSE, timerHander, [](xTimerHandle timer) {
        xEventGroupSetBits(wifiEvent, EVENT_WIFI_CONNETC_TIMEOUT);
        xTimerDelete(timer, portMAX_DELAY);
    });
    xTimerStart(timerHander, portMAX_DELAY);
}

void setup()
{
    wifiEvent =  xEventGroupCreate();

    Serial.begin(115200);

    Serial.println(F("Boot Ticker"));

    // Get Watch Instance
    ttgo = TTGOClass::getWatch();

    // Initialize watch
    ttgo->begin();

    // Register lvgl
    ttgo->lvgl_begin();

    // Turn on the backlight
    ttgo->openBL();

    // Get Object
    tft = ttgo->tft;

    // Register wifi events
    WiFi.mode(WIFI_STA);

    WiFi.onEvent(WiFiEvent);

    // Initialize the startup interface with lvgl
    setupGUI();

    //load default wifi config
    if (loadWiFiConfig()) {
        Serial.printf("Try to connect : SSID:%s PASSWORD:%s\n",
                      (const char *)current_conf.sta.ssid,
                      (const char *)current_conf.sta.password);

        startWindows("Connecting ...", WIFI_CONNECT_TIMEOUT);

        WiFi.begin(
            (const char *)current_conf.sta.ssid,
            (const char *)current_conf.sta.password
        );

        // Create a wifi connection timeout timer
        startConnectTimeoutTimer();
    }

    // Handle wifi events here
    while (1) {

        if (xEventGroupGetBits(wifiEvent) & EVENT_WIFI_CONNECTING) {
            xEventGroupClearBits(wifiEvent, EVENT_WIFI_CONNECTING);
            Serial.println("conneting ....");
            startWindows("conneting ...", 5000);
            startConnectTimeoutTimer();
        }

        if (xEventGroupGetBits(wifiEvent) & EVENT_WIFI_CONNETC_TIMEOUT) {
            xEventGroupClearBits(wifiEvent, EVENT_WIFI_CONNETC_TIMEOUT);
            Serial.println("connetc time out ....");
            scanEventStart();
        }

        if (xEventGroupGetBits(wifiEvent) & EVENT_WIFI_SCAN_DONE) {
            Serial.println("scan done....");
            xEventGroupClearBits(wifiEvent, EVENT_WIFI_SCAN_DONE);
            scanEventDone();
        }

        if (xEventGroupGetBits(wifiEvent) & EVENT_WIFI_DISCONNECTED) {
            xEventGroupClearBits(wifiEvent, EVENT_WIFI_DISCONNECTED);
            Serial.println("disconnetc ....");
        }

        if (xEventGroupGetBits(wifiEvent) & EVENT_WIFI_CONNECTED) {
            Serial.println("connected! ....");
            break;
        }
        lv_task_handler();
        delay(5);
    }

    // Set api ca certificate
    client.setCACert(api.getRootCA());
}


// Plot data on screen
void drawWindons()
{
    int bars;
    float RSSI = 0.0;
    Serial.println("---------------------------------");

    tft->fillScreen(TFT_BLACK);
    tft->pushImage(0, 0, iota2Width, iota2Height, iota2);

    Serial.print("WiFi Signal strength: ");
    Serial.print(WiFi.RSSI());
    tft->fillRect(200, 2, 40, 32, TFT_BLACK); //wifi RSSI and alert

    RSSI = WiFi.RSSI();

    if (RSSI >= -55) {
        bars = 5;
        Serial.println(" 5 bars");
    } else if (RSSI < -55 & RSSI >= -65) {
        bars = 4;
        Serial.println(" 4 bars");
    } else if (RSSI < -65 & RSSI >= -70) {
        bars = 3;
        Serial.println(" 3 bars");
    } else if (RSSI < -70 & RSSI >= -78) {
        bars = 2;
        Serial.println(" 2 bars");
    } else if (RSSI < -78 & RSSI >= -82) {
        bars = 1;
        Serial.println(" 1 bars");
    } else {
        bars = 0;
        Serial.println(" 0 bars");
    }

    // signal bars
    for (int b = 0; b <= bars; b++) {
        tft->fillRect(202 + (b * 6), 23 - (b * 4), 5, b * 4, TFT_GRAY);
    }

    Serial.print("ID: ");
    Serial.println(response.id);
    Serial.print("Name: ");
    Serial.println(response.name);
    Serial.print("Symbol: ");
    Serial.println(response.symbol);

    Serial.print("Rank: ");
    Serial.println(response.cmc_rank);

    Serial.print("Price: ");
    Serial.println(response.price);

    Serial.print("24h Volume: ");
    Serial.println(response.volume_24h);
    Serial.print("Market Cap: ");
    Serial.println(response.market_cap);

    Serial.print("Circulating Supply: ");
    Serial.println(response.circulating_supply);
    Serial.print("Total Supply: ");
    Serial.println(response.total_supply);

    Serial.print("Percent Change 1h: ");
    Serial.println(response.percent_change_1h);
    Serial.print("Percent Change 24h: ");
    Serial.println(response.percent_change_24h);
    Serial.print("Percent Change 7d: ");
    Serial.println(response.percent_change_7d);
    Serial.print("Last Updated: ");
    Serial.println(response.last_updated);

    tft->setTextColor(TFT_GRAY);
    tft->drawString("MIOTA USD Price", 95, 11, 2);

    tft->setTextColor(TFT_WHITE);

    tft->fillRect(117, 37, 123, 38, TFT_BLACK); //price
    tft->fillRect(185, 80, 55, 20, TFT_BLACK); //rank

    tft->setTextColor(TFT_YELLOW);

    if (response.percent_change_1h < 0) {
        tft->setTextColor(TFT_RED);
    }
    if (response.percent_change_1h > 0) {
        tft->setTextColor(TFT_GREEN);
    }


    tft->drawString(String(response.price).c_str(), 117, 37, 6);
    tft->setTextColor(TFT_AQUA);
    tft->drawString("Rank:", 117, 80, 4);

    tft->drawString(String(response.cmc_rank).c_str(), 190, 80, 4);

    tft->drawLine(11, 106, 229, 106, TFT_GRAY);

    // hours change
    tft->fillRect(100, 110, 140, 25, TFT_BLACK);
    tft->setTextColor(TFT_YELLOW);

    if (response.percent_change_1h < 0) {
        tft->setTextColor(TFT_RED);
    }
    if (response.percent_change_1h > 0) {
        tft->setTextColor(TFT_GREEN);
    }
    tft->drawString("% Price 1h:", 11, 110, 4);
    tft->drawString(String(response.percent_change_1h).c_str(), 156, 110, 4);

    // 24 hours change
    tft->setTextColor(TFT_YELLOW);

    if (response.percent_change_24h < 0) {
        tft->setTextColor(TFT_RED);
    }
    if (response.percent_change_24h > 0) {
        tft->setTextColor(TFT_GREEN);
    }
    tft->drawString("% Price 24h:", 11, 136, 4);
    tft->drawString(String(response.percent_change_24h).c_str(), 156, 136, 4);

    // 7d hours change
    tft->setTextColor(TFT_YELLOW);
    if (response.percent_change_7d < 0) {
        tft->setTextColor(TFT_RED);
    }
    if (response.percent_change_7d > 0) {
        tft->setTextColor(TFT_GREEN);
    }
    tft->drawString("% Price 7d:", 11, 162, 4);
    tft->drawString(String(response.percent_change_7d).c_str(), 156, 162, 4);

    Serial.println("---------------------------------");
}

// Shallow sleep function
void lightSleep()
{
    Serial.println("sleep!");
    delay(20);
    esp_sleep_enable_timer_wakeup(api_mtbs * uS_TO_S_FACTOR);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
    esp_light_sleep_start();
    Serial.println("Wakup!");
}



void loop()
{
    // Wait for the network connection. If the set waiting time is exceeded, the system will enter the light sleep state again
    EventBits_t bits = xEventGroupWaitBits(wifiEvent, EVENT_WIFI_CONNECTED, pdTRUE, pdTRUE, pdTICKS_TO_MS(10000));
    if ((bits & EVENT_WIFI_CONNECTED)) {
        //For the new API, you can use the currency ID or abbreviated name, such as
        //Bitcoin, you can view the letter after Circulating Supply at https://coinmarketcap.com/, it is BTC
        response = api.update("MIOTA", "USD");
        if (response.error == "") {
            // Display the acquired data
            drawWindons();
        }
    }
    // Entering shallow sleep can effectively reduce the current consumption to about 80mA
    lightSleep();
    // After a light sleep, the WiFi connection may be invalid. Here, re initialize the WiFi connection
    WiFi.begin();
}
