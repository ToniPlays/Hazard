#pragma once
#include "Core/Core.h"
#include "spdlog/spdlog.h"

namespace Hazard {

	class HAZARD_API Log {

	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}
//Core logging macros
#define HZR_CORE_TRACE(...)   ::Hazard::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define HZR_CORE_ERROR(...)   ::Hazard::Log::GetCoreLogger()->error(__VA_ARGS__)
#define HZR_CORE_WARN(...)    ::Hazard::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define HZR_CORE_INFO(...)    ::Hazard::Log::GetCoreLogger()->info(__VA_ARGS__)
#define HZR_CORE_FATAL(...)   ::Hazard::Log::GetCoreLogger()->critical(__VA_ARGS__)

//Client logging macros
#define HZR_TRACE(...)   ::Hazard::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HZR_ERROR(...)   ::Hazard::Log::GetClientLogger()->error(__VA_ARGS__)
#define HZR_WARN(...)    ::Hazard::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HZR_INFO(...)    ::Hazard::Log::GetClientLogger()->info(__VA_ARGS__)
#define HZR_FATAL(...)   ::Hazard::Log::GetClientLogger()->critical(__VA_ARGS__)