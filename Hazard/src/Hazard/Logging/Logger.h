#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"

#include "spdlog/spdlog.h"

namespace Hazard
{
	class Logger
	{
		using SPLogger = std::shared_ptr<spdlog::logger>;

	public:
		static void Init();
        
		static SPLogger GetCoreLogger() { return s_CoreLogger; }
		static SPLogger GetClientLogger() { return s_ClienLogger; }

	private:
		inline static SPLogger s_CoreLogger;
		inline static SPLogger s_ClienLogger;
	};
}
