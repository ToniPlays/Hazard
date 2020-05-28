#pragma once
#include "Hazard/Core/Core.h"
#include "spdlog/spdlog.h"
#include "Hazard/Modules/Module.h"

namespace Hazard {


	struct ProfiledFunction {
		long double start = -1;
		long double curVal = -1;

		ProfiledFunction(long double _start) : start(_start) {}
		ProfiledFunction() {};

	};

	class HAZARD_API Logger : public Module {
	public:
		Logger();
		~Logger() {};
		void OnEnabled() override;
		void OnDisabled() override;
		static void EnableRealtime() { isRealtime = true; }
		static void DisableRealtime() { isRealtime = false; }
		static bool IsRealtime() { return isRealtime; }

	public:
		static void Assert(bool success, std::string text);
		static void CoreLog(std::string text, int level = 1);
		static void ClientLog(std::string text, int level = 1);
		static void Begin(std::string name);
		static std::unordered_map<std::string, ProfiledFunction> GetLogs() { return logs; }
		static ProfiledFunction GetLog(std::string key);
	private:
		static bool isRealtime;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::unordered_map<std::string, ProfiledFunction> logs;
	};
}


