#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Color.h"
#include <imgui_internal.h>

namespace Hazard::ImUI
{
	class InputFloat
	{
	public:
		InputFloat() = default;
		InputFloat(const std::string& title, uint32_t count) : m_Title(title)
		{
			m_Configs.resize(count);
		};
		~InputFloat() = default;

		void Render();
		void ConfigureField(uint32_t index, const std::string& buttonText, ImVec4 color, bool isMixed = false)
		{
			auto& config = m_Configs[index];
			config.ButtonText = buttonText;
			config.Color = color;
			config.Mixed = isMixed;
		}
		void SetFieldValue(uint32_t index, float value, float defaultValue = 0.0f) 
		{
			auto& config = m_Configs[index];
			config.Value = value;
			config.DefaultValue = defaultValue;
		}
		bool DidAnyChange() { return m_Flags != 0; }
		bool DidChange(uint32_t field) { return m_Configs[field].DidChange; }
		float GetValue(uint32_t field) { return m_Configs[field].Value; }

	private:
		void DrawInputField(uint32_t index, ImVec2 buttonSize, float itemWidth);

	private:

		std::string m_Title;
		uint32_t m_Flags = 0;

		struct FieldConfig
		{
			std::string ButtonText;
			ImVec4 Color;
			float Value;
			float DefaultValue;
			bool Mixed = false;
			bool DidChange = false;
		};

		std::vector<FieldConfig> m_Configs;
	};
}