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
	static void SetPerFrameTiming(const char* name, float time, float maxTime = 0.0f) {
		if (s_PerFrameData.find(name) == s_PerFrameData.end())
		{
			s_PerFrameData[name] = time;
			return;
		}
		s_PerFrameData[name] += time;
	}
	static void Clear() { s_PerFrameData.clear(); }
	static const std::unordered_map<const char*, float>& GetPerFrameData() { return s_PerFrameData; }
private:
	inline static std::unordered_map<const char*, float> s_PerFrameData;
};

class ScopePerfTimer
{
public:
	ScopePerfTimer(const char* name)
		: m_Name(name) {}

	~ScopePerfTimer()
	{
		float time = m_Timer.ElapsedMillis();
		PerformanceProfiler::SetPerFrameTiming(m_Name, time);
	}
private:
	const char* m_Name;
	Timer m_Timer;
};

#define HZR_TIMED_SCOPE(name, maxtime) ::ScopePerfTimer timer__LINE__(name, maxtime);
#define HZR_TIMED_SCOPE(name) ::ScopePerfTimer timer__LINE__(name);
#define HZR_TIMED_FUNCTION() ::ScopePerfTimer timer__LINE__(__FUNCTION__);