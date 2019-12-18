
#pragma mark - Depend ArduinoJson and arduino-coinmarketcap-api libraries
/*
cd ~/Arduino/libraries
git clone https://github.com/witnessmenow/arduino-coinmarketcap-api.git
git clone -b 5.x https://github.com/bblanchon/ArduinoJson.git
*/

//! This code is ported from https://github.com/oxinon/IOTA-price-ticker-V2-TTGO-T-Display
//! This code is for demonstration only, if you like please donate to the original author


#include <TTGO.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include "CoinMarketCapApi.h"

#include "alert.h"
#include "info.h"
#include "iota.h"
#include "iota2.h"

WiFiClientSecure client;

CoinMarketCapApi api(client);

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.
unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;
TFT_eSPI *tft = nullptr;
TTGOClass *ttgo = nullptr;
//---------------------------------------//

WiFiClient espClient2;

// edit this for your network
#define wifi_ssid "WIFISSID"
#define wifi_password "WIFIPASSWD"


//---------------------------------------//

// supportet colors
#define ST7735_BLACK   0x0000
#define ST7735_GRAY    0x8410
#define ST7735_WHITE   0xFFFF
#define ST7735_RED     0xF800
#define ST7735_ORANGE  0xEC80
#define ST7735_YELLOW  0xFFE0
#define ST7735_LIME    0x07FF
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_AQUA    0x04FF
#define ST7735_BLUE    0x001F
#define ST7735_MAGENTA 0xF81F
#define ST7735_PINK    0xF8FF
#define ST7735_CHYN    0x2D05


// begin setup
void setup()
{
    Serial.begin(115200);
    Serial.println(F("Boot Ticker"));

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    ttgo->lvgl_begin();
    tft = ttgo->eTFT;

    uint16_t time = millis();
    time = millis() - time;
    tft->fillScreen(ST7735_BLACK);
    tft->setSwapBytes(true);
    tft->pushImage(0, 0, iotaWidth, iotaHeight, iota);
    delay(3000);

    // Starup

    tft->fillScreen(ST7735_BLACK);
    Serial.print("Connecting to ");
    tft->drawString("Connecting to ", 15, 10, 2);;
    Serial.println(wifi_ssid);
    tft->drawString(wifi_ssid, 15, 25, 2);
    tft->pushImage(200, 2, infoWidth, infoHeight, info);
    delay(1000);

    WiFi.begin(wifi_ssid, wifi_password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    tft->setTextColor(ST7735_GREEN);
    tft->drawString("WiFi connected", 15, 40, 2);
    tft->setTextColor(ST7735_WHITE);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
    tft->fillRect(0, 0, 240, 135, ST7735_BLACK);

    tft->pushImage(0, 0, iota2Width, iota2Height, iota2);
}



void printTickerData(String ticker)
{
    Serial.println("---------------------------------");
    Serial.println("Getting ticker data for " + ticker);


    // Ticker unfortunately is not the symbol for some reason.
    // Go to CoinMarketCap.com and select the coin you would like to check
    // The ticker name makes up the last part of the URL
    // e.g: http://coinmarketcap.com/currencies/iota/ , "iota" is the ticker value

    // Currency is optional, so you can pass only ticker if you want.
    // Check out the currency drop down on CoinMarketCap.com to get available values

    CMCTickerResponse response = api.GetTickerInfo(ticker, "usd");
    if (response.error == "") {
        Serial.print("ID: ");
        Serial.println(response.id);
        Serial.print("Name: ");
        Serial.println(response.name);
        Serial.print("Symbol: ");
        Serial.println(response.symbol);

        Serial.print("Rank: ");
        Serial.println(response.rank);

        Serial.print("Price in USD: ");
        Serial.println(response.price_usd);
        Serial.print("Price in BTC: ");
        Serial.println(response.price_btc);

        Serial.print("24h Volume USD: ");
        Serial.println(response.volume_usd_24h);
        Serial.print("Market Cap USD: ");
        Serial.println(response.market_cap_usd);

        Serial.print("Available Supply: ");
        Serial.println(response.available_supply);
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

        tft->setTextColor(ST7735_GRAY);
        tft->drawString("MIOTA USD Price", 95, 11, 2);

        tft->setTextColor(ST7735_WHITE);

        tft->fillRect(117, 37, 123, 38, ST7735_BLACK); //price
        tft->fillRect(185, 80, 55, 20, ST7735_BLACK); //rank



        tft->setTextColor(ST7735_YELLOW);

        if (response.percent_change_1h < 0) {
            tft->setTextColor(ST7735_RED);
        }
        if (response.percent_change_1h > 0) {
            tft->setTextColor(ST7735_GREEN);
        }


        tft->drawString(String(response.price_usd).c_str(), 117, 37, 6);
        tft->setTextColor(ST7735_AQUA);
        tft->drawString("Rank:", 117, 80, 4);

        tft->drawString(String(response.rank).c_str(), 190, 80, 4);

        tft->drawLine(11, 106, 229, 106, ST7735_GRAY);



// hours change
        tft->fillRect(100, 110, 140, 25, ST7735_BLACK);
        tft->setTextColor(ST7735_YELLOW);

        if (response.percent_change_1h < 0) {
            tft->setTextColor(ST7735_RED);
        }
        if (response.percent_change_1h > 0) {
            tft->setTextColor(ST7735_GREEN);
        }
        tft->drawString("% Price 1h:", 11, 110, 4);
        tft->drawString(String(response.percent_change_1h).c_str(), 156, 110, 4);
        delay(20000);


// 24 hours change
        tft->fillRect(100, 110, 140, 25, ST7735_BLACK);
        tft->setTextColor(ST7735_YELLOW);

        if (response.percent_change_24h < 0) {
            tft->setTextColor(ST7735_RED);
        }
        if (response.percent_change_24h > 0) {
            tft->setTextColor(ST7735_GREEN);
        }
        tft->drawString("% Price 24h:", 11, 110, 4);
        tft->drawString(String(response.percent_change_24h).c_str(), 156, 110, 4);
        delay(20000);


// 7d hours change
        tft->fillRect(100, 110, 140, 25, ST7735_BLACK);
        tft->setTextColor(ST7735_YELLOW);

        if (response.percent_change_7d < 0) {
            tft->setTextColor(ST7735_RED);
        }
        if (response.percent_change_7d > 0) {
            tft->setTextColor(ST7735_GREEN);
        }
        tft->drawString("% Price 7d:", 11, 110, 4);
        tft->drawString(String(response.percent_change_7d).c_str(), 156, 110, 4);
        delay(20000);


    }

    else {
        Serial.print("Error getting data: ");
        Serial.println(response.error);
        tft->fillRect(200, 2, 40, 32, ST7735_BLACK); //wifi RSSI and alert
        tft->pushImage(203, 2, alertWidth, alertHeight, alert);
    }
    Serial.println("---------------------------------");



}

float RSSI = 0.0;

void loop()
{
    unsigned long timeNow = millis();
    if ((timeNow > api_due_time))  {
        // int signal bars
        Serial.print("WiFi Signal strength: ");
        Serial.print(WiFi.RSSI());
        tft->fillRect(200, 2, 40, 32, ST7735_BLACK); //wifi RSSI and alert


        int bars;
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
            tft->fillRect(202 + (b * 6), 23 - (b * 4), 5, b * 4, ST7735_GRAY);
        }
        printTickerData("iota");
        api_due_time = timeNow + api_mtbs;
    }

}
