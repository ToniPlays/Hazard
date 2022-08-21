#pragma once

#include "HazardScript.h"
#include "Hazard/Math/Random.h"
#include "MathCore.h"

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
}