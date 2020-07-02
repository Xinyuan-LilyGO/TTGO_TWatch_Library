#pragma mark - Depend TinyGSM library
/*
cd ~/Arduino/libraries
git clone https://github.com/vshymanskyy/TinyGSM.git
*/


#include "config.h"

TTGOClass *watch = nullptr;

// Your GPRS credentials (leave empty, if missing)
const char apn[]      = ""; // Your APN
const char gprsUser[] = ""; // User
const char gprsPass[] = ""; // Password
const char simPIN[]   = ""; // SIM card PIN code, if any

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to the module)
#define SerialAT  Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800               // Modem is SIM800
#define TINY_GSM_RX_BUFFER          1024    // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
// #define TINY_GSM_DEBUG SerialMon
// #define DUMP_AT_COMMANDS

#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(SerialAT, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialAT);
#endif

// Server details
const char *server = "vsh.pp.ua";
const char *resource = "/TinyGSM/logo.txt";

TinyGsmClient client(modem);
const int  port = 80;
TFT_eSPI *tft;
Button2 *btn;
int8_t modem_rslt;

// Simple visualization
bool  showResult(String str, bool(*func)(), uint32_t timeout = 10000)
{
    bool ret = false;
    uint32_t startMillis = millis();
    float sec ;
    String strSec ;
    int16_t w  = 200;
    int16_t y = tft->getCursorY();
    int16_t h = tft->fontHeight();
    if (y + h > tft->height()) {
        tft->fillScreen(TFT_BLACK);
        tft->setCursor(0, 0);
        y = 0;
    }
    tft->print(str);
    do {
        ret = func();
        if (ret == 0) {
            tft->fillRect(w - 5, y, w, h, TFT_BLACK);
        }
        sec = (millis() - startMillis) / 1000.0;
        strSec = "[" + String(sec) + "]";
        w = tft->width() - tft->textWidth(strSec);
        tft->setCursor( w - 5, y);
        tft->print(strSec);
        if ((millis() - startMillis) > timeout) {
            return false;
        }
    } while (ret == 0);


    tft->fillRect(w - 5, y, w, h, TFT_BLACK);
    tft->setCursor( w - 2, y);
    tft->setTextColor(TFT_GREEN);
    tft->print("[OK]");
    tft->setTextColor(TFT_WHITE);
    tft->println();
    return true;
}


void setup()
{
    // Set console baud rate
    Serial.begin(115200);
    delay(10);

    watch = TTGOClass::getWatch();

    watch->begin();

    // Receive as a local variable for easy writing
    tft = watch->tft;
    btn = watch->button;

    tft->setCursor(0, 0);
    tft->setTextColor(TFT_WHITE, TFT_BLACK);
    tft->setTextFont(2);

    // Turn on the backlight
    watch->openBL();

    // Turn on the modem power
    watch->powerOnModem();

    // Initialize the serial port used by the modem
    SerialAT.begin(115200, SERIAL_8N1, SIM800_MODEM_RX, SIM800_MODEM_TX);

    // Need to wait for booting, Modem takes time to start
    delay(3000);

}


void loop()
{
    bool rslt;

    rslt = showResult("Check SIM Card", []()->bool{
        modem.sendAT("+CPIN?");
        modem_rslt = modem.waitResponse() ;
        Serial.println(modem_rslt);
        if (SIM_READY != modem_rslt)
        {
            delay(1000);
        }
        return modem_rslt == SIM_READY;
    });

    if (!rslt) {
        tft->setTextColor(TFT_RED);
        if (modem_rslt == SIM_ERROR) {
            tft->println("Modem not respone,Press the button to restart");
        } else {
            tft->println("Please check if the SIM card is inserted,Press the button to restart");
        }

        watch->powerOffModem();

        btn->setPressedHandler([]() {
            esp_restart();
        });
        while (1) {
            btn->loop();
        }
    }

    Serial.println("Initializing modem...");
    rslt = showResult("Initializing modem", []()->bool{
        return modem.init();
    });

    if (!rslt) {
        watch->restartModem();
        tft->println("restart modem");
        delay(5000);
        return;
    }

    // Unlock your SIM card with a PIN if needed
    if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
        modem.simUnlock(simPIN);
    }

    rslt = showResult("Waiting for network", []()->bool{
        return modem.waitForNetwork(1000) == true;
    });
    if (!rslt) {
        tft->println("restart modem");
        delay(5000);
        return;
    }

    rslt = showResult("Connecting to APN", []()->bool{
        if (!modem.gprsConnect(apn, gprsUser, gprsPass))
        {
            Serial.println(" FAIL");
            return false;
        }
        return true;
    });
    if (!rslt) {
        tft->println("restart modem");
        delay(5000);
        return;
    }

    String str = "Connecting to " + String(server);
    showResult(str, []()->bool{
        if (!client.connect(server, port))
        {
            Serial.println(" FAIL");
            delay(500);
            return false;
        }
        return true;
    });

    // Make a HTTP GET request:
    tft->println("Performing HTTP GET request...");
    Serial.println("Performing HTTP GET request...");


    client.print(String("GET ") + resource + " HTTP/1.1\r\n");
    client.print(String("Host: ") + server + "\r\n");
    client.print("Connection: close\r\n\r\n");
    client.println();

    unsigned long timeout = millis();
    while (client.connected() && millis() - timeout < 10000L) {
        // Print available data
        while (client.available()) {
            char c = client.read();
            str += c;
            Serial.print(c);
            timeout = millis();
        }
    }
    Serial.println();

    if (millis() - timeout > 10000L) {
        tft->setTextColor(TFT_RED);
        tft->println("Performing HTTP GET request FAIL");
        tft->setTextColor(TFT_GREEN);
    } else {
        tft->setCursor(0, 0);
        tft->fillScreen(TFT_BLACK);
        tft->println(str);
        delay(5000);
        tft->setCursor(0, 0);
        tft->fillScreen(TFT_BLACK);
    }

    client.stop();
    tft->println("Server disconnected");
    Serial.println(F("Server disconnected"));

    modem.gprsDisconnect();
    tft->println("GPRS disconnected");
    Serial.println(F("GPRS disconnected"));

    tft->println("Modem power off");
    watch->powerOffModem();

    // Do nothing forevermore
    while (true) {
        delay(1000);
    }
}

