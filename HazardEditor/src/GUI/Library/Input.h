#pragma once

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
	};
}