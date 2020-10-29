#pragma once
#include <hzrpch.h>
#include "Logger.h"
#include <chrono>

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "GLFW/glfw3.h"

namespace Hazard {

	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

	std::vector<Log> Logger::engineLogs;
	ProfiledFn* Logger::profiledRoot = new ProfiledFn("Root", nullptr, std::chrono::time_point<std::chrono::high_resolution_clock>());
	ProfiledFn* Logger::profiled = profiledRoot;

	bool Logger::OnEnabled()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Hazard");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Hazard Editor");
		s_ClientLogger->set_level(spdlog::level::trace);

		return true;
	}

	bool Logger::OnDisabled()
	{
		s_CoreLogger.reset();
		s_ClientLogger.reset();
		return true;
	}

	void Logger::Assert(bool success, std::string text)
	{
		if (!success) 
			CoreLog(text, 3);
	}

	void Logger::CoreLog(std::string text, int level)
	{
		if (s_CoreLogger == nullptr) {
			return;
		}
		switch (level)
		{
		case 0:
			s_CoreLogger->info(text);
			break;
		case 1:
			s_CoreLogger->warn(text);
			break;
		case 2:
			s_CoreLogger->error(text);
			break;
		case 3:
			s_CoreLogger->critical(text);
			break;
		case 4:
			s_CoreLogger->trace(text);
			break;
		}
		engineLogs.push_back(Log(text, level));
	}
	void Logger::ClientLog(std::string text, int level)
	{
		if (s_ClientLogger == nullptr) return;

		switch (level)
		{
		case 0:
			s_ClientLogger->info(text);
			break;
		case 1:
			s_ClientLogger->warn(text);
			break;
		case 2:
			s_ClientLogger->error(text);
			break;
		case 3:
			s_ClientLogger->critical(text);
			break;
		case 4:
			s_ClientLogger->trace(text);
			break;
		}
	}
	void Logger::Push(const char* fn)
	{
		ProfiledFn* pFn = FindProfiled(fn);
		pFn->startTime = std::chrono::high_resolution_clock::now();
		profiled = pFn;
	}
	void Logger::Pop()
	{
		auto time = std::chrono::high_resolution_clock::now();
		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(profiled->startTime).time_since_epoch().count();
		auto endPoint = std::chrono::time_point_cast<std::chrono::microseconds>(time).time_since_epoch().count();

		profiled->timeInMillis = (float)(endPoint - start) / 1000.0f;
		profiled = profiled->parentFn;
	}
	ProfiledFn* Logger::FindProfiled(const char* fn)
	{
		for (ProfiledFn* pFn : profiled->subFunctions) {
			if (pFn->name == fn) return pFn;
		}
		ProfiledFn* newFn = new ProfiledFn(fn, profiled, std::chrono::time_point<std::chrono::high_resolution_clock>());
		profiled->subFunctions.push_back(newFn);
		return newFn;
	}
}
