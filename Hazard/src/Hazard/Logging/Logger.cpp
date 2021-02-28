#pragma once

#include <hzrpch.h>
#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazard::Logging {

	std::shared_ptr<spdlog::logger> Logger::coreLogger;
	std::shared_ptr<spdlog::logger> Logger::clienLogger;

	Logger::Logger() : Module("Logger")
	{
		spdlog::set_pattern("%^[%T] [%n]: %v%$");
		coreLogger = spdlog::stdout_color_mt("Hazard");
		coreLogger->set_level(spdlog::level::trace);

		clienLogger = spdlog::stdout_color_mt("Client");
		clienLogger->set_level(spdlog::level::trace);

	}
	Logger::~Logger()
	{
		Close();
	}
	void Logger::Close()
	{
		coreLogger.reset();
		clienLogger.reset();
	}
}