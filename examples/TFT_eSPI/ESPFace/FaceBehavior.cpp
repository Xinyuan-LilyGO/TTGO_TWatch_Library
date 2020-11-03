/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#include "Face.h"
#include "FaceBehavior.h"
#include "FaceEmotions.hpp"

FaceBehavior::FaceBehavior(Face& face) : _face(face), Timer(500)
{
	Timer.Start();
	Clear();
	Emotions[(int)eEmotions::Normal] = 2.0;
	Emotions[(int)eEmotions::Happy] = 1.0;
}

void FaceBehavior::SetEmotion(eEmotions emotion, float value)
{
	Emotions[emotion] = value;
}

float FaceBehavior::GetEmotion(eEmotions emotion)
{
	return Emotions[emotion];
}

void FaceBehavior::Clear()
{
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		Emotions[emotion] = 0.0;
	}
}

eEmotions FaceBehavior::GetRandomEmotion()
{
	float sum_of_weight = 0;
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		sum_of_weight += Emotions[emotion];
	}

	if (sum_of_weight == 0)
	{
		return eEmotions::Normal;
	}

	float rand = random(0, 1000 * eEmotions::EMOTIONS_COUNT) / 1000.0;

	float acc = 0;
	for (int emotion = 0; emotion < eEmotions::EMOTIONS_COUNT; emotion++)
	{
		if (Emotions[emotion] == 0) continue;
		acc += Emotions[emotion] * (eEmotions::EMOTIONS_COUNT - 1) / sum_of_weight;

		if (rand <= acc)
		{
			return (eEmotions)emotion;
		}
	}

	return eEmotions::Normal;
}


void FaceBehavior::Update()
{
	Timer.Update();

	if (Timer.IsExpired())
	{
		Timer.Reset();
		eEmotions newEmotion = GetRandomEmotion();
		if (CurrentEmotion != newEmotion)
		{
			GoToEmotion(newEmotion);
		}
	}
}


void FaceBehavior::GoToEmotion(eEmotions emotion)
{
	CurrentEmotion = emotion;

	switch (CurrentEmotion)
	{
	case eEmotions::Normal: _face.Expression.GoTo_Normal(); break;
	case eEmotions::Angry: _face.Expression.GoTo_Angry(); break;
	case eEmotions::Glee: _face.Expression.GoTo_Glee(); break;
	case eEmotions::Happy: _face.Expression.GoTo_Happy(); break;
	case eEmotions::Sad: _face.Expression.GoTo_Sad(); break;
	case eEmotions::Worried: _face.Expression.GoTo_Worried(); break;
	case eEmotions::Focused: _face.Expression.GoTo_Focused(); break;
	case eEmotions::Annoyed: _face.Expression.GoTo_Annoyed(); break;
	case eEmotions::Surprised: _face.Expression.GoTo_Surprised(); break;
	case eEmotions::Skeptic: _face.Expression.GoTo_Skeptic(); break;
	case eEmotions::Fustrated: _face.Expression.GoTo_Fustrated(); break;
	case eEmotions::Unimpressed: _face.Expression.GoTo_Unimpressed(); break;
	case eEmotions::Sleepy: _face.Expression.GoTo_Sleepy(); break;
	case eEmotions::Suspicious: _face.Expression.GoTo_Suspicious(); break;
	case eEmotions::Squint: _face.Expression.GoTo_Squint(); break;
	case eEmotions::Furious: _face.Expression.GoTo_Furious(); break;
	case eEmotions::Scared: _face.Expression.GoTo_Scared(); break;
	case eEmotions::Awe: _face.Expression.GoTo_Awe(); break;

	default: break;
	}

}
