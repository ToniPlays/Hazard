#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Core/Timer.h"

namespace Hazard
{
	class ScopedTimer
	{
	public:
		ScopedTimer(const std::string& name): m_Name(name) {}
		~ScopedTimer()
		{

		}
	private:
		std::string m_Name;
		Timer m_Timer;
	};

	class PerformanceProfiler
	{
	public:
		void SetPerFrameTiming(const char* name, float time, float maxTime = 0.0f) {
			if (m_PerFrameData.find(name) == m_PerFrameData.end()) {
				m_PerFrameData[name] = 0.0f;
			}
			m_PerFrameData[name] += time;
		}
		void Clear() { m_PerFrameData.clear(); }
		const std::unordered_map<const char*, float>& GetPerFrameData() const { return m_PerFrameData; }
	private:
		std::unordered_map<const char*, float> m_PerFrameData;
	};


	class ScopePerfTimer
	{
	public:
		ScopePerfTimer(const char* name, PerformanceProfiler* profiler)
			: m_Name(name), m_Profiler(profiler) {}

		~ScopePerfTimer()
		{
			float time = m_Timer.ElapsedMillis();
			m_Profiler->SetPerFrameTiming(m_Name, time);
		}
	private:
		const char* m_Name;
		PerformanceProfiler* m_Profiler;
		Timer m_Timer;
		
	};
}
#define HZ_SCOPE_PERF(name)\
	ScopePerfTimer timer__LINE__(name, Application::GetData().Profiler);

#define HZ_SCOPE_TIMER(name)\
	ScopedTimer timer__LINE__(name);
