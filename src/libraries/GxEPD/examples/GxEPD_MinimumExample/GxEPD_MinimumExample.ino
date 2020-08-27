// GxEPD_MinimumExample by Jean-Marc Zingg

#include <GxEPD.h>

// select the display class to use, only one, copy from GxEPD_Example
#include <GxGDEH0154D67/GxGDEH0154D67.h>  // 1.54" b/w

#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// constructor for AVR Arduino, copy from GxEPD_Example else
GxIO_Class io(SPI, /*CS=*/ SS, /*DC=*/ 8, /*RST=*/ 9); // arbitrary selection of 8, 9 selected for default of GxEPD_Class
GxEPD_Class display(io, /*RST=*/ 9, /*BUSY=*/ 7); // default selection of (9), 7

void setup()
{
  display.init();
  display.eraseDisplay();
  // comment out next line to have no or minimal Adafruit_GFX code
  display.drawPaged(drawHelloWorld); // version for AVR using paged drawing, works also on other processors
}

void drawHelloWorld()
{
  display.setTextColor(GxEPD_BLACK);
  display.print("Hello World!");
}

void loop() {};
