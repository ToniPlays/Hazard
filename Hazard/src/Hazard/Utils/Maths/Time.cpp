#pragma once
#include <hzrpch.h>
#include "Time.h"

namespace Hazard {

	double Time::deltaTime = 0;
	long double Time::time = 0;
	double Time::unscaledDeltaTime = 0;
	float Time::timeScale = 1.0f;
}