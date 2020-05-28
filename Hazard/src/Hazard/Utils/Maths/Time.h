#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard {
	class HAZARD_API Time {
	public:
		static double deltaTime;
		static long double time;
		static double unscaledDeltaTime;
		static float timeScale;
	};
}