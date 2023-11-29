
#include <hzrpch.h>
#include "Logger.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazard::Logging {

	Logger::Logger() : Module("Logger")
	{
		spdlog::set_pattern("%^[%T] [%n]: %v%$");
		s_CoreLogger = spdlog::stdout_color_mt("Hazard");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_ClienLogger = spdlog::stdout_color_mt("Client");
		s_ClienLogger->set_level(spdlog::level::trace);
	}
	Logger::~Logger()
	{
		s_CoreLogger.reset();
		s_ClienLogger.reset();
	}
}
