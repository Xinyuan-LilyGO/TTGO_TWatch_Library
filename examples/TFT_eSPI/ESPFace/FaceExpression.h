/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#ifndef _FACEEXPRESSION_h
#define _FACEEXPRESSION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class Face;

class FaceExpression
{
protected:
	Face&  _face;

public:
	FaceExpression(Face& face);

	void ClearVariations();

	void GoTo_Normal();
	void GoTo_Angry();
	void GoTo_Glee();
	void GoTo_Happy();
	void GoTo_Sad();
	void GoTo_Worried();
	void GoTo_Focused();
	void GoTo_Annoyed();
	void GoTo_Surprised();
	void GoTo_Skeptic();
	void GoTo_Fustrated();
	void GoTo_Unimpressed();
	void GoTo_Sleepy();
	void GoTo_Suspicious();
	void GoTo_Squint();
	void GoTo_Furious();
	void GoTo_Scared();
	void GoTo_Awe();
};

#endif

