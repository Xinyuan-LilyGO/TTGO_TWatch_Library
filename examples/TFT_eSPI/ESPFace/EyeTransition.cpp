/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include "EyeTransition.h"

EyeTransition::EyeTransition() : Animation(500)
{
}

void EyeTransition::Update()
{
	float t = Animation.GetValue();
	Apply(t);
}

void EyeTransition::Apply(float t)
{
	Origin->OffsetX = Origin->OffsetX * (1.0 - t) + Destin.OffsetX * t;
	Origin->OffsetY = Origin->OffsetY * (1.0 - t) + Destin.OffsetY * t;
	Origin->Height = Origin->Height * (1.0 - t) + Destin.Height * t;
	Origin->Width = Origin->Width * (1.0 - t) + Destin.Width * t;
	Origin->Slope_Top = Origin->Slope_Top * (1.0 - t) + Destin.Slope_Top * t;
	Origin->Slope_Bottom = Origin->Slope_Bottom * (1.0 - t) + Destin.Slope_Bottom * t;
	Origin->Radius_Top = Origin->Radius_Top * (1.0 - t) + Destin.Radius_Top * t;
	Origin->Radius_Bottom = Origin->Radius_Bottom * (1.0 - t) + Destin.Radius_Bottom * t;
	Origin->Inverse_Radius_Top = Origin->Inverse_Radius_Top * (1.0 - t) + Destin.Inverse_Radius_Top * t;
	Origin->Inverse_Radius_Bottom = Origin->Inverse_Radius_Bottom * (1.0 - t) + Destin.Inverse_Radius_Bottom * t;
	Origin->Inverse_Offset_Top = Origin->Inverse_Offset_Top * (1.0 - t) + Destin.Inverse_Offset_Top * t;
	Origin->Inverse_Offset_Bottom = Origin->Inverse_Offset_Bottom * (1.0 - t) + Destin.Inverse_Offset_Bottom * t;

}



