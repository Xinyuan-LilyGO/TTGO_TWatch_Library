/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#ifndef _EYETRANSFORMATION_h
#define _EYETRANSFORMATION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Animations.h"
#include "EyeConfig.h"

struct Transformation
{
	float MoveX = 0.0;
	float MoveY = 0.0;
	float ScaleX = 1.0;
	float ScaleY = 1.0;
};

class EyeTransformation
{
public:
	EyeTransformation();

	EyeConfig* Input;
	EyeConfig Output;

	Transformation Origin;
	Transformation Current;
	Transformation Destin;

	RampAnimation Animation;

	void Update();
	void Apply();
	void SetDestin(Transformation transformation);
};

#endif

