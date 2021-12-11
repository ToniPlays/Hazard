#pragma once

namespace Hazard {
	class CommandLineArgs {
	public:
		static void Init(int count, char* values[]);
		template<typename T>
		static T Get();
	private:
		static inline std::unordered_map<std::string, std::string> m_Arguments;
	};
}