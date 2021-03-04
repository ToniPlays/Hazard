#pragma once

#include "Hazard.h"
#include "imgui.h"

namespace WindowElement {
	class Input {
	public:
		static bool InputField(std::string& text);
		static bool Button(const char* name, ImVec2 size = {0, 0});
		static void ResettableDragButton(const char* label, float& value, float resetValue = 0.0f, ImVec2 size = {0, 0}, 
			uint16_t buttonFont = 0, uint16_t dragFont = 0);
		static void Vec1(const char* label, float& value, float resetValue, float columnWidth);
		static void Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth);
		static void Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth);

		static void Checkbox(const char* label, bool& value);
		static void ColorPicker(const char* label, Hazard::Color& color);
		static bool TextureSlot(const char* label, uint32_t& index);
		static bool Slider(const char* label, float& value, float min = 0.0f, float max = 1000.0f);
	};
}