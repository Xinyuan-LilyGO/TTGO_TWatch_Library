/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#ifndef _EYECONFIG_h
#define _EYECONFIG_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct EyeConfig
{
	int16_t OffsetX;
	int16_t OffsetY;

 	int16_t Height;
	int16_t Width;

	float Slope_Top;
	float Slope_Bottom;

	int16_t Radius_Top;
	int16_t Radius_Bottom;

	int16_t Inverse_Radius_Top;
	int16_t Inverse_Radius_Bottom;

	int16_t Inverse_Offset_Top;
	int16_t Inverse_Offset_Bottom;
};

#endif