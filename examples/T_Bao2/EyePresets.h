/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#ifndef _EYEPRESETS_h
#define _EYEPRESETS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "EyeConfig.h"


static const EyeConfig Preset_Normal = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 40,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 8,
	.Radius_Bottom = 8,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Happy = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 10,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 10,
	.Radius_Bottom = 0,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Glee = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 8,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 8,
	.Radius_Bottom = 0,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 5,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};



static const EyeConfig Preset_Sad = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 15,
	.Width = 40,
	.Slope_Top = -0.5,
	.Slope_Bottom = 0,
	.Radius_Top = 1,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Worried = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 25,
	.Width = 40,
	.Slope_Top = -0.1,
	.Slope_Bottom = 0,
	.Radius_Top = 6,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Worried_Alt = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 35,
	.Width = 40,
	.Slope_Top = -0.2,
	.Slope_Bottom = 0,
	.Radius_Top = 6,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Focused = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 14,
	.Width = 40,
	.Slope_Top = 0.2,
	.Slope_Bottom = 0,
	.Radius_Top = 3,
	.Radius_Bottom = 1,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Annoyed = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 12,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 0,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Annoyed_Alt = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 5,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 0,
	.Radius_Bottom = 4,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Surprised = {
	.OffsetX = -2,
	.OffsetY = 0,
	.Height = 45,
	.Width = 45,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 16,
	.Radius_Bottom = 16,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Skeptic = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 40,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 10,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Skeptic_Alt = {
	.OffsetX = 0,
	.OffsetY = -6,
	.Height = 26,
	.Width = 40,
	.Slope_Top = 0.3,
	.Slope_Bottom = 0,
	.Radius_Top = 1,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Fustrated = {
	.OffsetX = 3,
	.OffsetY = -5,
	.Height = 12,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 0,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Unimpressed = {
	.OffsetX = 3,
	.OffsetY = 0,
	.Height = 12,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 1,
	.Radius_Bottom = 10,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Unimpressed_Alt = {
	.OffsetX = 3,
	.OffsetY = -3,
	.Height = 22,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 1,
	.Radius_Bottom = 16,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Sleepy = {
	.OffsetX = 0,
	.OffsetY = -2,
	.Height = 14,
	.Width = 40,
	.Slope_Top = -0.5,
	.Slope_Bottom = -0.5,
	.Radius_Top = 3,
	.Radius_Bottom = 3,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Sleepy_Alt = {
	.OffsetX = 0,
	.OffsetY = -2,
	.Height = 8,
	.Width = 40,
	.Slope_Top = -0.5,
	.Slope_Bottom = -0.5,
	.Radius_Top = 3,
	.Radius_Bottom = 3,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};



static const EyeConfig Preset_Suspicious = {
	.OffsetX = 0,
	.OffsetY = 0,
	.Height = 22,
	.Width = 40,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 8,
	.Radius_Bottom = 3,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Suspicious_Alt = {
	.OffsetX = 0,
	.OffsetY = -3,
	.Height = 16,
	.Width = 40,
	.Slope_Top = 0.2,
	.Slope_Bottom = 0,
	.Radius_Top = 6,
	.Radius_Bottom = 3,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};



static const EyeConfig Preset_Squint = {
	.OffsetX = -10,
	.OffsetY = -3,
	.Height = 35,
	.Width = 35,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 8,
	.Radius_Bottom = 8,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Squint_Alt = {
	.OffsetX = 5,
	.OffsetY = 0,
	.Height = 20,
	.Width = 20,
	.Slope_Top = 0,
	.Slope_Bottom = 0,
	.Radius_Top = 5,
	.Radius_Bottom = 5,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};



static const EyeConfig Preset_Angry = {
	.OffsetX = -3,
	.OffsetY = 0,
	.Height = 20,
	.Width = 40,
	.Slope_Top = 0.3,
	.Slope_Bottom = 0,
	.Radius_Top = 2,
	.Radius_Bottom = 12,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};


static const EyeConfig Preset_Furious = {
	.OffsetX = -2,
	.OffsetY = 0,
	.Height = 30,
	.Width = 40,
	.Slope_Top = 0.4,
	.Slope_Bottom = 0,
	.Radius_Top = 2,
	.Radius_Bottom = 8,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Scared = {
	.OffsetX = -3,
	.OffsetY = 0,
	.Height = 40,
	.Width = 40,
	.Slope_Top = -0.1,
	.Slope_Bottom = 0,
	.Radius_Top = 12,
	.Radius_Bottom = 8,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};

static const EyeConfig Preset_Awe = {
	.OffsetX = 2,
	.OffsetY = 0,
	.Height = 35,
	.Width = 45,
	.Slope_Top = -0.1,
	.Slope_Bottom = 0.1,
	.Radius_Top = 12,
	.Radius_Bottom = 12,
	.Inverse_Radius_Top = 0,
	.Inverse_Radius_Bottom = 0,
	.Inverse_Offset_Top = 0,
	.Inverse_Offset_Bottom = 0
};



#endif