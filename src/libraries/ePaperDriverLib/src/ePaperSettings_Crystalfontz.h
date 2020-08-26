//     ePaper Driver Lib for Arduino Project
//     Copyright (C) 2019 Michael Kamprath
//
//     This file is part of Crystalfontz ePaper Display Lib for Arduino Project.
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
#ifndef __ePaperSettings_Crystalfontz__
#define __ePaperSettings_Crystalfontz__
// This file contains all the device specific configurations for the Crystalfontz brand
// ePaper devices this library supports. 

// the configuration is set up as a byte stream configuration bytes, commands, and data.
// The first byte read in a stream indicates whether the following bytes are a command or 
// data. If the byte is 0, then the next single byte is a command. If the byte is greater
// than 0 but less than 255 (0xFF), then it indicates the number of following bytes that 
// should be sent as data. If the byte is 255 (OxFF), then this library will wait until
// the ePaper device is ready, as indicated by its BUSY pin going high. This
// byte sequence can then be repeated to construct longer command and data sequences. 
// 
// Given that the data size indicated is an uint8_t and that 0xFF is reserved, any given data 
// block can only be 254 bytes in size. if more need to be sent, then just define multiple 
// consecutive  data blocks.

// 
// Model CFAP200200A1-0154 - 1.54 Inch B&W ePaper display
// 
// 
const uint8_t deviceConfiguration_CFAP200200A1_0154[] PROGMEM = 
{
	// panel setting command
	0,	0x01,	
	3,	0xC7,	0x00,	0x00,

	// soft start
	0,	0x0C,
	3,	0xD7,	0xD6,	0x9D,

	// VCOMVol
	0,	0x2c,
	1,	0x7f,
	
	//dummy line per gate
	0,	0x3A,
	1,	0x1A,
	
	// RAM data entry sequence
	0,	0x11,
	1,	0x01,
};

const uint8_t setFullScreenImage_CMD_CFAP200200A1_0154[] PROGMEM = {
			
	// waveform LUT
	0,	0x32,
	30,	0x66,    0x66,	  0x44,	  0x66,	  0xAA,	  0x11,	  0x80,	  0x08,	  0x11,	  0x18,	  0x81,	  0x18,	  0x11,	  0x88,	  0x11,	  0x88,	  0x11,	  0x88,	  0x00,	  0x00,	  0xFF,	  0xFF,	  0xFF,	  0xFF,	  0x5F,	  0xAF,	  0xFF,	  0xFF,	  0x2F,	  0x00,
	
	// RAMX Start/End
	0,	0x44,
	2,	0x00,	0x18,
	
	// RAMY Start/End
	0,	0x45,
	4,	0xC7,	0x00,	0x00,	0x00,

	// set RAMX address counter
	0,	0x4E,
	1,	0x00,
	
	// set RAMX address counter
	0,	0x4F,
	2,	0xC7,	0x00,
	
	// wait for ready
	0xFF,

	// set black image command
	0,	0x24,
	
	// send black data
	0xFD,

	// display update using LUT in RAM
	0,	0x22,
	1,	0xC7,
	
	// Image update
	0,	0x20,
	0,	0xFF,

	// wait until ready
	0xFF	
	
};	

//
// Model CFAP104212E0-0213 - 2.13 Inch 3-color ePaper display
//

const uint8_t deviceConfiguration_CFAP104212E0_0213[] PROGMEM = 
{
	// panel setting command
	0,	0x01,	
	5,	0x03,	0x00,	0x2B,	0x2B,	0x03,

	// booster soft start
	0,	0x06,
	3,	0x17,	0x17,	0x17,

	// power on command. Always "wait for ready" after this sequence
	0,	0x04,
	0xFF,		// wait for ready

	// Panel Setting
	0,	0x00,
	1,	0x0F,
		
	// PLL Control
	0,	0x30,
	1,	0x3a,
	
	//Resolution
	0,	0x61,
	3,	0x68,	0x00,	0xD4,
	
	//VCOM_DC Setting
	0,	0x82,
	1,	0x28,
	
	//Vcom and data interval setting
	0,	0x50,
	1,	0x87
};
const uint8_t deviceConfigurationSize_CFAP104212E0_0213 PROGMEM = 39;

//
// MODEL CFAP176264A0-0270 - 2.7 inch 3-color ePaper display
//

// same as GoodDisplay GDEW027C44

//
// MODEL CFAP4003002A0-0420 - 4.2 INCH 3-Color ePaper Display
//
const uint8_t deviceConfiguration_CFAP4003002A0_0420[] PROGMEM = 
{
	// panel setting command
	0,	0x01,	
	5,	0x03,	0x00,	0x0A,	0x00,	0x03,

	// booster soft start
	0,	0x06,
	3,	0x17,	0x17,	0x17,
	
	// power on command. Always "wait for ready" after this sequence
	0,	0x04,
	0xFF,		// wait for ready

	// Panel Setting
	0,	0x00,
	1,	0x0F,
		
	// PLL Control
	0,	0x30,
	1,	0x29,
	
	//Resolution
	0,	0x61,
	4,	0x01,	0x90,	0x01,	0x2C,
	
	//VCOM_DC Setting
	0,	0x82,
	1,	0x28,
	
	//Vcom and data interval setting
	0,	0x50,
	1,	0x87
};
const uint8_t deviceConfigurationSize_CFAP4003002A0_0420 PROGMEM = 40;

#endif // __ePaperSettings_Crystalfontz__
