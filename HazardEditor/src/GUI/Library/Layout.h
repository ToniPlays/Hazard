#pragma once

#include "imgui.h"

namespace WindowLayout {
	class Layout {
	public:
		static void Table(uint8_t columns, bool border = true, const char* id = "#aa");
		static void TableNext();
		static void SameLine(float offset = 0.0f, float spacing = 0.0f);
		static void SetColumnWidth(float width);
		static void EndTable();

		template<typename T>
		static void IDGroud(const char* id, T callback) {
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
		template<typename T, typename UI, typename Prop>
		static void ComponentTreenode(const char* name, UI uiCall, Prop propCall) {
			const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

			ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGui::TreeNodeEx((void*)name, flags, name);
			ImGui::PopStyleVar();

			ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
			{
				propCall();
			}
			if (open) {
				uiCall();
				ImGui::TreePop();
			}
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

		static void Separator() { ImGui::Separator(); }
		static void Text(const char* text);
		static void NextLine(float height = 15.0f);
		static void ItemWidth(float width);
		static void PushWidths(uint16_t cols);
		static void PopWidths(uint16_t cols);
		static void PopWidth();
		static void MaxWidth();
		static float GetMaxWidth();

	};
}