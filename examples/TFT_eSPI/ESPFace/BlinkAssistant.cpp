/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#include "BlinkAssistant.h"
#include "Face.h"

BlinkAssistant::BlinkAssistant(Face& face) : _face(face), Timer(3500)
{
	Timer.Start();
}

void BlinkAssistant::Update()
{
	Timer.Update();

	if (Timer.IsExpired())
	{
		Blink();
	}
}

void BlinkAssistant::Blink()
{
	_face.LeftEye.BlinkTransformation.Animation.Restart();
	_face.RightEye.BlinkTransformation.Animation.Restart();
	Timer.Reset();
}