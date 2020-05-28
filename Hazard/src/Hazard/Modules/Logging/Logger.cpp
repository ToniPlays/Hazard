#pragma once
#include <hzrpch.h>
#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "GLFW/glfw3.h"

namespace Hazard {


	std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;
	std::unordered_map<std::string, ProfiledFunction> Logger::logs;

	bool Logger::isRealtime = false;

	Logger::Logger() : Module("Logger")
	{

	}

	void Logger::OnEnabled()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Hazard");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClientLogger = spdlog::stdout_color_mt("Application");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

	void Logger::OnDisabled()
	{
		s_CoreLogger.reset();
		s_ClientLogger.reset();
	}

	void Logger::Assert(bool success, std::string text)
	{
		if (!success) CoreLog(text, 3);
	}

	void Logger::CoreLog(std::string text, int level)
	{
		if (s_CoreLogger == nullptr) {
			return;
		}
		switch (level)
		{
		case 0:
			s_CoreLogger->trace(text);
			break;
		case 1:
			s_CoreLogger->info(text);
			break;
		case 2:
			s_CoreLogger->warn(text);
			break;
		case 3:
			s_CoreLogger->error(text);
			break;
		case 4:
			s_CoreLogger->critical(text);
			break;
		}
	}
	void Logger::ClientLog(std::string text, int level)
	{
		if (s_ClientLogger == nullptr) return;

		switch (level)
		{
		case 0:
			s_ClientLogger->trace(text);
			break;
		case 1:
			s_ClientLogger->info(text);
			break;
		case 2:
			s_ClientLogger->warn(text);
			break;
		case 3:
			s_ClientLogger->error(text);
			break;
		case 4:
			s_ClientLogger->critical(text);
			break;
		}
	}
	ProfiledFunction Logger::GetLog(std::string key) {
		if (s_CoreLogger == nullptr) return ProfiledFunction(Time::deltaTime);

		if (logs.count(key) > 0) {
			return logs[key];
		}
		else {
			return -1;
		}
	}
	void Logger::Begin(std::string name) {
		//If not exists

		if (s_CoreLogger == nullptr) return;

		if (logs.count(name) == 0) {
			ProfiledFunction fn(glfwGetTime() * 1000);
			logs[name] = fn;
		}
		else {
			double t = glfwGetTime() * 1000;
			if (logs[name].start == -1) {
				logs[name].start = t;
			}
			else {
				double time = t - logs[name].start;
				logs[name].curVal = time;
				logs[name].start = -1;
			}
		}
	}
}
