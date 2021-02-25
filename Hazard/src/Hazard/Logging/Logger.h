#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Module/Module.h"

#include "spdlog/spdlog.h"

namespace Hazard::Logging {
	class Logger : public Module::Module {
	public:
		Logger();
		~Logger();

		void PreInit() override;
		void Flush();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return coreLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return clienLogger; }


	private:
		static std::shared_ptr<spdlog::logger> coreLogger;
		static std::shared_ptr<spdlog::logger> clienLogger;
	};
}