#pragma once

#include "Hazard/Utils/StringUtil.h"
#include "Hazard/RenderContext/RenderContext.h"

namespace Hazard {
	class CommandLineArgs {
	public:
		static std::unordered_map<std::string, std::vector<std::string>> m_Arguments;
		static void Init(int count, char* values[])
		{
			std::stringstream ss;
			for (int i = 1; i < count; i++) {
				ss << values[i] << " ";
			}
			std::cout << ss.str() << std::endl;
			
			std::vector<std::string> args = StringUtil::SplitString(ss.str(), '-');
			for (auto& arg : args) {
				std::string key = arg.substr(0, arg.find_first_of(' '));
				std::string value = arg.substr(arg.find_first_of(' '));
				m_Arguments[key] = StringUtil::SplitString(value, ' ');
			}
		}

		template<typename C>
		static C Get(const std::string& key)
		{
			assert(false);
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
		template<>
		static RenderAPI Get(const std::string& key) {
			return Rendering::RenderContext::StringToApi(Get<std::string>(key));
		}
	};
}