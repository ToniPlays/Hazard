#pragma once

namespace Hazard {
	class CommandLineArgs {
	public:
		static void Init(int count, char* values[]);
		template<typename T>
		static T Get();
		template<typename T>
		static T Get(const std::string& key);
	private:
		static inline std::unordered_map<std::string, std::vector<std::string>> m_Arguments;
	};
}