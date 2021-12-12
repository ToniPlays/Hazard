#include "hzrpch.h"
#include "CommandLineArgs.h"
#include "Hazard/RenderContext/RenderContext.h"
#include "Hazard/Utils/StringUtil.h"

namespace Hazard
{
	void CommandLineArgs::Init(int count, char* values[])
	{
		std::stringstream ss;
		for (uint32_t i = 1; i < count; i++) {
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

	template<typename T>
	static T CommandLineArgs::Get(const std::string& key)
	{
		static_assert(false);
	}
	template<>
	static bool CommandLineArgs::Get(const std::string& key)
	{
		return m_Arguments.find(key) != m_Arguments.end();
	}
	template<>
	static std::string CommandLineArgs::Get(const std::string& key)
	{
		if (m_Arguments[key].size() == 0) 
			return "";
		return m_Arguments[key][0];
	}
	template<>
	static std::vector<std::string> CommandLineArgs::Get(const std::string& key)
	{
		return m_Arguments[key];
	}

	template<typename T>
	static T CommandLineArgs::Get() {
		static_assert(false);
	}

	template<>
	static RenderAPI CommandLineArgs::Get()
	{
		return Rendering::RenderContext::StringToApi(Get<std::string>("RenderAPI"));
	}
}
