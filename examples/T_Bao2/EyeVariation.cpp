/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#include "EyeVariation.h"


EyeVariation::EyeVariation() : Animation(0, 1000, 0, 1000, 0)
{
}

void EyeVariation::Clear()
{
	Values.OffsetX = 0;
	Values.OffsetY = 0;
	Values.Height = 0;
	Values.Width = 0;
	Values.Slope_Top = 0;
	Values.Slope_Bottom = 0;
	Values.Radius_Top = 0;
	Values.Radius_Bottom = 0;
	Values.Inverse_Radius_Top = 0;
	Values.Inverse_Radius_Bottom = 0;
	Values.Inverse_Offset_Top = 0;
	Values.Inverse_Offset_Bottom = 0;
}

void EyeVariation::Update()
{
	auto t = Animation.GetValue();
	Apply(2.0 * t - 1.0);
}

void EyeVariation::Apply(float t)
{
	Output.OffsetX = Input->OffsetX + Values.OffsetX * t;
	Output.OffsetY = Input->OffsetY + Values.OffsetY * t;
	Output.Height = Input->Height + Values.Height * t;;
	Output.Width = Input->Width + Values.Width * t;
	Output.Slope_Top = Input->Slope_Top + Values.Slope_Top * t;
	Output.Slope_Bottom = Input->Slope_Bottom + Values.Slope_Bottom * t;
	Output.Radius_Top = Input->Radius_Top + Values.Radius_Top * t;
	Output.Radius_Bottom = Input->Radius_Bottom + Values.Radius_Bottom * t;
	Output.Inverse_Radius_Top = Input->Inverse_Radius_Top + Values.Inverse_Radius_Top * t;
	Output.Inverse_Radius_Bottom = Input->Inverse_Radius_Bottom + Values.Inverse_Radius_Bottom * t;
	Output.Inverse_Offset_Top = Input->Inverse_Offset_Top + Values.Inverse_Offset_Top * t;
	Output.Inverse_Offset_Bottom = Input->Inverse_Offset_Bottom + Values.Inverse_Offset_Bottom * t;;
}