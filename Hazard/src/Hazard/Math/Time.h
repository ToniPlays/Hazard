#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard {
	class  Time {
	public:
		static double s_DeltaTime;
		static long double s_Time;
		static double s_UnscaledDeltaTime;
		static float s_TimeScale;
	};
}