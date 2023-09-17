#pragma once

#include "HazardScript.h"
#include "Random.h"
#include "MathCore.h"
#include "Hazard/Math/Time.h"

namespace Hazard
{
	using namespace HazardScript;

	static float Math_RandomRange_Native(float min, float max) 
	{
		return Random::Range(min, max);
	}
	static float Math_Pow_Native(float value, float pow)
	{
		return Math::Pow(value, pow);
	}
	static float Math_Sqrt_Native(float value)
	{
		return Math::Sqrt(value);
	}
	static float Math_Radians_Native(float degrees)
	{
		return glm::radians(degrees);
	}
	//Time internal calls
	static float Time_GetDelta_Native()
	{
		return Time::s_DeltaTime;
	}
	static float Time_GetUnscaledDelta_Native()
	{
		return Time::s_UnscaledDeltaTime;
	}
	static float Time_GetSinceStart_Native()
	{
		return Time::s_Time;
	}
	static float Time_GetDeltaScale_Native()
	{
		return Time::s_TimeScale;
	}
	static void Time_SetDeltaScale_Native(float value)
	{
		Time::s_TimeScale = value;
	}
}