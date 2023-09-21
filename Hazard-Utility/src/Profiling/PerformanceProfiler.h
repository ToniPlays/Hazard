#pragma once

#include "Timer.h"
#include <string>
#include <unordered_map>


class ScopedTimer
{
public:
	ScopedTimer(const std::string& name) : m_Name(name) {}
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
	struct TimerData
	{
		float Time;
		float MaxTime;
	};

	static void SetPerFrameTiming(const char* name, float time, float maxTime = 0.0f) {
		if (s_PerFrameData.find(name) == s_PerFrameData.end())
		{
			s_PerFrameData[name] = { time, maxTime };
			return;
		}
		s_PerFrameData[name].Time += time;
	}
	static void Clear() { s_PerFrameData.clear(); }
	static const std::unordered_map<const char*, TimerData>& GetPerFrameData() { return s_PerFrameData; }
private:
	inline static std::unordered_map<const char*, TimerData> s_PerFrameData;
};

class ScopePerfTimer
{
public:
	ScopePerfTimer(const char* name)
		: m_Name(name) {}
	ScopePerfTimer(const char* name, float maxTime)
		: m_Name(name), m_MaxTime(maxTime)
	{
	}

	~ScopePerfTimer()
	{
		float time = m_Timer.ElapsedMillis();
		PerformanceProfiler::SetPerFrameTiming(m_Name, time, m_MaxTime);
	}
private:
	const char* m_Name;
	float m_MaxTime = 100.0f;
	Timer m_Timer;
};

#define HZR_TIMED_SCOPE(name, maxtime) ::ScopePerfTimer timer__LINE__(name, maxtime);
#define HZR_TIMED_SCOPE(name) ::ScopePerfTimer timer__LINE__(name);
#define HZR_TIMED_FUNCTION() ::ScopePerfTimer timer__LINE__(__FUNCTION__);