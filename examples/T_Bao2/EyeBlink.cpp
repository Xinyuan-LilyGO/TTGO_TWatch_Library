/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#include "EyeBlink.h"


EyeBlink::EyeBlink() : Animation(40, 100, 40)
{
}

void EyeBlink::Update()
{
	auto t = Animation.GetValue();
	if(Animation.GetElapsed() > Animation.Interval) t = 0.0;
	Apply(t * t);
}


void EyeBlink::Apply(float t)
{
	Output.OffsetX = Input->OffsetX;
	Output.OffsetY = Input->OffsetY;

	Output.Width = (BlinkWidth - Input->Width) * t + Input->Width;
	Output.Height = (BlinkHeight - Input->Height) * t + Input->Height;


	Output.Slope_Top = Input->Slope_Top * (1.0 - t);
	Output.Slope_Bottom = Input->Slope_Bottom * (1.0 - t);
	Output.Radius_Top = Input->Radius_Top * (1.0 - t);
	Output.Radius_Bottom = Input->Radius_Bottom * (1.0 - t);
	Output.Inverse_Radius_Top = Input->Inverse_Radius_Top * (1.0 - t);
	Output.Inverse_Radius_Bottom = Input->Inverse_Radius_Bottom * (1.0 - t);
	Output.Inverse_Offset_Top = Input->Inverse_Offset_Top * (1.0 - t);
	Output.Inverse_Offset_Bottom = Input->Inverse_Offset_Bottom * (1.0 - t);
}

