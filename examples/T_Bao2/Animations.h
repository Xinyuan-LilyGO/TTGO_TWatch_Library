/***************************************************
Copyright (c) 2020 Luis Llamas
(www.luisllamas.es)

This program is free software: you can redistribute it and/or modify it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses
****************************************************/

#ifndef _ANIMATIONS_h
#define _ANIMATIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class IAnimation
{
public:

	virtual float GetValue() = 0;
	virtual float GetValue(unsigned long overWriteMillis) = 0;
	virtual unsigned long GetElapsed() = 0;

private:
	virtual float Calculate(unsigned long elapsedMillis) = 0;

};


class AnimationBase : IAnimation
{
public:
	AnimationBase(unsigned long interval) : Interval(interval), StarTime(millis()) {}

	unsigned long Interval;

	virtual void Restart()
	{
		StarTime = millis();
	}

	float GetValue() override final
	{
		return GetValue(GetElapsed());
	}

	float GetValue(unsigned long elapsedMillis) override final
	{
		return Calculate(elapsedMillis);
	}

	unsigned long GetElapsed() override
	{
		return static_cast<unsigned long> (millis() - StarTime);
	}

	unsigned long StarTime;

protected:
	float Calculate(unsigned long elapsedMillis) override { return 0.0; }

};

class DeltaAnimation : public AnimationBase
{
public:

	unsigned long StarTime;

	DeltaAnimation(unsigned long interval) : AnimationBase(interval) {};

	float Calculate(unsigned long elapsedMillis)
	{

		if (elapsedMillis < Interval)
		{
			return 0.0f;
		}
		else
		{
			return 1.0f;
		}

	};
};


class StepAnimation : public AnimationBase
{
public:

	unsigned long Interval;
	unsigned long StarTime;
	bool IsActive = true;

	StepAnimation(unsigned long interval) : AnimationBase(interval) {};

	float Calculate(unsigned long elapsedMillis)
	{

		if (elapsedMillis < Interval)
		{
			return 0.0f;
		}
		return 1.0f;
	};
};


class RampAnimation : public AnimationBase
{
public:

	unsigned long StarTime;
	bool IsActive = true;

	RampAnimation(unsigned long interval) : AnimationBase(interval) {};


	float Calculate(unsigned long elapsedMillis)
	{

		if (elapsedMillis < Interval)
		{
			return static_cast<float>(elapsedMillis) / Interval;
		}
		return 1.0f;
	};


};


class TriangleAnimation : public AnimationBase
{
public:

	TriangleAnimation(unsigned long interval) : AnimationBase(interval)
	{
		_t0 = interval / 2;
		_t1 = interval - _t0;
	}

	TriangleAnimation(unsigned long t0, unsigned long t1) : AnimationBase(t0 + t1)
	{
		_t0 = t0;
		_t1 = t1;
	};

	float Calculate(unsigned long elapsedMillis)
	{

		if (elapsedMillis % Interval < _t0)
		{
			return static_cast<float>(elapsedMillis % Interval) / _t0;
		}
		return 1.0f - (static_cast<float>(elapsedMillis % Interval) - _t0) / _t1;
	};

	unsigned long _t0;
	unsigned long _t1;
};


class TrapeziumAnimation : public AnimationBase
{
public:
	TrapeziumAnimation(unsigned long t) : AnimationBase(t)
	{
		_t0 = t / 3;
		_t1 = _t0;
		_t2 = t - _t0 - _t1;
	};

	TrapeziumAnimation(unsigned long t0, unsigned long t1, unsigned long t2) : AnimationBase(t0 + t1 + t2)
	{
		_t0 = t0;
		_t1 = t1;
		_t2 = t2;
	};

	float Calculate(unsigned long elapsedMillis) override
	{
		if (elapsedMillis > Interval) return 0.0;
		if (elapsedMillis < _t0)
		{
			return static_cast<float>(elapsedMillis) / _t0;
		}
		else if (elapsedMillis < _t0 + _t1)
		{
			return 1.0f;
		}
		else
		{
			return 1.0f - (static_cast<float>(elapsedMillis) - _t1 - _t0) / _t2;
		}
	};

	unsigned long _t0;
	unsigned long _t1;
	unsigned long _t2;
};


class TrapeziumPulseAnimation : public AnimationBase
{
public:
	TrapeziumPulseAnimation(unsigned long t) : AnimationBase(t)
	{
		_t0 = 0;
		_t1 = t / 3;
		_t2 = t - _t0 - _t0;
		_t3 = _t1;
		_t4 = 0;
	};

	TrapeziumPulseAnimation(unsigned long t0, unsigned long t1, unsigned long t2) : AnimationBase(t0 + t1 + t2)
	{
		_t0 = 0;
		_t1 = t0;
		_t2 = t1;
		_t3 = t2;
		_t4 = 0;
	};

	TrapeziumPulseAnimation(unsigned long t0, unsigned long t1, unsigned long t2, unsigned long t3, unsigned long t4) : AnimationBase(t0 + t1 + t2 + t3 + t4)
	{
		_t0 = t0;
		_t1 = t1;
		_t2 = t2;
		_t3 = t3;
		_t4 = t4;
	};

	float Calculate(unsigned long elapsedMillis) override
	{
		unsigned long elapsed = elapsedMillis % Interval;

		if (elapsed < _t0)
		{
			return 0.0;
		}
		if (elapsed < _t0 + _t1)
		{
			return static_cast<float>(elapsed - _t0) / _t1;
		}
		else if (elapsed < _t0 + _t1 + _t2)
		{
			return 1.0f;
		}
		else if (elapsed < _t0 + _t1 + _t2 + _t3)
		{
			return 1.0f - (static_cast<float>(elapsed) - _t2 - _t1 - _t0) / _t3;
		}

		return 0.0;
	};

	void SetInterval(uint16_t t)
	{
		_t0 = 0;
		_t1 = t / 3;
		_t2 = t - _t0 - _t0;
		_t3 = _t1;
		_t4 = 0;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}

	void SetTriangle(uint16_t t, uint16_t delay)
	{
		_t0 = 0;
		_t1 = t / 2;
		_t2 = 0;
		_t3 = _t1;
		_t4 = delay;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}

	void SetTriangleCuadrature(uint16_t t, uint16_t delay)
	{
		_t0 = delay;
		_t1 = t / 2;
		_t2 = 0;
		_t3 = _t1;
		_t4 = 0;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}
	
	void SetPulse(uint16_t t, uint16_t delay)
	{
		_t0 = 0;
		_t1 = t / 3;
		_t2 = t - _t0 - _t0;
		_t3 = _t1;
		_t4 = delay;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}

	void SetPulseCuadrature(uint16_t t, uint16_t delay)
	{
		_t0 = delay;
		_t1 = t / 3;
		_t2 = t - _t0 - _t0;
		_t3 = _t1;
		_t4 = 0;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}

	void SetInterval(uint16_t t0, uint16_t t1, uint16_t t2, uint16_t t3, uint16_t t4)
	{
		_t0 = t0;
		_t1 = t1;
		_t2 = t2;
		_t3 = t3;
		_t4 = t4;
		Interval = _t0 + _t1 + _t2 + _t3 + _t4;
	}

	unsigned long _t0;
	unsigned long _t1;
	unsigned long _t2;
	unsigned long _t3;
	unsigned long _t4;
};

#endif