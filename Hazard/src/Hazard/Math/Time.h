#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard {
	class Time {
	public:
		static double s_DeltaTime;
		static long double s_Time;
		static double s_UnscaledDeltaTime;
		static float s_TimeScale;

		static void Update(double time) 
		{
			s_UnscaledDeltaTime = time - s_LastTime;
			s_DeltaTime = s_UnscaledDeltaTime * Time::s_TimeScale;
			s_Time = time;
			s_LastTime = time;
		}
	private:
		inline static double s_LastTime = 0;
	};
}