#ifndef __ePaperSettings__
#define __ePaperSettings__
#include "ePaperSettings_Crystalfontz.h"
#include "ePaperSettings_GoodDisplay.h"


// 
// This is the command sequence for setting the image for most three-color
// displays
//
const uint8_t setImage_CMD_3color[] PROGMEM = {
	// wait until ready
//	0xFF,
	
	// set black image command
	0,	0x10,
	
	// send black data
	0xFD,
	
	// set color image command
	0,	0x13,
	
	// send color data
	0xFC,
	
	// display refresh command
	0,	0x12,
	
	// wait until ready
	0xFE, 5,
	0xFF,
	
	//VCOM AND DATA INTERVAL SETTING
	0, 0X50,
	1, 0xf7,

	// power off
	0,	0x02,
	0xFF,
	
	// Deep Sleep
	0,	0x07,
	1,	0xA5,
};

const uint16_t setImage_CMD_3color_size = 15;




#endif // __ePaperSettings__
