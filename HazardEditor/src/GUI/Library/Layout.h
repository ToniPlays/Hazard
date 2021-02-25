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
		static void Treenode(const char* title, ImGuiTreeNodeFlags flags, void(*callback)());
		static void ContextMenu(void(*callback)());
		static bool MenuItem(const char* name);


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