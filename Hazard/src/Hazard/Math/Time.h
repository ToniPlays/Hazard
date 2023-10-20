#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard 
{
	class Time 
	{
	public:
		inline static double s_DeltaTime = 0.0f;
		inline static long double s_Time = 0.0f;
		inline static double s_UnscaledDeltaTime = 0.0f;
		inline static float s_TimeScale = 1.0f;

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