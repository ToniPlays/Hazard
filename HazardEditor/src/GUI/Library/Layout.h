#pragma once

namespace WindowLayout {
	class Layout {
	public:
		static void Table(uint8_t columns, bool border = true, const char* id = "#aa");
		static void TableNext();
		static void SameLine();
		static void SetColumnWidth(float width);
		static void EndTable();

		static void IDGroud(const char* id, void(*callback)());
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