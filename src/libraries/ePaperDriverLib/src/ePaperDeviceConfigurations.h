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

#ifndef __ePaperDeviceConfigurations__
#define __ePaperDeviceConfigurations__
#include "ePaperDeviceModels.h"

typedef enum {
	CMODE_BW,
	CMODE_3COLOR,
	CMODE_4GRAY
} ePaperColorMode;


namespace ePaperDeviceConfigurations {

	const uint8_t* deviceConfigurationCMD(ePaperDeviceModel model);
	uint8_t deviceConfigurationCMDSize(ePaperDeviceModel model);

	const uint8_t* setImageAndRefreshCMD(ePaperDeviceModel model);
	uint16_t setImageAndRefreshCMDSize(ePaperDeviceModel model);
	
	int deviceSizeVertical(ePaperDeviceModel model);
	int deviceSizeHorizontal(ePaperDeviceModel model);
	ePaperColorMode deviceColorMode(ePaperDeviceModel model);
	bool deviceUsesInvertedBlackBits(ePaperDeviceModel model);
	bool deviceUsesInvertedColorBits(ePaperDeviceModel model);

	uint8_t deviceBusyValue(ePaperDeviceModel model);
};

#endif // __ePaperDeviceConfigurations__
