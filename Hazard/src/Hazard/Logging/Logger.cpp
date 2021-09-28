#pragma once

#include <hzrpch.h>
#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazard::Logging {

	std::shared_ptr<spdlog::logger> Logger::m_CoreLogger;
	std::shared_ptr<spdlog::logger> Logger::m_ClienLogger;

	Logger::Logger() : Module("Logger")
	{
		spdlog::set_pattern("%^[%T] [%n]: %v%$");
		m_CoreLogger = spdlog::stdout_color_mt("Hazard");
		m_CoreLogger->set_level(spdlog::level::trace);

		m_ClienLogger = spdlog::stdout_color_mt("Client");
		m_ClienLogger->set_level(spdlog::level::trace);
	}
	Logger::~Logger()
	{
		m_CoreLogger.reset();
		m_ClienLogger.reset();
	}
}