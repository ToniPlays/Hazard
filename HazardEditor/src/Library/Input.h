#pragma once

#include <hzrpch.h>
#include "Layout/Layout.h"
#include "imgui.h"

using namespace Hazard;

namespace WindowElement {

	enum InputType { None = 0, ImageChange, Remove, Flip };

	class Input {
	public:
		static bool InputField(std::string& text);
		static bool InputField(std::string& text, const char* hint);
		static bool Button(const char* name, ImVec2 size = { 0, 0 });
		template<typename T>
		/*static bool FileButton(const char* name, Ref<Hazard::Rendering::Texture2D> image, T fn, ImVec2 size = {0, 0}, const std::string& id = "##id")
		{
			HZR_ASSERT(image, "What is this image");
			ImGui::BeginChild(name, size, false, ImGuiWindowFlags_NoScrollbar);
			ImGui::PushID(id.c_str());
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 0 });
			WindowLayout::Layout::Texture(image, { size.x, size.x }, { 0, 1 }, { 1, 0 });
			fn();

			WindowLayout::Layout::Separator(5);
			ImGui::PopStyleVar();
			ImGui::PopID();
			WindowLayout::Layout::NextLine(1);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 5);
			ImGui::TextWrapped("%s", name);
			ImGui::EndChild();
			return false;
		}*/
		//static bool ResettableDragButton(const char* label, float& value, float resetValue = 0.0f, ImVec2 size = { 0, 0 },
		//	uint16_t buttonFont = 0, uint16_t dragFont = 0);
		//static bool Vec1(const char* label, float& value, float resetValue, float columnWidth);
		//static bool Vec2(const char* label, glm::vec2& value, float resetValue, float columnWidth);
		//static bool Vec3(const char* label, glm::vec3& value, float resetValue, float columnWidth);

		static bool ModuleField(const char* label, std::string& moduleName, bool exists) { return false; };

		static bool Checkbox(const char* label, bool& value);
		static bool ColorPicker(const char* label, Color& color, bool& open);

		/*
		template<typename T, typename C>
		static InputType TextureSlot(Ref<Rendering::Texture2D> texture, T callback, C dropCallback) {
			InputType input = InputType::None;

			WindowLayout::Layout::Table(2, false, "##textureSlot");
			WindowLayout::Layout::SetColumnWidth(75);

			if (ImageButton(texture, { 50, 50 }))
				input = InputType::ImageChange;
			dropCallback();

			WindowLayout::Layout::Tooltip("Image");
			WindowLayout::Layout::TableNext();
			Button("Flip");
			WindowLayout::Layout::SameLine(0, 5);
			Button("Preview");
			WindowLayout::Layout::SameLine(0, 5);
			if (Button("Remove"))
				input = InputType::Remove;
			callback();
			WindowLayout::Layout::EndTable();

			return input;
		};
		*/
		static bool Slider(const char* label, float& value, float min = 0.0f, float max = 1000.0f);
		static bool DragInt(const char* label, int& value);
		static bool DragUInt(const char* label, uint32_t& value, uint32_t min = 0, uint32_t max = 0);
		static bool DragFloat(const char* label, float& value, float speed = 0.025f, float min = 0, float max = 0);

		static void DynamicToggleButton(const char* offLabel, const char* onLabel, const Color& offColor, const Color& onColor, bool& modify, ImVec2 size = { 0, 0 }) {};
		static bool ToggleButtonColorChange(const char* label, const Color& offColor, const Color& onColor, const Color& textColor,
			bool& modify, ImVec2 size = { 0, 0 }) {
			return false;
		};

		static bool ButtonColorChange(const char* label, const Color& offColor, const Color& onColor, const Color& background, const bool state, ImVec2 size = { 0, 0 }) { return false; };
		static bool ColoredButton(const char* label, const Color& color, const Color& textColor, ImVec2 size = { 0, 0 }) { return false; };
		//static bool ImageButton(Ref<Rendering::Texture2D> image, ImVec2 size = { 50, 50 });
	};
}
