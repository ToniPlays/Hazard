#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"

#include "spdlog/spdlog.h"

namespace Hazard::Logging
{

	class Logger : public Module
	{
		using SPLogger = std::shared_ptr<spdlog::logger>;

	public:
		Logger();
		~Logger();

		static SPLogger GetCoreLogger() { return s_CoreLogger; }
		static SPLogger GetClientLogger() { return s_ClienLogger; }

	private:
		inline static SPLogger s_CoreLogger;
		inline static SPLogger s_ClienLogger;
	};
}