#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "Platform/GLFW/FontAwesome.h"
#include "Library/Style.h"
#include "Hazard/Entity/Entity.h"

namespace WindowLayout 
{
	class Layout {
	public:
		//static ImTextureID GetTextureID(const Ref<Hazard::Rendering::Image2D>& image);
		//static void Image(const Ref<Hazard::Rendering::Image2D>& image, ImVec2 size, ImVec2 t0, ImVec2 t1);
		//static void Texture(const Ref<Hazard::Rendering::Texture2D>& image, ImVec2 size, ImVec2 t0, ImVec2 t1);
		static void Table(uint8_t columns, bool border = true, const char* id = "#aa");
		inline static void TableNext() { ImGui::NextColumn(); }
		static void SameLine(float offset = 0.0f, float spacing = 0.0f);
		static void SetColumnWidth(float width);
		inline static float GetColumnWidth();
		inline static void EndTable() { ImGui::Columns(1); }
		static bool Tooltip(const char* text);
		static void Separator(float height = 1.0f);
		static void Text(const std::string& text);
		static void TextColored(const char* text, const Color& color);
		static void NextLine(float height = 15.0f);
		static void ItemWidth(float width);
		static void PushWidths(uint16_t cols);
		static void PopWidths(uint16_t cols);
		inline static void PopWidth() { ImGui::PopItemWidth(); }
		inline static void MaxWidth() { ItemWidth(ImGui::GetContentRegionAvail().x); }
		inline static float GetMaxWidth() { return ImGui::GetContentRegionAvail().x; }
		inline static void Shift(float x, float y) {
			ShiftX(x);
			ShiftY(y);
		}
		inline static void ShiftX(float amount) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + amount);
		}
		inline static void ShiftY(float amount) {
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + amount);
		}

		template<typename T>
		static void IDGroup(const char* id, T callback) {
			ImGui::PushID(id);
			callback();
			ImGui::PopID();
		}

		template<typename T>
		static bool Treenode(const char* title, ImGuiTreeNodeFlags flags, T callback)
		{
			if (ImGui::TreeNodeEx(title, flags)) {
				callback();
				ImGui::TreePop();
				return true;
			}
			return false;
		}
		template<typename T>
		static bool Treenode(void* id, const char* title, ImGuiTreeNodeFlags flags, T callback)
		{
			if (ImGui::TreeNodeEx(id, flags, title)) {
				callback();
				ImGui::TreePop();
				return true;
			}
			return false;
		}
		static bool Treenode(const char* title, ImGuiTreeNodeFlags flags)
		{
			if (ImGui::TreeNodeEx(title, flags)) {
				return true;
			}
			return false;
		}

		template<typename T, typename UI, typename Prop>
		static void ComponentTreenode(Hazard::ECS::Entity entity, const char* name, UI uiCall, Prop propCall) {

			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)name, flags, "%s", name);
			bool removed = false;
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f - 5);
			if (ImGui::Button(ICON_FK_LIST_UL, ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			if (ImGui::BeginPopup("ComponentSettings")) {
				propCall();
				Layout::Separator();
				Layout::MenuItem("Remove component", [&removed]() {
					removed = true;
				});

				ImGui::EndPopup();
			}
			if (open) {
				uiCall();
				ImGui::TreePop();
			}
			if (removed) entity.RemoveComponent<T>();
		}

		template<typename T>
		static bool ContextMenu(T callback) {
			if (ImGui::BeginPopupContextWindow(0, 1, false)) {
				callback();
				ImGui::EndPopup();
				return true;
			}
			return false;
		}
		template<typename T>
		static void Menu(const char* name, T callback) {
			if (ImGui::BeginMenu(name)) {
				callback();
				ImGui::EndMenu();
			}
		}
		template<typename T>
		static void MenuItem(const char* name, T callback) {
			if (ImGui::MenuItem(name)) 
				callback();
		}
		static void Underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f)
		{
			if (fullWidth)
			{
				if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
					ImGui::PushColumnsBackground();
				else if (ImGui::GetCurrentTable() != nullptr)
					ImGui::TablePushBackgroundChannel();
			}

			const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
			const ImVec2 cursor = ImGui::GetCursorScreenPos();

			ImGui::GetWindowDrawList()->AddLine(ImVec2(cursor.x + offsetX, cursor.y + offsetY),
				ImVec2(cursor.x + width, cursor.y + offsetY),
				Style::GetStyleColor32(ColorType::Background), 1.0f);

			if (fullWidth)
			{
				if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
					ImGui::PopColumnsBackground();
				else if (ImGui::GetCurrentTable() != nullptr)
					ImGui::TablePopBackgroundChannel();
			}
		}
		static bool NavigatedTo()
		{
			ImGuiContext& g = *GImGui;
			return g.NavJustMovedToId == g.LastItemData.ID;
		}
	};
}
