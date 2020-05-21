#pragma once
#include "Core/Core.h"
#include "spdlog/spdlog.h"
#include "Modules/Module/Module.h"
#include "Maths/Queue.h"

namespace Hazard {


	struct ProfiledFunction {
		long double start = -1;
		long double curVal = -1;
		
		Queue<double>* data;
		ProfiledFunction(long double _start) : start(_start) {
			data = new Queue<double>(5000);
		}
		ProfiledFunction() {
			data = new Queue<double>(5000);
		};
	};

	class HAZARD_API Logger : public Module {
	public:
		Logger();
		~Logger() {};
		void OnEnable() override;
		void OnDisable() override;
		static void EnableRealtime() { isRealtime = true; }
		static void DisableRealtime() { isRealtime = false; }
		static bool IsRealtime() { return isRealtime; }

	public:
		static void CoreLog(std::string text, int level);
		static void ClientLog(std::string text, int level);
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


