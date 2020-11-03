/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/


#include "Face.h"

Face::Face(TFT_eSprite& buffer, uint16_t screenWidth, uint16_t screenHeight, uint16_t eyeSize) 
	: LeftEye(*this), RightEye(*this), Blink(*this), Look(*this), Behavior(*this), Expression(*this), _buffer(buffer)
{
	Width = screenWidth;
	Height = screenHeight;
	EyeSize = eyeSize;

	CenterX = Width / 2;
	CenterY = Height / 2;

	LeftEye.IsMirrored = true;

	LeftEye.Foreground = TFT_CYAN;
	RightEye.Foreground = TFT_CYAN;

	Behavior.Timer.Start();
}

void Face::LookFront()
{
	Look.LookAt(0.0, 0.0);
}

void Face::LookRight()
{
	Look.LookAt(-1.0, 0.0);
}

void Face::LookLeft()
{
	Look.LookAt(1.0, 0.0);
}

void Face::LookTop()
{
	Look.LookAt(0.0, 1.0);
}

void Face::LookBottom()
{
	Look.LookAt(0.0, -1.0);
}

void Face::Wait(unsigned long milliseconds)
{
	unsigned long start;
	start = millis();
	while (millis() - start < milliseconds)
	{
		Draw();
	}
}

void Face::DoBlink()
{
	Blink.Blink();
}

void Face::Update()
{
	if(RandomBehavior) Behavior.Update();
	if(RandomLook) Look.Update();
	if(RandomBlink)	Blink.Update();
	Draw();
}

void Face::Draw()
{
	_buffer.fillScreen(Background);

	LeftEye.CenterX = CenterX - EyeSize / 2 - EyeInterDistance;
	LeftEye.CenterY = CenterY;
	LeftEye.Draw(_buffer);

	RightEye.CenterX = CenterX + EyeSize / 2 + EyeInterDistance;
	RightEye.CenterY = CenterY;
	RightEye.Draw(_buffer);
  _buffer.setPivot(_buffer.width(), 0);//_buffer.height()/2);
  _buffer.pushRotated(270, -1);
	//_buffer.pushSprite(0, 0);
}
