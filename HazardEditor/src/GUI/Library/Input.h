#pragma once

#include "Hazard.h"
#include "imgui.h"

namespace WindowElement {
	class Input {
	public:
		static bool InputField(std::string& text);
		static bool InputField(std::string& text, const char* hint);
		static bool Button(const char* name, ImVec2 size = {0, 0});
		static bool FileButton(const char* name, Hazard::Rendering::Texture2D* texture, ImVec2 size = { 0, 0 });
		static bool ResettableDragButton(const char* label, float& value, float resetValue = 0.0f, ImVec2 size = {0, 0}, 
			uint16_t buttonFont = 0, uint16_t dragFont = 0);
		static bool Vec1(const char* label, float& value, float resetValue, float columnWidth);
		static bool Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth);
		static bool Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth);

		static bool ScriptField(const char* label, std::string& moduleName, bool exists);

		static void Checkbox(const char* label, bool& value);
		static void ColorPicker(const char* label, Hazard::Color& color, bool& open);

		template<typename T>
		static bool TextureSlot(Rendering::Texture* texture, T callback) {
			bool changed = false;

			Layout::Table(2, false, "##textureSlot");
			Layout::SetColumnWidth(75);

			changed = ImageButton(texture->GetID(), { 50, 50 });
			Layout::Tooltip(texture->GetFile().c_str());
			Layout::TableNext();
			Button("Flip");
			Layout::SameLine(0, 5);
			Button("Preview");
			Layout::SameLine(0, 5);
			Button("Remove");
			callback();
			Layout::EndTable();

			return changed;
		};
		static bool Slider(const char* label, float& value, float min = 0.0f, float max = 1000.0f);

		static bool DragInt(const char* label, int& value);
		static bool DragUInt(const char* label, uint32_t& value);
		static bool DragFloat(const char* label, float& value);

		static void DynamicToggleButton(const char* offLabel, const char* onLabel, const Hazard::Color offColor, const Hazard::Color onColor, bool& modify);
		static bool ToggleButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, 
			bool& modify, ImVec2 size = {0, 0});

		static bool ButtonColorChange(const char* label, const Hazard::Color offColor, const Hazard::Color onColor, const Hazard::Color textColor, const bool state, ImVec2 size = { 0, 0 });
		static bool ColoredButton(const char* label, const Hazard::Color color, const Hazard::Color textColor, ImVec2 size = { 0, 0 });
		static bool ImageButton(uint32_t imageID, ImVec2 size = { 50, 50 });

		static bool PublicField(const std::string& name, Scripting::PublicField* field);
	};
}