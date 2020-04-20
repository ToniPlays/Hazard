#pragma once
#include "Core/Core.h"

namespace Hazard {
	class HAZARD_API Time {
	public:
		static double deltaTime;
		static double time;
		static double unscaledDeltaTime;
		static float timeScale;
	};
}