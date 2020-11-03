/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _EYE_h
#define _EYE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif


#include "Animations.h"
#include "EyeConfig.h"
#include "EyeDrawer.h"
#include "EyeTransition.h"
#include "EyeTransformation.h"
#include "EyeVariation.h"
#include "EyeBlink.h"
#include "EyeVariation.h"

class Face;

class Eye
{
protected:
	Face& _face;

	void Update();
	void ChainOperators();

public:
	Eye(Face& face);


	uint32_t Foreground;
	uint32_t Background;

	uint16_t CenterX;
	uint16_t CenterY;
	bool IsMirrored;

	EyeConfig Config;
	EyeConfig* FinalConfig;

	EyeTransition Transition;
	EyeTransformation Transformation;
	EyeVariation Variation1;
	EyeVariation Variation2;
	EyeBlink BlinkTransformation;

	void ApplyPreset(const EyeConfig preset);
	void TransitionTo(const EyeConfig preset);

	void Draw(TFT_eSprite& buffer);
};


#endif

