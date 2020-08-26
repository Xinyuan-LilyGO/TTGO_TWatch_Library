//     ePaper Driver Lib for Arduino Project
//     Copyright (C) 2019 Michael Kamprath
//
//     This file is part of ePaper Driver Lib for Arduino Project.
// 
//     ePaper Driver Lib for Arduino Project is free software: you can 
//	   redistribute it and/or modify it under the terms of the GNU General Public License
//     as published by the Free Software Foundation, either version 3 of the License, or
//     (at your option) any later version.
// 
//     ePaper Driver Lib for Arduino Project is distributed in the hope that 
// 	   it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//     GNU General Public License for more details.
// 
//     You should have received a copy of the GNU General Public License
//     along with Shift Register LED Matrix Project.  If not, see <http://www.gnu.org/licenses/>.
//
//     This project and its creators are not associated with Crystalfontz, Good display
//	   or any other manufacturer, nor is this  project officially endorsed or reviewed for 
//	   correctness by any ePaper manufacturer. 
//

//
// Some elements of this file were heavily inspired by Adafruit's of their driver for their
// SSD1306 monochrome device:
//		https://github.com/adafruit/Adafruit_SSD1306
// 

#include <Arduino.h>
#include <SPI.h>
#include "ePaperDriver.h"
#include "ePaperDeviceConfigurations.h"

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINTLN(s) Serial.println(s)
#define DEBUG_PRINT(s) Serial.print(s)
#define DEBUG_PRINTFORMAT(s,f) Serial.print(s,f)
#else
#define DEBUG_PRINTLN(s)
#define DEBUG_PRINT(s)
#define DEBUG_PRINTFORMAT(s,f)
#endif


ePaperDisplay::ePaperDisplay(
		ePaperDeviceModel model,
		int deviceReadyPin,
		int deviceResetPin,
		int deviceDataCommandPin,
		int deviceSelectPin
	) :	ePaperCanvas(
				ePaperDeviceConfigurations::deviceSizeHorizontal(model),
				ePaperDeviceConfigurations::deviceSizeVertical(model),
				ePaperDeviceConfigurations::deviceColorMode(model)
			),
		_model( model ),
		_deviceReadyPin( deviceReadyPin ),
		_deviceResetPin( deviceResetPin ),
		_deviceDataCommandPin( deviceDataCommandPin ),
		_deviceSelectPin( deviceSelectPin ),
		_configuration(ePaperDeviceConfigurations::deviceConfigurationCMD(model)),
		_configurationSize(ePaperDeviceConfigurations::deviceConfigurationCMDSize(model))
{
	pinMode(_deviceSelectPin, OUTPUT);
	pinMode(_deviceResetPin, OUTPUT);
	pinMode(_deviceDataCommandPin, OUTPUT);
	pinMode(_deviceReadyPin, INPUT);
	
	SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
	SPI.begin();
	
	DEBUG_PRINTLN(F("ePaperDisplay object constructed"));
	DEBUG_PRINT(F("_deviceReadyPin = "));
	DEBUG_PRINT(_deviceReadyPin);	
	DEBUG_PRINT(F(", _deviceResetPin = "));
	DEBUG_PRINT(_deviceResetPin);	
	DEBUG_PRINT(F(", _deviceDataCommandPin = "));
	DEBUG_PRINT(_deviceDataCommandPin);	
	DEBUG_PRINT(F(", _deviceSelectPin = "));
	DEBUG_PRINT(_deviceSelectPin);	
	DEBUG_PRINT(F("\n\n"));	
	
	// test malloc success
	DEBUG_PRINTLN(F("Testing initial malloc success ..."));

	if (this->getBuffer1()) {
		DEBUG_PRINTLN(F("SUCCESS - getBuffer1 malloc"));
	} else {
		DEBUG_PRINTLN(F("FAIL - getBuffer1 malloc"));
	}
	if (this->getBuffer2()) {
		DEBUG_PRINTLN(F("SUCCESS - getBuffer2 malloc"));
	} else {
		DEBUG_PRINTLN(F("FAIL - getBuffer2 malloc"));
	}
	_waitCallbackFunc = nullptr;
}

ePaperDisplay::~ePaperDisplay()
{
}

void ePaperDisplay::waitForReady(void) const
{
	uint8_t busyValue = ePaperDeviceConfigurations::deviceBusyValue(model());
	
	DEBUG_PRINT(F("Waiting until epaper device is complete with busy value = "));
	DEBUG_PRINT(busyValue);
	DEBUG_PRINT(F(" : ."));
	
	sendCommand(0x71);
	while (busyValue == digitalRead(_deviceReadyPin)) {
		yield();
		if (_waitCallbackFunc) _waitCallbackFunc();
		DEBUG_PRINT(".");
		sendCommand(0x71);
	}
	DEBUG_PRINT(F("  Done!\n"));
}

void ePaperDisplay::resetDriver(void) const
{
	DEBUG_PRINTLN(F("reset driver"));
	digitalWrite(_deviceResetPin, LOW);
	delay(200);
	digitalWrite(_deviceResetPin, HIGH);
	delay(200);
}

void ePaperDisplay::sendCommand( uint8_t cmd ) const
{
	if (cmd != 0x71) {
		DEBUG_PRINT(F("Sending command to device: 0x"));
		DEBUG_PRINTFORMAT(cmd, HEX);
		DEBUG_PRINT(F("\n"));
	}
	digitalWrite(_deviceDataCommandPin, LOW);
	digitalWrite(_deviceSelectPin, LOW);
	SPI.transfer(cmd);
	digitalWrite(_deviceSelectPin, HIGH);
}

void ePaperDisplay::sendData( const uint8_t *dataArray, uint16_t arraySize, bool isProgMem, bool invertBits ) const
{
	DEBUG_PRINTLN(F("Sending data to device..."));
	digitalWrite(_deviceDataCommandPin, HIGH);
	for (uint16_t i = 0; i < arraySize; i++ ) {
		digitalWrite(_deviceSelectPin, LOW);
		uint8_t data;
		if (isProgMem) {
			data = pgm_read_byte(&dataArray[i]);
		} else {
			data = dataArray[i];
			if (invertBits) {
				data = ~data;
			}
		}
		SPI.transfer(data);
		digitalWrite(_deviceSelectPin, HIGH);
		yield();
	}
	DEBUG_PRINTLN(F("    Done sending data to device."));
}

/****************************
	Handles sending a sequence of commands and data based on configuration found
	in a byte blob. The method reads the first byte which represents a directive,
	then operates on the next N bytes based on the nature of the directive. The
	Supported directives are:

			0x00 - Send Command - Sends the ned 1 byte as a command to the device
			0xFF - Wait until ready - Waites under device's ready pin goes HIGH. No
					further bytes are consumed.
			0xFE - Delay X milliseconds. The next bye is the number of millisecond (up to 255)
			0xFD - Send B&W image - Sends B&W image to device from current buffer. No
					further bytes are consumed.
			0xFC - Send color image - Sends color image to devices that support it from
					current buffer. Ignored if device does not support color image. No
					further bytes are consumed.
			0xFB -	Reserved
			0xFA -	Reserved
			0xF9 -	Reserved
			0xF8 -	Reserved
			0xF7 -	Reserved
			0xF6 -	Reserved
			0xF5 -	Reserved
			0xF4 -	Reserved
			0xF3 -	Reserved
			0xF2 -	Reserved
			0xF0 -	Reserved
			
		
			>0x01 - Send data - For directive values not defined above, interpret as a 
					"send next N bytes as data" when N is equal to directive byte value.
					Note that any directive value not defined above is always interpreted
					as an N value.

*/
void ePaperDisplay::sendCommandAndDataSequenceFromProgMem( const uint8_t *dataArray, uint16_t arraySize) const
{
	uint16_t index = 0;
	
	while (index < arraySize) {
		// read byte
		uint8_t b = pgm_read_byte(&dataArray[index]);
		if (b == 0x00) {
			// send next byte as command
			index++;
			sendCommand(pgm_read_byte(&dataArray[index]));
			index++;
		} else if (b == 0xFF ) {
			waitForReady();
			index++;
		} else if (b == 0xFE) {
			index++;
			uint8_t delay_millis = pgm_read_byte(&dataArray[index]);
			DEBUG_PRINT(F("Delaying for "));
			DEBUG_PRINT(delay_millis);
			DEBUG_PRINT(F(" milliseconds\n"));
			delay(delay_millis);
			index++;
		} else if (b == 0xFD ) {
			sendData(
				this->getBuffer1(),
				this->getBufferrSize(),
				false,
				ePaperDeviceConfigurations::deviceUsesInvertedBlackBits(this->model())
			);
			index++;
		} else if (b == 0xFC ) {
			sendData(
				this->getBuffer2(),
				this->getBufferrSize(),
				false,
				ePaperDeviceConfigurations::deviceUsesInvertedColorBits(this->model())
			);		
			index++;
		} else if (b < (uint16_t)0xF0) {
			// b is and array length. send the next b bytes as dataArray
			index++;
			sendData(&dataArray[index], b, true);
			index += b;
		}
		yield();
	}

}

void ePaperDisplay::initializeDevice(void) const
{
	DEBUG_PRINTLN(F("powering up device"));
	DEBUG_PRINTLN(F("resetting driver"));
	resetDriver();
	DEBUG_PRINT(F("sending configuration with size = "));
	DEBUG_PRINT(_configurationSize);
	DEBUG_PRINT(F("\n"));
	sendCommandAndDataSequenceFromProgMem(_configuration, _configurationSize);	
	DEBUG_PRINTLN(F("done setting up device.\n"));
}

/*!
    @brief  Pushes the current image buffer contents to the ePaper device.
    @return None (void).
    @note   Pushes the current buffer contents to the ePaper device, and then triggers
    		a display refresh. This function does not return until the display refresh 
    		has completed.
*/
void ePaperDisplay::refreshDisplay(void)
{
	initializeDevice();
	DEBUG_PRINTLN(F("Starting display refresh sequence."));
	sendCommandAndDataSequenceFromProgMem(
		ePaperDeviceConfigurations::setImageAndRefreshCMD(model()),
		ePaperDeviceConfigurations::setImageAndRefreshCMDSize(model())
	);
}


/*!
    @brief  Clear contents of display buffer (set all pixels to off).
    @return None (void).
    @note   Changes buffer contents only, no immediate effect on display.
            Follow up with a call to display(). Has the effect of setting
            the screen ePaper_WHITE.
*/
void ePaperDisplay::clearDisplay(void)
{
	fillScreen(ePaper_WHITE);
	DEBUG_PRINTLN(F("Done clearing screen"));
}
