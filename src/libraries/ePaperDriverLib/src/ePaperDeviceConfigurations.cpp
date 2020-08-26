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
#include <Arduino.h>
#include "ePaperDeviceConfigurations.h"
#include "ePaperSettings.h"

const uint8_t* ePaperDeviceConfigurations::deviceConfigurationCMD(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
			return deviceConfiguration_CFAP104212E0_0213;
			break;
		case CFAP176264A0_0270:
			return deviceConfiguration_GDEW027C44;
			break;
		case CFAP400300C0_420:
		case CFAP400300A0_420:
			return deviceConfiguration_CFAP4003002A0_0420;
			break;
		case CFAP200200A1_0154:
			return deviceConfiguration_CFAP200200A1_0154;

		case GDEW026Z39:
			return deviceConfiguration_GDEW026Z39;
			break;
		case GDEW027C44:
			return deviceConfiguration_GDEW027C44;
			break;
		case GDEW029Z10:
			return deviceConfiguration_GDEW029Z10;
			break;
		case GDEW0371Z80:
			return deviceConfiguration_GDEW0371Z80;
			break;
		case GDEW0371W7:
			return deviceConfiguration_GDEW0371W7;
			break;
		default:
			return 0;
			break;
	}
}

uint8_t ePaperDeviceConfigurations::deviceConfigurationCMDSize(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
			return pgm_read_byte(&deviceConfigurationSize_CFAP104212E0_0213);
			break;
		case CFAP176264A0_0270:
			return pgm_read_byte(&deviceConfigurationSize_GDEW027C44);
			break;
		case CFAP400300A0_420:
		case CFAP400300C0_420:
			return pgm_read_byte(&deviceConfigurationSize_CFAP4003002A0_0420);
			break;
		case CFAP200200A1_0154:
			return sizeof(deviceConfiguration_CFAP200200A1_0154);
			break;

		case GDEW026Z39:
			return pgm_read_byte(&deviceConfigurationSize_GDEW026Z39);
			break;
		case GDEW027C44:
			return pgm_read_byte(&deviceConfigurationSize_GDEW027C44);
			break;
		case GDEW029Z10:
			return pgm_read_byte(&deviceConfigurationSize_GDEW029Z10);
			break;
		case GDEW0371Z80:
			return pgm_read_byte(&deviceConfigurationSize_GDEW0371Z80);
			break;
		case GDEW0371W7:
			return pgm_read_byte(&deviceConfigurationSize_GDEW0371W7);
			break;
		default:
			return 0;
			break;
	}
}

const uint8_t* ePaperDeviceConfigurations::setImageAndRefreshCMD(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
		case CFAP176264A0_0270:
		case CFAP400300C0_420:
		case CFAP400300A0_420:
		case GDEW026Z39:
		case GDEW027C44:
		case GDEW029Z10:
		case GDEW0371Z80:
			return setImage_CMD_3color;
			break;
		case GDEW0371W7:
			return setImage_CMD_GDEW0371W7;
			break;
		case CFAP200200A1_0154:
			return setFullScreenImage_CMD_CFAP200200A1_0154;
			break;
		default:
			return 0;
			break;
	}
}

uint16_t ePaperDeviceConfigurations::setImageAndRefreshCMDSize(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
		case CFAP176264A0_0270:
		case CFAP400300C0_420:
		case CFAP400300A0_420:
		case GDEW026Z39:
		case GDEW027C44:
		case GDEW029Z10:
		case GDEW0371Z80:
			return setImage_CMD_3color_size;
			break;
		case GDEW0371W7:
			return setImage_CMD_GDEW0371W7_size;
			break;
		case CFAP200200A1_0154:
			return sizeof(setFullScreenImage_CMD_CFAP200200A1_0154);
			break;
		default:
			return 0;
			break;
	}
}


int ePaperDeviceConfigurations::deviceSizeVertical(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
			return 212;
			break;
		case GDEW027C44:
		case CFAP176264A0_0270:
			return 264;
			break;
		case CFAP400300A0_420:
		case CFAP400300C0_420:
			return 300;
			break;
		case GDEW026Z39:
			return 296;
			break;
		case GDEW029Z10:
			return 296;
			break;
		case GDEW0371Z80:
		case GDEW0371W7:
			return 416;
			break;
		case CFAP200200A1_0154:
			return 200;
			break;
		default:
			return 0;
			break;
	}
}

int ePaperDeviceConfigurations::deviceSizeHorizontal(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
			return 104;
			break;
		case GDEW027C44:
		case CFAP176264A0_0270:
			return 176;
			break;
		case CFAP400300A0_420:
		case CFAP400300C0_420:
			return 400;
			break;
		case GDEW026Z39:
			return 152;
			break;
		case GDEW029Z10:
			return 128;
			break;
		case GDEW0371Z80:
		case GDEW0371W7:
			return 240;
			break;
		case CFAP200200A1_0154:
			return 200;
			break;
		default:
			return 0;
			break;
	}
}


ePaperColorMode ePaperDeviceConfigurations::deviceColorMode(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP104212C0_0213:
		case CFAP104212E0_0213:
		case CFAP176264A0_0270:
		case CFAP400300A0_420:
		case CFAP400300C0_420:
		case GDEW026Z39:
		case GDEW027C44:
		case GDEW029Z10:
		case GDEW0371Z80:
			return CMODE_3COLOR;
			break;
		case GDEW0371W7:
			return CMODE_4GRAY;
			break;
		default:
			return CMODE_BW;
			break;
	}
}

bool ePaperDeviceConfigurations::deviceUsesInvertedBlackBits(ePaperDeviceModel model)
{
	switch (model) {
		case GDEW026Z39:
		case GDEW029Z10:
		case GDEW0371Z80:
		case GDEW0371W7:
		case CFAP200200A1_0154:
			return true;
			break;
		default:
			return false;
			break;
	}
}

bool ePaperDeviceConfigurations::deviceUsesInvertedColorBits(ePaperDeviceModel model)
{
	switch (model) {
		case GDEW026Z39:
		case GDEW029Z10:
		case GDEW0371W7:
			return true;
			break;
		default:
			return false;
			break;
	}
}
uint8_t ePaperDeviceConfigurations::deviceBusyValue(ePaperDeviceModel model)
{
	switch (model) {
		case CFAP200200A1_0154:
			// documentation says it should be LOW, but sample code and reality says is is HIGH
			return HIGH;
			break;
		default:
			return LOW;
			break;
	}
}
