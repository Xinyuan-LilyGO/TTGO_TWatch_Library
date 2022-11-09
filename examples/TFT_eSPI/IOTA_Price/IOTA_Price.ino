
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

// The new api requires a developer key, so you must apply for a key to use
// https://pro.coinmarketcap.com/account
#define APIKEY "YOUR API KEY"

WiFiClientSecure client;
CoinMarketCapApi api(client, APIKEY);

// CoinMarketCap's limit is "no more than 10 per minute"
// Make sure to factor in if you are requesting more than one coin.
unsigned long api_mtbs = 60000; //mean time between api requests
unsigned long api_due_time = 0;
TFT_eSPI *tft = nullptr;
TTGOClass *ttgo = nullptr;
//---------------------------------------//

// edit this for your network
#define wifi_ssid "WIFISSID"
#define wifi_password "WIFIPASSWD"

//---------------------------------------//
#define TFT_AQUA  0x04FF
#define TFT_GRAY  0x8410

// begin setup
void setup()
{
    Serial.begin(115200);
    Serial.println(F("Boot Ticker"));

    ttgo = TTGOClass::getWatch();
    ttgo->begin();
    ttgo->openBL();
    tft = ttgo->tft;

    uint16_t time = millis();
    time = millis() - time;
    tft->fillScreen(TFT_BLACK);
    tft->setSwapBytes(true);
    tft->pushImage(0, 0, iotaWidth, iotaHeight, iota);
    delay(3000);

    // Starup
    tft->fillScreen(TFT_BLACK);
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

    client.setCACert(api.getRootCA());

    Serial.println("");
    Serial.println("WiFi connected");
    tft->setTextColor(TFT_GREEN);
    tft->drawString("WiFi connected", 15, 40, 2);
    tft->setTextColor(TFT_WHITE);
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(1000);
    tft->fillRect(0, 0, 240, 135, TFT_BLACK);

    tft->pushImage(0, 0, iota2Width, iota2Height, iota2);
}



void printTickerData(String ticker)
{
    Serial.println("---------------------------------");
    Serial.println("Getting ticker data for " + ticker);


    //For the new API, you can use the currency ID or abbreviated name, such as
    //Bitcoin, you can view the letter after Circulating Supply at https://coinmarketcap.com/, it is BTC

    CMCTickerResponse response = api.update(ticker, "USD");
    if (response.error == "") {
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
        delay(20000);


        // 24 hours change
        tft->fillRect(100, 110, 140, 25, TFT_BLACK);
        tft->setTextColor(TFT_YELLOW);

        if (response.percent_change_24h < 0) {
            tft->setTextColor(TFT_RED);
        }
        if (response.percent_change_24h > 0) {
            tft->setTextColor(TFT_GREEN);
        }
        tft->drawString("% Price 24h:", 11, 110, 4);
        tft->drawString(String(response.percent_change_24h).c_str(), 156, 110, 4);
        delay(20000);


        // 7d hours change
        tft->fillRect(100, 110, 140, 25, TFT_BLACK);
        tft->setTextColor(TFT_YELLOW);

        if (response.percent_change_7d < 0) {
            tft->setTextColor(TFT_RED);
        }
        if (response.percent_change_7d > 0) {
            tft->setTextColor(TFT_GREEN);
        }
        tft->drawString("% Price 7d:", 11, 110, 4);
        tft->drawString(String(response.percent_change_7d).c_str(), 156, 110, 4);
        delay(20000);


    }

    else {
        Serial.print("Error getting data: ");
        Serial.println(response.error);
        tft->fillRect(200, 2, 40, 32, TFT_BLACK); //wifi RSSI and alert
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
        tft->fillRect(200, 2, 40, 32, TFT_BLACK); //wifi RSSI and alert


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
            tft->fillRect(202 + (b * 6), 23 - (b * 4), 5, b * 4, TFT_GRAY);
        }
        printTickerData("MIOTA");
        api_due_time = timeNow + api_mtbs;
    }
}
