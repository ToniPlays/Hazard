#pragma once

#include "Hazard.h"
#include "imgui.h"

namespace WindowElement {
	class Input {
	public:
		static bool InputField(std::string& text);
		static bool Button(const char* name, ImVec2 size = {0, 0});
		static bool ResettableDragButton(const char* label, float& value, float resetValue = 0.0f, ImVec2 size = {0, 0}, 
			uint16_t buttonFont = 0, uint16_t dragFont = 0);
		static bool Vec1(const char* label, float& value, float resetValue, float columnWidth);
		static bool Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth);
		static bool Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth);

		static bool ScriptField(const char* label, std::string& moduleName, bool exists);

		static void Checkbox(const char* label, bool& value);
		static void ColorPicker(const char* label, Hazard::Color& color, bool& open);
		static bool TextureSlot(const char* label, uint32_t& index, uint32_t min = 0, uint32_t max = 10);
		static bool Slider(const char* label, float& value, float min = 0.0f, float max = 1000.0f);

		static bool DragInt(const char* label, int& value);
		static bool DragUInt(const char* label, uint32_t& value);
		static bool DragFloat(const char* label, float& value);

		static void PublicField(const char* label, Scripting::PublicField& field);
		static void DynamicToggleButton(const char* offLabel, const char* onLabel, const Hazard::Color offColor, const Hazard::Color onColor, bool& modify);
		static void ToggleButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, 
			bool& modify, ImVec2 size = {0, 0});
		static bool ButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, const bool state, ImVec2 size = { 0, 0 });
	};
}