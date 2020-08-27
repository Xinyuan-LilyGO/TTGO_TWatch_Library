// IoT_SHT31LP_Example_1.54inchEPD : GxEPD demo example for Waveshare 1.54 e-Paper display or Good Display GDEP015OC1
//
// https://www.aliexpress.com/item/200x200-1-54inch-E-Ink-display-module-Without-PCB-Communicate-via-SPI-interface-Supports-various-controller/32811540997.html
// "200x200, 1.54inch E-Ink display module with embedded controller Communicate via SPI interface Supports various controller boards"
//
// https://www.aliexpress.com/store/product/E-paper-display-GDEW0154T8-with-demo-Board-DESTM32-S2/600281_32813958757.html
// https://www.aliexpress.com/store/product/E001-1-54-inch-partial-refresh-Small-size-dot-matrix-e-paper-display/600281_32815089163.html
//
// Created for my IoT sensor network by Jean-Marc Zingg, adapted for demo example use
//
// for low power deep sleep mode connect D0 pin to reset pin of ESP8266, e.g. through 0.1k 
//
// for supply voltage measurement connect 5V pin through 300k to A0

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Dispay ePaper for Arduino : https://forum.arduino.cc/index.php?topic=436411.0

// SPECIAL mapping for IoT from Waveshare 1.54inch e-Paper to Wemos D1 mini
// BUSY -> D6, RST -> D4, DC -> D3, CS -> D8, CLK -> D5, DIN -> D7, GND -> GND, 3.3V -> 3.3V

// mapping example for AVR, UNO, NANO etc.
// BUSY -> 7, RST -> 9, DC -> 8, C S-> 10, CLK -> 13, DIN -> 11


#define SENSOR A0
#define PERIOD_MINUTES 3

#define USE_GxGDEP015OC1
//#define USE_MicroOLED

//#define USE_NETWORK
//#define RE_INIT_NEEDED

// SDA is on D2 GPIO04
// SCL is on D1 GPIO05

#include <Wire.h>
#include "Adafruit_SHT31.h"

#ifdef USE_GxGDEP015OC1

// include library, include base class, make path known
#include <GxEPD.h>
// select the display class to use
#include <GxGDEP015OC1/GxGDEP015OC1.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI, SS, D3, D4);
// D2 reserved for SDA, use D6 (MISO) instead
GxEPD_Class display(io, D4, D6);

// FreeFonts from Adafruit_GFX
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold24pt7b.h>

#endif

Adafruit_SHT31 myHumidity;

#include <ESP8266WiFi.h>
#ifdef ESP8266
extern "C"
{
#include "user_interface.h"
}
#endif

struct parameter_type
{
  float delta_t;
  float delta_h;
  float sv_factor;
};

parameter_type parameter[] =
{
  { -0.5, +0.0, 0.00575 }, // 0 : ESP_1191B4
  { -0.5, +0.0, 0.00575 }, // 1 : ESP_118E0F
  { -0.5, +0.0, 0.00558 }, // 2 : ESP_246056 SHT21 D6RGP
  { -0.5, +3.0, 0.00558 }, // 3 : ESP_0BE239 HTU21 D5247 (+3% on 23° 63%)
  { -0.5, +0.0, 0.00558 }, // 4 : ESP_CC9456 HTU21 ok
  { -0.5, +0.0, 0.00665 }, // 5 : ESP_CAE971 SHT21 D6RGP
  { -0.0, +5.0, 0.00667 }, // 6 : ESP_D092C6 SHT21 D5M5E (rot) 300k
  { -0.5, +0.0, 0.00667 }, // 7 : ESP_D09477 SHT21 D6RGP 300k
  { -0.5, +0.0, 0.00575 }, // 8 : ESP_CC9268 SHT21 D6RGP
  { -0.0, +0.0, 0.00558 }, // 9 : ESP_CC844F HTU21 test
  { -0.0, +0.0, 0.00575 }, // 10 : ESP_CC9268 SHT31
  { -0.0, +0.0, 0.00667 }, // 11 : ESP_192589 SHT31 300k
  { -0.0, +0.0, 0.00558 }
};

int pidx = 11;

const char* ssid = "4D2DE33";
const char* password = "F4DFD1D1";
const int port = 80;
IPAddress IoTMasterIP = {192, 168, 4, 1};

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(port);

WiFiClient client;

#ifdef USE_MicroOLED

// include this after ESP8266WiFi.h to avoid conflicting swap
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

// MicroOLED Definition
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
// MicroOLED Object Declaration
MicroOLED oled(PIN_RESET, DC_JUMPER);    // I2C declaration

#endif

byte ReadDHT(byte dht_dat[]);
void sendUpdateDHT22();

float SupplyVoltage = 5.0;

//#define Serial if(0) Serial

void setup()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.println("setup");

  SupplyVoltage = system_adc_read() * parameter[pidx].sv_factor;
  Serial.print("SupplyVoltage=");
  Serial.println(SupplyVoltage);

  if (SupplyVoltage < 3.4) // Li 18650
  {
    Serial.println("SupplyVoltage low - going to sleep deep for 10 min");
    system_deep_sleep(600000000);
    return;
  }

  myHumidity.begin(SHT31_DEFAULT_ADDR); // 0x44

#ifdef USE_NETWORK

  RandomInfo();

#ifdef RE_INIT_NEEDED
  WiFi.persistent(true);
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.disconnect();
#endif

  if (!WiFi.getAutoConnect() || ( WiFi.getMode() != WIFI_STA) || (WiFi.SSID() != ssid))
  {
    Serial.println();
    Serial.print("wifi_station_dhcpc_status()=");
    Serial.println(wifi_station_dhcpc_status());
    Serial.print("WiFi.getAutoConnect()=");
    Serial.println(WiFi.getAutoConnect());
    Serial.print("WiFi.SSID()=");
    Serial.println(WiFi.SSID());
    WiFi.mode(WIFI_STA); // switch off AP
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
  }
  int ConnectTimeout = 20; // 5 seconds
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
    if (--ConnectTimeout <= 0)
    {
      Serial.println("WiFi connect timeout - going to sleep deep for 10 min");
      system_deep_sleep(600000000);
      return;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  WiFi.setOutputPower(20.5);

#endif // USE_NETWORK

#ifdef USE_MicroOLED
  oled.begin();    // Initialize the OLED
  oled.clear(ALL); // Clear the display's internal memory
  oled.clear(PAGE); // Clear the buffer.
#endif
#ifdef USE_GxGDEP015OC1
  display.init();
#endif
}

void loop()
{
  sendUpdateDHT22();
  Serial.println("system_deep_sleep");
  system_deep_sleep(60000000 * PERIOD_MINUTES);
  delay(120000);
}

double SaturationVaporPressure(double Temperature)
{
  return 6.1078 * pow(10.0, ((7.5 * Temperature) / (237.3 + Temperature)));
}

double VaporPressure(double Humidity, double Temperature)
{
  return Humidity / 100 * SaturationVaporPressure(Temperature);
}

double Humidity(double Temperature, double DewpointTemperature)
{
  return 100 * SaturationVaporPressure(DewpointTemperature) / SaturationVaporPressure(Temperature);
}

double DewpointTemperature(double Humidity, double Temperature)
{
  double v = log10(VaporPressure(Humidity, Temperature) / 6.1078);
  return 237.3 * v / (7.5 - v);
}

double AbsoluteHumidity(double Humidity, double Temperature)
{
  return 10e4 * 18.016 / 8314.3 * VaporPressure(Humidity, Temperature) / (Temperature + 273.15);
}

double AbsoluteHumidityFromDewpoint(double DewpointTemperature, double Temperature)
{
  return 10e4 * 18.016 / 8314.3 * SaturationVaporPressure(DewpointTemperature) / (Temperature + 273.15);
}

void sendUpdateDHT22()
{
  float humidity = myHumidity.readHumidity() + parameter[pidx].delta_h;
  float temperature = myHumidity.readTemperature() + parameter[pidx].delta_t;
  float dewpoint = DewpointTemperature(humidity, temperature);
  //float water = AbsoluteHumidity(humidity, temperature);
  float waterFDP = AbsoluteHumidityFromDewpoint(dewpoint, temperature);
  char dC[3] = {'°', 'C', 0};
  Serial.print("Current humdity = ");
  Serial.print(humidity);
  Serial.print("% ");
  Serial.print("temperature = ");
  Serial.print(temperature);
  Serial.print(dC);
  //    Serial.print(" water=");
  //    Serial.print(water);
  //    Serial.print("g/m3");
  Serial.print(" water=");
  Serial.print(waterFDP);
  Serial.print("g/m3");
  Serial.print(" dewpoint = ");
  Serial.print(dewpoint);
  Serial.println(dC);
#ifdef USE_MicroOLED
  int middleY = oled.getLCDHeight() / 2;
  oled.clear(PAGE);
  oled.setCursor(0, 0);
  oled.setFontType(2);
  oled.print(temperature, 1);
  oled.setFontType(0); // no '°' in font 1 !
  //  oled.setCursor(oled.getLCDWidth()-oled.getFontWidth(), 0);
  oled.print(" ");
  oled.write(247); // '°' in font 0
  oled.setCursor(0, middleY);
  oled.setFontType(2);
  oled.print(humidity, 1);
  oled.setFontType(0);
  //  oled.setCursor(oled.getLCDWidth()-oled.getFontWidth(), middleY);
  oled.print(" %");
  oled.display();
#endif
#ifdef USE_GxGDEP015OC1
  uint16_t middleY = display.height() / 2;
  display.setRotation(3);
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&FreeSansBold24pt7b);
  display.setTextSize(2);
  display.setCursor(8, 72);
  display.print(temperature, 1);
  display.setFont(&FreeSansBold18pt7b);
  display.setTextSize(2);
  display.setCursor(16, 146);
  display.print(humidity, 1);
  display.setTextSize(1);
  display.print("%");
  display.setCursor(16, 198);
  display.print(dewpoint, 1);
  display.print("  ");
  display.print(SupplyVoltage, 1);
  display.print("V");
  display.update();
#endif
#ifdef USE_NETWORK
  if (client.connect(IoTMasterIP, port))
  {
    Serial.println("IoTMaster connected successfully");
    String postDHT22 = "POST /update/DHT22/?device=" +
                       String(wifi_station_get_hostname()) +
                       "&temperature=" + String(temperature, 1) +
                       "&humidity=" + String(humidity, 1) +
                       "&water=" + String(waterFDP, 1) +
                       "&dewpoint=" + String(dewpoint, 1) +
                       "&supply=" + String(SupplyVoltage, 1) +
                       " HTTP/1.1\n Host: " + String(WiFi.localIP()) + "Connection: close\n";
    client.print(postDHT22);
  }
  else
  {
    Serial.println("IoTMaster connection failed - going to sleep deep for 10 min");
    system_deep_sleep(600000000);
  }
#endif
}

void RandomInfo()
{
  long r1 = os_random();
  long r2 = os_random();
  Serial.print("random1 : ");
  Serial.println(r1, HEX);
  Serial.print("random2 : ");
  Serial.println(r2, HEX);
}


