#pragma once

#include "Utility/StringUtil.h"

namespace Hazard
{
	class CommandLineArgs {
	public:
		static std::unordered_map<std::string, std::vector<std::string>> m_Arguments;
		static void Init(std::string cmdLine) 
		{
			std::vector<std::string> args = StringUtil::SplitString(cmdLine, '-');

			for (auto& arg : args) 
			{
				if (arg.find_first_of(' ') == std::string::npos) continue;
				std::string key = arg.substr(0, arg.find_first_of(' '));
				std::string value = arg.substr(arg.find_first_of(' '));
				m_Arguments[key] = StringUtil::SplitString(value, ' ');
			}
		}
		static void Init(int count, char* values[])
		{
			std::stringstream ss;
			for (int i = 0; i < count; i++) {
				ss << values[i] << " ";
			}
			Init(ss.str());
		}

		template<typename C>
		static C Get(const std::string& key)
		{
			STATIC_ASSERT(false, C);
		}
		template<>
		static bool Get(const std::string& key)
		{
			return m_Arguments.find(key) != m_Arguments.end();
		}
		template<>
		static std::string Get(const std::string& key)
		{
			if (m_Arguments[key].size() == 0)
				return "";
			return m_Arguments[key][0];
		}
		template<>
		static std::vector<std::string> Get(const std::string& key)
		{
			return m_Arguments[key];
		}
	};
}
