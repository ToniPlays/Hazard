#pragma once
#include <hzrpch.h>
#include "Time.h"

namespace Hazard {

	double Time::s_DeltaTime = 0;
	long double Time::s_Time = 0;
	double Time::s_UnscaledDeltaTime = 0;
	float Time::s_TimeScale = 1.0f;
}