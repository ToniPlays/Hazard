#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "ScopedVar.h"
#include "StyleManager.h"
#include "ImGuiUtils.h"
#include "HazardRenderer.h"

#include "Backend/OpenGL/Textures/OpenGLImage2D.h"

namespace UI
{
#pragma region Utility
	inline static void ShiftX(float amount) {
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + amount);
	}
	inline static void ShiftY(float amount) {
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + amount);
	}
	inline static void Shift(float x, float y) {
		ImGui::SetCursorPos({ ImGui::GetCursorPosX() + x, ImGui::GetCursorPosY() + y });
	}
	template<typename T>
	inline static void Group(const char* id, T callback) {
		ImGui::PushID(id);
		callback();
		ImGui::PopID();
	}
	static void FocusCurrentWindow() {
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
	}
	static bool NavigatedTo()
	{
		ImGuiContext& g = *GImGui;
		return g.NavJustMovedToId == g.LastItemData.ID;
	}

	inline static void Underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f) {
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
			ImGui::ColorConvertFloat4ToU32({ 0.2f, 0.2f, 0.2f, 1.0f }), 1.0f);

		if (fullWidth)
		{
			if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
				ImGui::PopColumnsBackground();
			else if (ImGui::GetCurrentTable() != nullptr)
				ImGui::TablePopBackgroundChannel();
		}
	}
	static bool Tooltip(const char* text)
	{
		if (strcmp(text, "") == 0) return false;
		if (!ImGui::IsItemHovered()) return false;

		ImGui::BeginTooltip();
		ImGui::Text(text);
		ImGui::EndTooltip();
		return true;
	}

#pragma endregion
#pragma region Input

	static bool TextField(std::string& text) {
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		if (ImGui::InputText("##InputField", buffer, sizeof(buffer))) {
			text = buffer;
			return true;
		}
		return false;
	}
	static bool TextField(std::string& text, const char* hint) {
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		if (ImGui::InputTextWithHint("##InputField", hint, buffer, sizeof(buffer))) {
			text = buffer;
			return true;
		}
		return false;
	}
	static bool InputFloatVec(const char* buttonText, float* value, float clearValue, ImVec2 buttonSize, ImFont* buttonFont, ImVec4 color)
	{
		bool modified = false;
		ImVec4 hovered = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(color, 0.9f));
		ImVec4 active = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(color, 0.8f));
		ScopedColourStack colors(ImGuiCol_Button, color, ImGuiCol_ButtonHovered, hovered, ImGuiCol_ButtonActive, active);

		ImGui::PushFont(buttonFont);
		if (ImGui::Button(buttonText, buttonSize)) {
			modified = true;
			*value = clearValue;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		std::stringstream ss;
		ss << "##" << buttonText << "_";
		if (ImGui::DragFloat(ss.str().c_str(), value)) modified = true;

		return modified;
	}
	static bool InputFloat(float& value, float clearValue = 0.0f) {

		return ImGui::DragFloat("##float", &value);
	}
	static bool InputFloat2(glm::vec2& value, float clearValue = 0.0f) {
		bool modified = false;

		ScopedStyleVar padding(ImGuiStyleVar_FrameBorderSize, 0);
		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[1];
		Style& style = StyleManager::GetCurrent();

		//X axis
		if (InputFloatVec("X", &value.x, clearValue, buttonSize, boldFont, style.Colors.AxisX)) {
			modified = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		//Y axis
		if (InputFloatVec("Y", &value.y, clearValue, buttonSize, boldFont, style.Colors.AxisY)) {
			modified = true;
		}
		ImGui::PopItemWidth();

		return modified;
	}
	static bool InputFloat3(glm::vec3& value, float clearValue = 0.0f) {

		bool modified = false;

		ScopedStyleVar padding(ImGuiStyleVar_FrameBorderSize, 0);
		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[1];
		Style& style = StyleManager::GetCurrent();

		//X axis
		if (InputFloatVec("X", &value.x, clearValue, buttonSize, boldFont, style.Colors.AxisX)) {
			modified = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		//Y axis
		if (InputFloatVec("Y", &value.y, clearValue, buttonSize, boldFont, style.Colors.AxisY)) {
			modified = true;
		}
		ImGui::PopItemWidth();
		ImGui::SameLine();
		//Z axis
		if (InputFloatVec("Z", &value.z, clearValue, buttonSize, boldFont, style.Colors.AxisZ)) {
			modified = true;
		}
		ImGui::PopItemWidth();

		return modified;
	}
	static bool InputFloat(const char* name, float& value, float clearValue = 0.0f)
	{
		bool modified = false;
		ImGui::Text(name);
		ImGui::NextColumn();
		Group(name, [&]() {
			modified = InputFloat(value, clearValue);
			});
		ImGui::NextColumn();
		return modified;
	}
	static bool InputFloat2(const char* name, glm::vec2& value, float clearValue = 0.0f)
	{
		bool modified = false;
		ImGui::Text(name);
		ImGui::NextColumn();
		Group(name, [&]() {
			modified = InputFloat2(value, clearValue);
			});
		ImGui::NextColumn();
		return modified;
	}
	static bool InputFloat3(const char* name, glm::vec3& value, float clearValue = 0.0f)
	{
		bool modified = false;
		ImGui::Text(name);
		ImGui::NextColumn();
		Group(name, [&]() {
			modified = InputFloat3(value, clearValue);
			});
		ImGui::NextColumn();
		return modified;
	}
	static bool Combo(const char* id, const char** options, uint32_t count, uint32_t& selected)
	{
		ScopedStyleVar padding(ImGuiStyleVar_FramePadding, ImVec2(4, 6));
		uint32_t currentSelection = selected;
		bool modified = false;

		if (ImGui::BeginCombo(id, options[selected]))
		{
			for (uint32_t i = 0; i < count; i++) {
				bool isSelected = i == selected;

				if (ImGui::Selectable(options[i], &currentSelection)) {
					currentSelection = i;
					modified = true;
					selected = i;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		return modified;
	}
	static bool Combo(const char* name, const char* id, const char** options, uint32_t count, uint32_t& selected) {
		ImGui::Text(name);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = Combo(id, options, count, selected);
		ImGui::NextColumn();
		return modified;
	}
	static bool ColorPicker(const char* id, Color& color) {

		bool modified = false;
		ImVec4 col = { color.r, color.g, color.b, color.a };

		if (ImGui::ColorEdit4(id, &color.r)) {
			modified = true;
		}
		return false;
	}
	static bool ColorPicker(const char* name, const char* id, Color& color)
	{
		ImGui::Text(name);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		bool modified = ColorPicker(id, color);
		ImGui::NextColumn();

		return modified;
	}

#pragma endregion
#pragma region Buttons

	static bool ColoredButton(const char* label, ImVec4 backgroundColor, ImVec4 foregroundColor, ImVec2 buttonSize = { 16, 16 }) {
		ScopedStyleColor textColor(ImGuiCol_Text, foregroundColor);
		ScopedStyleColor buttonColor(ImGuiCol_Button, backgroundColor);

		return ImGui::Button(label, buttonSize);
	}

#pragma endregion
#pragma region Images

	static ImTextureID GetImageID(Ref<HazardRenderer::Image2D>& image) {
		using namespace HazardRenderer;
		static std::unordered_map<Image2D*, ImTextureID> textureIDS;

		if (textureIDS.find(image.Raw()) != textureIDS.end()) {
			return textureIDS[image.Raw()];
		}

		switch (GraphicsContext::GetRenderAPI()) {
		case RenderAPI::OpenGL:
			ImTextureID id = (ImTextureID)image.As<OpenGL::OpenGLImage2D>()->GetID();
			textureIDS[image.Raw()] = id;
			return id;
		}

		return 0;
	}
	static void Image(Ref<HazardRenderer::Image2D>& image, ImVec2 size, ImVec2 t0 = { 0, 0 }, ImVec2 t1 = { 1, 1 }) {
		ImGui::Image(GetImageID(image), size, t0, t1);
	}
#pragma endregion
#pragma region Treenodes

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
	static bool Treenode(const char* title, void* id, ImGuiTreeNodeFlags flags, T callback) {
		if (ImGui::TreeNode(id, title, flags)) {

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
	template<typename T, typename Prop>
	static bool TreenodeWithOptions(const char* title, ImGuiTreeNodeFlags flags, T callback, Prop propCallback = nullptr) {
		bool treeOpen = false;
		bool optionsOpen = false;
		{

			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			treeOpen = ImGui::TreeNodeEx((void*)title, flags, "%s", title);
			ImGui::PopStyleVar();
			ImGui::SameLine(contentRegion.x - lineHeight * 0.5f - 12);

			Style& style = StyleManager::GetCurrent();
			ScopedColourStack colors(ImGuiCol_Button, style.Window.Header, ImGuiCol_ButtonHovered, style.Window.HeaderHovered, ImGuiCol_ButtonActive, style.Window.HeaderActive);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 0, 0 });
			if (ImGui::Button(ICON_FK_LIST_UL, ImVec2{ lineHeight, lineHeight }))
			{
				ImGui::OpenPopup("Settings");
			}
			ImGui::PopStyleVar(1);

			if (ImGui::BeginPopup("Settings")) 
			{
				propCallback();
				ImGui::EndPopup();
			}
		}

		if (treeOpen) {
			callback();
			ImGui::TreePop();
		}

		return treeOpen;
	}

#pragma endregion
#pragma region Table
	template<typename T>
	static void Table(const char* tableName, const char** columns, uint32_t columnCount, T callback) {

		float edgeOffset = 4.0f;

		ScopedStyleVar cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));
		ImVec4 bgColor = StyleManager::GetCurrent().BackgroundColor;
		const ImU32 colRowAlt = ColorWithMultiplier(bgColor, 1.2f);
		ScopedStyleColor rowColor(ImGuiCol_TableRowBg, bgColor);
		ScopedStyleColor altRowColor(ImGuiCol_TableRowBgAlt, colRowAlt);

		{
			ScopedStyleColor tableBG(ImGuiCol_ChildBg, bgColor);

			ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
				| ImGuiTableFlags_Resizable
				| ImGuiTableFlags_Reorderable
				| ImGuiTableFlags_ScrollY;

			ImGui::BeginTable(tableName, columnCount, flags, ImGui::GetContentRegionAvail());

			//Setup
			for (uint32_t i = 0; i < columnCount; i++) {
				ImGui::TableSetupColumn(columns[i]);
			}

			//Headers
			{
				const ImU32 activeColor = ColorWithMultiplier(bgColor, 1.3f);
				ScopedColourStack headerCol(ImGuiCol_HeaderHovered, activeColor, ImGuiCol_HeaderActive, activeColor);

				ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);
				ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);

				for (uint32_t i = 0; i < columnCount; i++) {
					ImGui::TableSetColumnIndex(i);
					const char* columnName = ImGui::TableGetColumnName(i);
					Group(columnName, [&]() {
						Shift(edgeOffset * 3.0f, edgeOffset * 2.0f);
						ImGui::TableHeader(columnName);
						Shift(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
						});
				}
				ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
				Underline(true, 0.0f, 5.0f);
			}

			//Draw content from callback
			callback();
			ImGui::EndTable();
		}
	}

	template<typename T>
	static bool TableRowTreeItem(const char* idText, bool selected, T callback)
	{
		bool clicked = false;
		constexpr float edgeOffset = 4.0f;
		constexpr float rowHeight = 21.0f;

		auto* window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = rowHeight;

		ImGui::TableNextRow(0, rowHeight);
		ImGui::TableNextColumn();

		window->DC.CurrLineTextBaseOffset = 3.0f;
		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = { ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x,
									rowAreaMin.y + rowHeight };

		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);

		bool isRowHovered, held;// = ImGui::ItemHoverable(ImRect(rowAreaMin, rowAreaMax), (uint64_t)(uint32_t)entity);
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(idText),
			&isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap | ImGuiButtonFlags_PressedOnClickRelease);

		ImGui::SetItemAllowOverlap();
		ImGui::PopClipRect();

		const bool isWindowFocused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);

		// Row colouring
		//--------------

		auto fillRowWithColour = [](const ImColor& colour)
		{
			for (int column = 0; column < ImGui::TableGetColumnCount(); column++)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, colour, column);
		};

		ImVec4 bgCol = StyleManager::GetCurrent().BackgroundColor;
		ImVec4 textCol = StyleManager::GetCurrent().Window.Text;

		if (selected)
		{
			if (isWindowFocused || NavigatedTo()) {
				fillRowWithColour(ColorWithMultiplier(bgCol, 1.2f));
			}
			else
			{
				const ImColor col = ColorWithMultiplier(bgCol, 0.9f);
				fillRowWithColour(col);
			}
		}

		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, textCol);

		ImGuiContext& g = *GImGui;
		auto& style = ImGui::GetStyle();
		const ImVec2 label_size = ImGui::CalcTextSize(idText, nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float text_offset_x = g.FontSize + padding.x * 2;           // Collapser arrow width + Spacing
		const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
		const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
		ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
		const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
		const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

		const bool opened = ImGui::TreeNodeWithIcon(ImGui::GetID(idText), flags, idText, nullptr);

		if (isRowClicked)
		{
			FocusCurrentWindow();
			clicked = true;
		}

		if (opened)
		{
			callback();
			ImGui::TreePop();
		}

		if (selected)
			ImGui::PopStyleColor();
		return clicked;
	}

#pragma endregion
#pragma region Menus

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
	static bool Submenu(const char* label, T callback) {
		if (ImGui::BeginMenu(label)) {
			callback();
			ImGui::EndMenu();
			return true;
		}
		return false;
	}
	template<typename T>
	static void MenuItem(const char* label, T callback) {
		if (ImGui::MenuItem(label)) {
			callback();
		}
	}

#pragma endregion

}