#pragma once

namespace Hazard {
	class CommandLineArgs {
	private:
		static std::unordered_map<std::string, std::vector<std::string>> m_Arguments;
	public:
		void Init(int count, char* values[])
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

		template<>
		bool Get(const std::string& key)
		{
			return m_Arguments.find(key) != m_Arguments.end();
		}
		template<>
		std::string Get(const std::string& key)
		{
			if (m_Arguments[key].size() == 0) 
				return "";
			return m_Arguments[key][0];
		}
		template<>
		std::vector<std::string> Get(const std::string& key)
		{
			return m_Arguments[key];
		}

		template<>
		RenderAPI Get()
		{
			return Rendering::RenderContext::StringToApi(Get<std::string>("RenderAPI"));
		}
	};
}