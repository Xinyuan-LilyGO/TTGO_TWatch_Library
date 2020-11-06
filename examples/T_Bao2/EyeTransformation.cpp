/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version. 

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses 
****************************************************/

#include "EyeTransformation.h"


EyeTransformation::EyeTransformation() : Animation(200)
{
}

void EyeTransformation::Update()
{
	auto t = Animation.GetValue();
	Current.MoveX = (Destin.MoveX - Origin.MoveX) * t + Origin.MoveX;
	Current.MoveY = (Destin.MoveY - Origin.MoveY) * t + Origin.MoveY;
	Current.ScaleX = (Destin.ScaleX - Origin.ScaleX) * t + Origin.ScaleX;
	Current.ScaleY = (Destin.ScaleY - Origin.ScaleY) * t + Origin.ScaleY;

	Apply();
}

void EyeTransformation::Apply()
{
	Output.OffsetX = Input->OffsetX + Current.MoveX;
	Output.OffsetY = Input->OffsetY - Current.MoveY;
	Output.Width = Input->Width * Current.ScaleX;
	Output.Height = Input->Height * Current.ScaleY;

	Output.Slope_Top = Input->Slope_Top;
	Output.Slope_Bottom = Input->Slope_Bottom;
	Output.Radius_Top = Input->Radius_Top;
	Output.Radius_Bottom = Input->Radius_Bottom;
	Output.Inverse_Radius_Top = Input->Inverse_Radius_Top;
	Output.Inverse_Radius_Bottom = Input->Inverse_Radius_Bottom;
	Output.Inverse_Offset_Top = Input->Inverse_Offset_Top;
	Output.Inverse_Offset_Bottom = Input->Inverse_Offset_Bottom;
}

void EyeTransformation::SetDestin(Transformation transformation)
{
	Origin.MoveX =  Current.MoveX;
	Origin.MoveY =  Current.MoveY;
	Origin.ScaleX = Current.ScaleX;
	Origin.ScaleY = Current.ScaleY;

	Destin.MoveX =  transformation.MoveX;
	Destin.MoveY =  transformation.MoveY;
	Destin.ScaleX = transformation.ScaleX;
	Destin.ScaleY = transformation.ScaleY;
}


