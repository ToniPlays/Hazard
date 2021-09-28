#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module.h"

#include "spdlog/spdlog.h"

namespace Hazard::Logging {

	class Logger : public Module::Module {
		using SPLogger = std::shared_ptr<spdlog::logger>;

	public:
		Logger();
		~Logger();

		static SPLogger GetCoreLogger() { return m_CoreLogger; }
		static SPLogger GetClientLogger() { return m_ClienLogger; }

	private:
		static SPLogger m_CoreLogger;
		static SPLogger m_ClienLogger;
	};
}