#pragma once
#include "Hazard/Core/Core.h"
#include "spdlog/spdlog.h"
#include "Hazard/Modules/Module.h"

namespace Hazard {

	;
	struct Log {
		const char* text = "";
		int level = 0;

		Log() = default;
		Log(const char* _text, int _level = 0) : text(_text), level(_level) {};
	};

	struct ProfiledFn {
		const char* name;

		std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
		float timeInMillis;

		ProfiledFn* parentFn = nullptr;
		std::vector<ProfiledFn*> subFunctions = std::vector<ProfiledFn*>();

		ProfiledFn(const char* _name, ProfiledFn* parent, std::chrono::time_point<std::chrono::high_resolution_clock> start) : startTime(start),
			name(_name), parentFn(parent)
		{
		}
	};

	class HAZARD_API Logger : public Module {
	public:
		Logger() : Module("Logger") {};
		~Logger() {};
		bool OnEnabled() override;
		bool OnDisabled() override;

	public:
		static void Assert(bool success, std::string text);
		static void CoreLog(std::string text, int level = 1);
		static void ClientLog(std::string text, int level = 1);

		static void Push(const char* fn);
		static void Pop();
		static ProfiledFn* GetProfiledFn() { return profiledRoot; }
		static std::vector<Log> GetEngineLogs() { return engineLogs; };

	private:
		static ProfiledFn* profiledRoot;
		static ProfiledFn* profiled;
		static ProfiledFn* FindProfiled(const char* fn);

		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::vector<Log> engineLogs;
	};
}