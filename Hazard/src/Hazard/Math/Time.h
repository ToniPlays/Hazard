#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard 
{
	class Time 
	{
	public:
		inline static double s_DeltaTime = 0.0f;
		inline static double s_Time = 0.0f;
		inline static double s_UnscaledDeltaTime = 0.0f;
		inline static float s_TimeScale = 1.0f;

		static void Update(double seconds) 
		{
			s_UnscaledDeltaTime = seconds - s_LastTime;
			s_DeltaTime = s_UnscaledDeltaTime * Time::s_TimeScale;
			s_Time = seconds;
			s_LastTime = seconds;
		}

	private:
		inline static double s_LastTime = 0;

	};
}