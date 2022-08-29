#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "ScopedVar.h"
#include "StyleManager.h"
#include "ImGuiUtils.h"
#include "Hazard/Core/Core.h"
#include "HazardRenderer.h"
#include "Hazard/RenderContext/Texture2D.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"

#include "../ImGui_Backend/imgui_impl_vulkan.h"

namespace Hazard::ImUI
{
	constexpr int vectorItemSpacingX = 4;

#pragma region Utility
	inline static void ShiftX(float amount) 
	{
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + amount);
	}
	inline static void ShiftY(float amount)
	{
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + amount);
	}
	inline static void Shift(float x, float y) 
	{
		ImGui::SetCursorPos({ ImGui::GetCursorPosX() + x, ImGui::GetCursorPosY() + y });
	}

	static ImRect RectOffset(const ImVec2& topLeft, const ImVec2& bottomRight, float xOffset, float yOffset) 
	{
		return ImRect({ topLeft.x + xOffset, topLeft.y + yOffset }, { bottomRight.x + xOffset, bottomRight.y + yOffset });
	}

	template<typename T>
	inline static void Group(void* ptr_id, T callback) 
	{
		ImGui::PushID(ptr_id);
		callback();
		ImGui::PopID();
	}
	template<typename T>
	inline static void Group(const char* id, T callback) 
	{
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

	inline static void Underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f) 
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
		ScopedStyleStack padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f), ImGuiStyleVar_WindowRounding, 4.0f);
		ImGui::BeginTooltip();
		ImGui::Text(text);
		ImGui::EndTooltip();
		return true;
	}

#pragma endregion
#pragma region Input

	static bool TextField(std::string& text, const char* id = "##InputField") 
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		if (ImGui::InputText(id, buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) 
		{
			text = buffer;
			return true;
		}
		return false;
	}

	static bool TextArea(std::string& text, int minLines, int maxLines) 
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		if (ImGui::InputTextMultiline("##InputField", buffer, sizeof(buffer), { 0, ImGui::GetTextLineHeight() * minLines })) 
		{
			text = buffer;
			return true;
		}
		return false;
	}

	static bool TextFieldWithHint(std::string& text, const char* hint) 
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

		if (ImGui::InputTextWithHint("##InputField", hint, buffer, sizeof(buffer)))
		{
			text = buffer;
			ImGui::PopStyleVar();
			return true;
		}
		ImGui::PopStyleVar();
		return false;
	}
	static bool InputFloatVec(const char* buttonText, float* value, float clearValue, float width, ImVec2 buttonSize, ImFont* buttonFont, ImVec4 color)
	{
		bool modified = false;
		ImVec4 hovered = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(color, 0.9f));
		ImVec4 active = ImGui::ColorConvertU32ToFloat4(ColorWithMultiplier(color, 0.8f));
		ScopedColourStack colors(ImGuiCol_Button, color, ImGuiCol_ButtonHovered, hovered, ImGuiCol_ButtonActive, active);

		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

		ImGui::PushFont(buttonFont);
		if (ImGui::Button(buttonText, buttonSize)) {
			modified = true;
			*value = clearValue;
		}
		ImGui::PopFont();
		ImGui::SameLine();
		std::stringstream ss;
		ss << "##" << buttonText << "_";
		ImGui::SetNextItemWidth(width);
		if (ImGui::DragFloat(ss.str().c_str(), value)) modified = true;

		return modified;
	}

	static bool InputInt(int& value, int clearValue, int min, int max) 
	{
		return ImGui::DragInt("##int", &value, 1, min, max);
	}
	static bool InputUInt(uint64_t& value, uint64_t clearValue = 0.0f, uint64_t min = 0, uint64_t max = 0) 
	{
		return ImGui::DragInt("##uint", (int*)&value, 0.5f, min, max);
	}

	static bool InputSliderFloat(float& value, float clearValue = 0.0f, float min = 0.0f, float max = 0.0f) 
	{
		return ImGui::SliderFloat("#sliderFloat", &value, min, max);
	}
	static bool InputFloat(float& value, float clearValue = 0.0f, float speed = 1.0f, float min = 0.0, float max = 0.0) 
	{
		return ImGui::DragFloat("##float", &value, speed, min, max);
	}
	static bool InputDouble(double& value, double clearValue = 0.0) 
	{
		return ImGui::InputDouble("##double", &value, 0.5);
	}

	static bool InputInt(int& value, int clearValue = 0.0) 
	{
		return ImGui::InputInt("##int", &value, 1);
	}

	static bool InputFloat2(glm::vec2& value, float clearValue = 0.0f) 
	{
		bool modified = false;
		ScopedStyleVar padding(ImGuiStyleVar_FrameBorderSize, 0.0f);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		float itemWidth = (ImGui::GetContentRegionAvailWidth() - vectorItemSpacingX) / 2.0f - buttonSize.x;
		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(vectorItemSpacingX, 0.0f));

		ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[1];
		Style& style = StyleManager::GetCurrent();

		//X axis
		if (InputFloatVec("X", &value.x, clearValue, itemWidth, buttonSize, boldFont, style.Colors.AxisX)) {
			modified = true;
		}
		ImGui::SameLine();
		//Y axis
		if (InputFloatVec("Y", &value.y, clearValue, itemWidth, buttonSize, boldFont, style.Colors.AxisY)) {
			modified = true;
		}
		return modified;
	}
	static bool InputFloat3(glm::vec3& value, float clearValue = 0.0f)
	{
		bool modified = false;
		ScopedStyleVar padding(ImGuiStyleVar_FrameBorderSize, 0.0f);

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
		float itemWidth = (ImGui::GetContentRegionAvailWidth() - vectorItemSpacingX) / 3.0f - buttonSize.x;

		ScopedStyleVar spacing(ImGuiStyleVar_ItemSpacing, ImVec2(vectorItemSpacingX, 0.0f));

		ImFont* boldFont = ImGui::GetIO().Fonts->Fonts[1];
		Style& style = StyleManager::GetCurrent();

		//X axis
		if (InputFloatVec("X", &value.x, clearValue, itemWidth, buttonSize, boldFont, style.Colors.AxisX)) 
			modified = true;
		ImGui::SameLine();
		//Y axis
		if (InputFloatVec("Y", &value.y, clearValue, itemWidth, buttonSize, boldFont, style.Colors.AxisY)) 
			modified = true;
		ImGui::SameLine();
		//Z axis
		if (InputFloatVec("Z", &value.z, clearValue, itemWidth, buttonSize, boldFont, style.Colors.AxisZ)) 
			modified = true;

		return modified;
	}
	static bool InputFloat(const char* name, float& value, float clearValue = 0.0f, float speed = 1.0f, float min = 0.0, float max = 0.0)
	{
		bool modified = false;
		ImGui::Text(name);
		ImGui::NextColumn();
		Group(name, [&]() {
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
			modified = InputFloat(value, clearValue, speed, min, max);
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
		ShiftY(4.0f);
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
	static bool Combo(const char* name, const char* id, const char** options, uint32_t count, uint32_t& selected)
	{
		ShiftY(4.0f);
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
		return modified;
	}
	static bool ColorPicker(const char* name, const char* id, Color& color)
	{
		ShiftY(4.0f);
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

	static ImTextureID GetImageID(Ref<HazardRenderer::Image2D> image)
	{
		using namespace HazardRenderer;

		switch (GraphicsContext::GetRenderAPI()) {
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:
		{
			ImTextureID id = (ImTextureID)image.As<OpenGL::OpenGLImage2D>()->GetID();
			return id;
		}
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:
		{
			Ref<Vulkan::VulkanImage2D> vkImage = image.As<Vulkan::VulkanImage2D>();
			const VkDescriptorImageInfo& imageInfo = vkImage->GetImageDescriptor();
			if (!imageInfo.imageView) return nullptr;

			ImTextureID id = ImGui_ImplVulkan_AddTexture(imageInfo.sampler, imageInfo.imageView, imageInfo.imageLayout);
			return id;
		}
#endif
		}

		HZR_CORE_ASSERT(false, "Undefined ImageID");
		return 0;
	}
	static void Image(Ref<HazardRenderer::Image2D> image, ImVec2 size, ImVec2 t0 = { 0, 1 }, ImVec2 t1 = { 1, 0 })
	{
		ImGui::Image(GetImageID(image), size, t0, t1);
	}
	static bool TextureSlot(Ref<Hazard::Image2DAsset> image)
	{
		std::string text = "Source image here";

		constexpr float size = 32.0f;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth() - size - 5.0f);
		{
			ImUI::TextField(text);

		}
		ImGui::SameLine(0, 5);

		if (image)
			Image(image->GetCoreImage(), { size, size });
		else 
		{
			ImVec2 pos = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(pos, { pos.x + size, pos.y + size }, ImGui::GetColorU32({ 1.0, 1.0, 1.0, 1.0 }));
			ImUI::ShiftY(36.0f);
		}
		ImUI::ShiftY(2.0f);
		return false;
	}
	template<typename T>
	static bool TextureSlot(const char* name, Ref<Image2DAsset> image, T callback)
	{

		bool modified = false;
		ImUI::ShiftY(8.0f);

		ImGui::Text(name);
		ImGui::NextColumn();
		Group(name, [&]() {
			modified = TextureSlot(image);
			callback();
			});
		ImGui::NextColumn();
		return modified;
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
	static bool Treenode(const char* title, void* id, ImGuiTreeNodeFlags flags, T callback)
	{
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
	static bool TreenodeWithOptions(const char* title, ImGuiTreeNodeFlags flags, T callback, Prop propCallback = nullptr)
	{
		return TreenodeWithOptions(title, flags, callback, propCallback, []() {});
	};

	template<typename T, typename Prop, typename DragSource>
	static bool TreenodeWithOptions(const char* title, ImGuiTreeNodeFlags flags, T callback, Prop propCallback = nullptr, DragSource dragSource = nullptr)
	{
		bool treeOpen = false;
		bool optionsOpen = false;
		{

			ImVec2 contentRegion = ImGui::GetContentRegionAvail();
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

			treeOpen = ImGui::TreeNodeEx((void*)title, flags, "%s", title);

			dragSource();

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
	static void Table(const char* tableName, const char** columns, uint32_t columnCount, ImVec2 size, T callback)
	{

		if (size.x <= 0.0f || size.y <= 0.0f) return;

		float edgeOffset = 4.0f;

		ScopedStyleVar cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));
		ImVec4 bgColor = StyleManager::GetCurrent().BackgroundColor;
		const ImU32 colRowAlt = ColorWithMultiplier(bgColor, 1.2f);
		ScopedStyleColor rowColor(ImGuiCol_TableRowBg, bgColor);
		ScopedStyleColor altRowColor(ImGuiCol_TableRowBgAlt, colRowAlt);
		ScopedStyleColor tableBG(ImGuiCol_ChildBg, bgColor);

		ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
			| ImGuiTableFlags_Resizable
			| ImGuiTableFlags_Reorderable
			| ImGuiTableFlags_ScrollY
			| ImGuiTableFlags_RowBg;

		if (!ImGui::BeginTable(tableName, columnCount, flags, size)) {
			return;
		}

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

	template<typename T>
	static void Table(const char* tableName, const char** columns, uint32_t columnCount, T callback) {
		Table(tableName, columns, columnCount, ImGui::GetContentRegionAvail(), callback);
	}

	template<typename T>
	static bool TableRowTreeItem(const char* id, const char* text, bool selected, T callback)
	{
		bool clicked = false;
		constexpr float edgeOffset = 4.0f;
		constexpr float rowHeight = 21.0f;

		auto* window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = rowHeight;

		ImGui::TableNextRow(0, rowHeight);
		ImGui::TableNextColumn();
		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		window->DC.CurrLineTextBaseOffset = 3.0f;
		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = { ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x,
									rowAreaMin.y + rowHeight };

		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);

		bool isRowHovered, held;// = ImGui::ItemHoverable(ImRect(rowAreaMin, rowAreaMax), (uint64_t)(uint32_t)entity);
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(id),
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
		const ImVec2 label_size = ImGui::CalcTextSize(text, nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float text_offset_x = g.FontSize + padding.x * 2;           // Collapser arrow width + Spacing
		const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
		const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
		ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
		const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
		const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

		const bool opened = ImGui::TreeNodeWithIcon(ImGui::GetID(id), flags, text, nullptr);

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
	static bool TableRowClickable(const char* id, float rowHeight)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		window->DC.CurrLineSize.y = rowHeight;

		ImGui::TableNextRow(0, rowHeight);
		ImGui::TableNextColumn();

		window->DC.CurrLineTextBaseOffset = 3.0f;
		const ImVec2 rowAreaMin = ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), 0).Min;
		const ImVec2 rowAreaMax = { ImGui::TableGetCellBgRect(ImGui::GetCurrentTable(), ImGui::TableGetColumnCount() - 1).Max.x,
									rowAreaMin.y + rowHeight };

		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);

		bool isRowHovered, held;// = ImGui::ItemHoverable(ImRect(rowAreaMin, rowAreaMax), (uint64_t)(uint32_t)entity);
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(id),
			&isRowHovered, &held, ImGuiButtonFlags_AllowItemOverlap | ImGuiButtonFlags_PressedOnDoubleClick);

		ImGui::SetItemAllowOverlap();
		ImGui::PopClipRect();

		return isRowClicked;
	}

#pragma endregion
#pragma region Menus

	template<typename T>
	static bool ContextMenu(T callback) {
		ScopedStyleStack padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f), ImGuiStyleVar_PopupRounding, 4.0f, ImGuiStyleVar_ItemSpacing, ImVec2(16.0f, 4.0f), ImGuiStyleVar_ChildBorderSize, 0);
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
	static void Separator(ImVec2 size, ImVec4 color);

	static void MenuHeader(const char* label) {
		ImGui::PushID(label);
		const Style& style = StyleManager::GetCurrent();
		ImGui::TextColored(style.Window.TextDark, label);
		ImGui::SameLine(0, 5);
		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::ShiftY(ImGui::GetTextLineHeight() * 0.5f);
		ImUI::Separator({ size.x, 2.0f }, style.Window.TextDark);
		ImUI::ShiftY(ImGui::GetTextLineHeight() * -0.5f);
		ImGui::PopID();
	}

#pragma endregion
#pragma region Layout

	static void Separator(ImVec2 size, ImVec4 color)
	{
		ImGui::PushStyleColor(ImGuiCol_ChildBg, color);
		ImGui::BeginChild("sep", size);
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

#pragma endregion
#pragma region DragDrop

	template<typename T, typename Callback>
	static bool DragSource(const char* type, T* data, Callback callback)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {

			ImGui::SetDragDropPayload(type, (void*)data, sizeof(T));
			callback();
			ImGui::EndDragDropSource();
			return true;
		}
		return false;
	}
	template<typename T, typename Callback>
	static bool DragSource(const AssetType& type, T* data, Callback callback)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID)) {
			ImGui::SetDragDropPayload(Utils::AssetTypeToString(type), (void*)data, sizeof(T));
			callback();
			ImGui::EndDragDropSource();
			return true;
		}
		return false;
	}

	template<typename T, typename Callback>
	static bool DropTarget(const char* type, Callback callback)
	{
		bool accepted = false;
		if (ImGui::BeginDragDropTarget()) {

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
			if (payload) {
				callback(*(T*)payload->Data);
				accepted = true;
			}

			ImGui::EndDragDropTarget();
		}
		return accepted;
	}
	template<typename T, typename Callback>
	inline static bool DropTarget(const AssetType& type, Callback callback)
	{
		bool accepted = false;
		if (ImGui::BeginDragDropTarget())
		{
			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(Utils::AssetTypeToString(type));
			if (payload) {
				callback(*(T*)payload->Data);
				accepted = true;
			}

			ImGui::EndDragDropTarget();
		}
		return accepted;
	}

#pragma endregion

	// Exposed resize behavior for native OS windows
	static bool UpdateWindowManualResize(ImGuiWindow* window, ImVec2& newSize, ImVec2& newPosition)
	{

		auto CalcWindowSizeAfterConstraint = [](ImGuiWindow* window, const ImVec2& size_desired)
		{
			ImGuiContext& g = *GImGui;
			ImVec2 new_size = size_desired;
			if (g.NextWindowData.Flags & ImGuiNextWindowDataFlags_HasSizeConstraint)
			{
				// Using -1,-1 on either X/Y axis to preserve the current size.
				ImRect cr = g.NextWindowData.SizeConstraintRect;
				new_size.x = (cr.Min.x >= 0 && cr.Max.x >= 0) ? ImClamp(new_size.x, cr.Min.x, cr.Max.x) : window->SizeFull.x;
				new_size.y = (cr.Min.y >= 0 && cr.Max.y >= 0) ? ImClamp(new_size.y, cr.Min.y, cr.Max.y) : window->SizeFull.y;
				if (g.NextWindowData.SizeCallback)
				{
					ImGuiSizeCallbackData data;
					data.UserData = g.NextWindowData.SizeCallbackUserData;
					data.Pos = window->Pos;
					data.CurrentSize = window->SizeFull;
					data.DesiredSize = new_size;
					g.NextWindowData.SizeCallback(&data);
					new_size = data.DesiredSize;
				}
				new_size.x = IM_FLOOR(new_size.x);
				new_size.y = IM_FLOOR(new_size.y);
			}

			// Minimum size
			if (!(window->Flags & (ImGuiWindowFlags_ChildWindow | ImGuiWindowFlags_AlwaysAutoResize)))
			{
				ImGuiWindow* window_for_height = (window->DockNodeAsHost && window->DockNodeAsHost->VisibleWindow) ? window->DockNodeAsHost->VisibleWindow : window;
				const float decoration_up_height = window_for_height->TitleBarHeight() + window_for_height->MenuBarHeight();
				new_size = ImMax(new_size, g.Style.WindowMinSize);
				new_size.y = ImMax(new_size.y, decoration_up_height + ImMax(0.0f, g.Style.WindowRounding - 1.0f)); // Reduce artifacts with very small windows
			}
			return new_size;
		};

		auto CalcWindowAutoFitSize = [CalcWindowSizeAfterConstraint](ImGuiWindow* window, const ImVec2& size_contents)
		{
			ImGuiContext& g = *GImGui;
			ImGuiStyle& style = g.Style;
			const float decoration_up_height = window->TitleBarHeight() + window->MenuBarHeight();
			ImVec2 size_pad{ window->WindowPadding.x * 2.0f, window->WindowPadding.y * 2.0f };
			ImVec2 size_desired = { size_contents.x + size_pad.x + 0.0f, size_contents.y + size_pad.y + decoration_up_height };
			if (window->Flags & ImGuiWindowFlags_Tooltip)
			{
				// Tooltip always resize
				return size_desired;
			}
			else
			{
				// Maximum window size is determined by the viewport size or monitor size
				const bool is_popup = (window->Flags & ImGuiWindowFlags_Popup) != 0;
				const bool is_menu = (window->Flags & ImGuiWindowFlags_ChildMenu) != 0;
				ImVec2 size_min = style.WindowMinSize;
				if (is_popup || is_menu) // Popups and menus bypass style.WindowMinSize by default, but we give then a non-zero minimum size to facilitate understanding problematic cases (e.g. empty popups)
					size_min = ImMin(size_min, ImVec2(4.0f, 4.0f));

				// FIXME-VIEWPORT-WORKAREA: May want to use GetWorkSize() instead of Size depending on the type of windows?
				ImVec2 avail_size = window->Viewport->Size;
				if (window->ViewportOwned)
					avail_size = ImVec2(FLT_MAX, FLT_MAX);
				const int monitor_idx = window->ViewportAllowPlatformMonitorExtend;
				if (monitor_idx >= 0 && monitor_idx < g.PlatformIO.Monitors.Size)
					avail_size = g.PlatformIO.Monitors[monitor_idx].WorkSize;
				ImVec2 size_auto_fit = ImClamp(size_desired, size_min, ImMax(size_min, { avail_size.x - style.DisplaySafeAreaPadding.x * 2.0f,
																						avail_size.y - style.DisplaySafeAreaPadding.y * 2.0f }));

				// When the window cannot fit all contents (either because of constraints, either because screen is too small),
				// we are growing the size on the other axis to compensate for expected scrollbar. FIXME: Might turn bigger than ViewportSize-WindowPadding.
				ImVec2 size_auto_fit_after_constraint = CalcWindowSizeAfterConstraint(window, size_auto_fit);
				bool will_have_scrollbar_x = (size_auto_fit_after_constraint.x - size_pad.x - 0.0f < size_contents.x && !(window->Flags & ImGuiWindowFlags_NoScrollbar) && (window->Flags & ImGuiWindowFlags_HorizontalScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysHorizontalScrollbar);
				bool will_have_scrollbar_y = (size_auto_fit_after_constraint.y - size_pad.y - decoration_up_height < size_contents.y && !(window->Flags& ImGuiWindowFlags_NoScrollbar)) || (window->Flags & ImGuiWindowFlags_AlwaysVerticalScrollbar);
				if (will_have_scrollbar_x)
					size_auto_fit.y += style.ScrollbarSize;
				if (will_have_scrollbar_y)
					size_auto_fit.x += style.ScrollbarSize;
				return size_auto_fit;
			}
		};

		ImGuiContext& g = *GImGui;

		// Decide if we are going to handle borders and resize grips
		const bool handle_borders_and_resize_grips = (window->DockNodeAsHost || !window->DockIsActive);

		if (!handle_borders_and_resize_grips || window->Collapsed)
			return false;

		const ImVec2 size_auto_fit = CalcWindowAutoFitSize(window, window->ContentSizeIdeal);

		// Handle manual resize: Resize Grips, Borders, Gamepad
		int border_held = -1;
		ImU32 resize_grip_col[4] = {};
		const int resize_grip_count = g.IO.ConfigWindowsResizeFromEdges ? 2 : 1; // Allow resize from lower-left if we have the mouse cursor feedback for it.
		const float resize_grip_draw_size = IM_FLOOR(ImMax(g.FontSize * 1.10f, window->WindowRounding + 1.0f + g.FontSize * 0.2f));
		window->ResizeBorderHeld = (signed char)border_held;

		//const ImRect& visibility_rect;

		struct ImGuiResizeBorderDef
		{
			ImVec2 InnerDir;
			ImVec2 SegmentN1, SegmentN2;
			float  OuterAngle;
		};
		static const ImGuiResizeBorderDef resize_border_def[4] =
		{
			{ ImVec2(+1, 0), ImVec2(0, 1), ImVec2(0, 0), IM_PI * 1.00f }, // Left
			{ ImVec2(-1, 0), ImVec2(1, 0), ImVec2(1, 1), IM_PI * 0.00f }, // Right
			{ ImVec2(0, +1), ImVec2(0, 0), ImVec2(1, 0), IM_PI * 1.50f }, // Up
			{ ImVec2(0, -1), ImVec2(1, 1), ImVec2(0, 1), IM_PI * 0.50f }  // Down
		};

		// Data for resizing from corner
		struct ImGuiResizeGripDef
		{
			ImVec2  CornerPosN;
			ImVec2  InnerDir;
			int     AngleMin12, AngleMax12;
		};
		static const ImGuiResizeGripDef resize_grip_def[4] =
		{
			{ ImVec2(1, 1), ImVec2(-1, -1), 0, 3 },  // Lower-right
			{ ImVec2(0, 1), ImVec2(+1, -1), 3, 6 },  // Lower-left
			{ ImVec2(0, 0), ImVec2(+1, +1), 6, 9 },  // Upper-left (Unused)
			{ ImVec2(1, 0), ImVec2(-1, +1), 9, 12 }  // Upper-right (Unused)
		};

		auto CalcResizePosSizeFromAnyCorner = [CalcWindowSizeAfterConstraint](ImGuiWindow* window, const ImVec2& corner_target, const ImVec2& corner_norm, ImVec2* out_pos, ImVec2* out_size)
		{
			ImVec2 pos_min = ImLerp(corner_target, window->Pos, corner_norm);                // Expected window upper-left
			ImVec2 pos_max = ImLerp({ window->Pos.x + window->Size.x, window->Pos.y + window->Size.y }, corner_target, corner_norm); // Expected window lower-right
			ImVec2 size_expected = { pos_max.x - pos_min.x,  pos_max.y - pos_min.y };
			ImVec2 size_constrained = CalcWindowSizeAfterConstraint(window, size_expected);
			*out_pos = pos_min;
			if (corner_norm.x == 0.0f)
				out_pos->x -= (size_constrained.x - size_expected.x);
			if (corner_norm.y == 0.0f)
				out_pos->y -= (size_constrained.y - size_expected.y);
			*out_size = size_constrained;
		};

		auto GetResizeBorderRect = [](ImGuiWindow* window, int border_n, float perp_padding, float thickness)
		{
			ImRect rect = window->Rect();
			if (thickness == 0.0f)
			{
				rect.Max.x -= 1;
				rect.Max.y -= 1;
			}
			if (border_n == ImGuiDir_Left) { return ImRect(rect.Min.x - thickness, rect.Min.y + perp_padding, rect.Min.x + thickness, rect.Max.y - perp_padding); }
			if (border_n == ImGuiDir_Right) { return ImRect(rect.Max.x - thickness, rect.Min.y + perp_padding, rect.Max.x + thickness, rect.Max.y - perp_padding); }
			if (border_n == ImGuiDir_Up) { return ImRect(rect.Min.x + perp_padding, rect.Min.y - thickness, rect.Max.x - perp_padding, rect.Min.y + thickness); }
			if (border_n == ImGuiDir_Down) { return ImRect(rect.Min.x + perp_padding, rect.Max.y - thickness, rect.Max.x - perp_padding, rect.Max.y + thickness); }
			IM_ASSERT(0);
			return ImRect();
		};

		static const float WINDOWS_HOVER_PADDING = 4.0f;                        // Extend outside window for hovering/resizing (maxxed with TouchPadding) and inside windows for borders. Affect FindHoveredWindow().
		static const float WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TIMER = 0.04f;    // Reduce visual noise by only highlighting the border after a certain time.

		auto& style = g.Style;
		ImGuiWindowFlags flags = window->Flags;

		if (/*(flags & ImGuiWindowFlags_NoResize) || */(flags & ImGuiWindowFlags_AlwaysAutoResize) || window->AutoFitFramesX > 0 || window->AutoFitFramesY > 0)
			return false;
		if (window->WasActive == false) // Early out to avoid running this code for e.g. an hidden implicit/fallback Debug window.
			return false;

		bool ret_auto_fit = false;
		const int resize_border_count = g.IO.ConfigWindowsResizeFromEdges ? 4 : 0;
		const float grip_draw_size = IM_FLOOR(ImMax(g.FontSize * 1.35f, window->WindowRounding + 1.0f + g.FontSize * 0.2f));
		const float grip_hover_inner_size = IM_FLOOR(grip_draw_size * 0.75f);
		const float grip_hover_outer_size = g.IO.ConfigWindowsResizeFromEdges ? WINDOWS_HOVER_PADDING : 0.0f;

		ImVec2 pos_target(FLT_MAX, FLT_MAX);
		ImVec2 size_target(FLT_MAX, FLT_MAX);

		// Calculate the range of allowed position for that window (to be movable and visible past safe area padding)
		// When clamping to stay visible, we will enforce that window->Pos stays inside of visibility_rect.
		ImRect viewport_rect(window->Viewport->GetMainRect());
		ImRect viewport_work_rect(window->Viewport->GetWorkRect());
		ImVec2 visibility_padding = ImMax(style.DisplayWindowPadding, style.DisplaySafeAreaPadding);
		ImRect visibility_rect({ viewport_work_rect.Min.x + visibility_padding.x, viewport_work_rect.Min.y + visibility_padding.y },
			{ viewport_work_rect.Max.x - visibility_padding.x, viewport_work_rect.Max.y - visibility_padding.y });

		// Clip mouse interaction rectangles within the viewport rectangle (in practice the narrowing is going to happen most of the time).
		// - Not narrowing would mostly benefit the situation where OS windows _without_ decoration have a threshold for hovering when outside their limits.
		//   This is however not the case with current backends under Win32, but a custom borderless window implementation would benefit from it.
		// - When decoration are enabled we typically benefit from that distance, but then our resize elements would be conflicting with OS resize elements, so we also narrow.
		// - Note that we are unable to tell if the platform setup allows hovering with a distance threshold (on Win32, decorated window have such threshold).
		// We only clip interaction so we overwrite window->ClipRect, cannot call PushClipRect() yet as DrawList is not yet setup.
		const bool clip_with_viewport_rect = !(g.IO.BackendFlags & ImGuiBackendFlags_HasMouseHoveredViewport) || (g.IO.MouseHoveredViewport != window->ViewportId) || !(window->Viewport->Flags & ImGuiViewportFlags_NoDecoration);
		if (clip_with_viewport_rect)
			window->ClipRect = window->Viewport->GetMainRect();

		// Resize grips and borders are on layer 1
		window->DC.NavLayerCurrent = ImGuiNavLayer_Menu;

		// Manual resize grips
		ImGui::PushID("#RESIZE");
		for (int resize_grip_n = 0; resize_grip_n < resize_grip_count; resize_grip_n++)
		{
			const ImGuiResizeGripDef& def = resize_grip_def[resize_grip_n];

			const ImVec2 corner = ImLerp(window->Pos, { window->Pos.x + window->Size.x, window->Pos.y + window->Size.y }, def.CornerPosN);

			// Using the FlattenChilds button flag we make the resize button accessible even if we are hovering over a child window
			bool hovered, held;
			const ImVec2 min = { corner.x - def.InnerDir.x * grip_hover_outer_size, corner.y - def.InnerDir.y * grip_hover_outer_size };
			const ImVec2 max = { corner.x + def.InnerDir.x * grip_hover_outer_size, corner.y + def.InnerDir.y * grip_hover_outer_size };
			ImRect resize_rect(min, max);

			if (resize_rect.Min.x > resize_rect.Max.x) ImSwap(resize_rect.Min.x, resize_rect.Max.x);
			if (resize_rect.Min.y > resize_rect.Max.y) ImSwap(resize_rect.Min.y, resize_rect.Max.y);
			ImGuiID resize_grip_id = window->GetID(resize_grip_n); // == GetWindowResizeCornerID()
			ImGui::ButtonBehavior(resize_rect, resize_grip_id, &hovered, &held, ImGuiButtonFlags_FlattenChildren | ImGuiButtonFlags_NoNavFocus);
			//GetForegroundDrawList(window)->AddRect(resize_rect.Min, resize_rect.Max, IM_COL32(255, 255, 0, 255));
			if (hovered || held)
				g.MouseCursor = (resize_grip_n & 1) ? ImGuiMouseCursor_ResizeNESW : ImGuiMouseCursor_ResizeNWSE;

			if (held && g.IO.MouseDoubleClicked[0] && resize_grip_n == 0)
			{
				// Manual auto-fit when double-clicking
				size_target = CalcWindowSizeAfterConstraint(window, size_auto_fit);
				ret_auto_fit = true;
				ImGui::ClearActiveID();
			}
			else if (held)
			{
				// Resize from any of the four corners
				// We don't use an incremental MouseDelta but rather compute an absolute target size based on mouse position
				ImVec2 clamp_min = ImVec2(def.CornerPosN.x == 1.0f ? visibility_rect.Min.x : -FLT_MAX, def.CornerPosN.y == 1.0f ? visibility_rect.Min.y : -FLT_MAX);
				ImVec2 clamp_max = ImVec2(def.CornerPosN.x == 0.0f ? visibility_rect.Max.x : +FLT_MAX, def.CornerPosN.y == 0.0f ? visibility_rect.Max.y : +FLT_MAX);

				const float x = g.IO.MousePos.x - g.ActiveIdClickOffset.x + ImLerp(def.InnerDir.x * grip_hover_outer_size, def.InnerDir.x * -grip_hover_inner_size, def.CornerPosN.x);
				const float y = g.IO.MousePos.y - g.ActiveIdClickOffset.y + ImLerp(def.InnerDir.y * grip_hover_outer_size, def.InnerDir.y * -grip_hover_inner_size, def.CornerPosN.y);

				ImVec2 corner_target(x, y); // Corner of the window corresponding to our corner grip
				corner_target = ImClamp(corner_target, clamp_min, clamp_max);
				CalcResizePosSizeFromAnyCorner(window, corner_target, def.CornerPosN, &pos_target, &size_target);
			}

			// Only lower-left grip is visible before hovering/activating
			if (resize_grip_n == 0 || held || hovered)
				resize_grip_col[resize_grip_n] = ImGui::GetColorU32(held ? ImGuiCol_ResizeGripActive : hovered ? ImGuiCol_ResizeGripHovered : ImGuiCol_ResizeGrip);
		}
		for (int border_n = 0; border_n < resize_border_count; border_n++)
		{
			const ImGuiResizeBorderDef& def = resize_border_def[border_n];
			const ImGuiAxis axis = (border_n == ImGuiDir_Left || border_n == ImGuiDir_Right) ? ImGuiAxis_X : ImGuiAxis_Y;

			bool hovered, held;
			ImRect border_rect = GetResizeBorderRect(window, border_n, grip_hover_inner_size, WINDOWS_HOVER_PADDING);
			ImGuiID border_id = window->GetID(border_n + 4); // == GetWindowResizeBorderID()
			ImGui::ButtonBehavior(border_rect, border_id, &hovered, &held, ImGuiButtonFlags_FlattenChildren);
			//GetForegroundDrawLists(window)->AddRect(border_rect.Min, border_rect.Max, IM_COL32(255, 255, 0, 255));
			if ((hovered && g.HoveredIdTimer > WINDOWS_RESIZE_FROM_EDGES_FEEDBACK_TIMER) || held)
			{
				g.MouseCursor = (axis == ImGuiAxis_X) ? ImGuiMouseCursor_ResizeEW : ImGuiMouseCursor_ResizeNS;
				if (held)
					border_held = border_n;
			}
			if (held)
			{
				ImVec2 clamp_min(border_n == ImGuiDir_Right ? visibility_rect.Min.x : -FLT_MAX, border_n == ImGuiDir_Down ? visibility_rect.Min.y : -FLT_MAX);
				ImVec2 clamp_max(border_n == ImGuiDir_Left ? visibility_rect.Max.x : +FLT_MAX, border_n == ImGuiDir_Up ? visibility_rect.Max.y : +FLT_MAX);
				ImVec2 border_target = window->Pos;
				border_target[axis] = g.IO.MousePos[axis] - g.ActiveIdClickOffset[axis] + WINDOWS_HOVER_PADDING;
				border_target = ImClamp(border_target, clamp_min, clamp_max);
				CalcResizePosSizeFromAnyCorner(window, border_target, ImMin(def.SegmentN1, def.SegmentN2), &pos_target, &size_target);
			}
		}
		ImGui::PopID();

		bool changed = false;
		newSize = window->Size;
		newPosition = window->Pos;

		// Apply back modified position/size to window
		if (size_target.x != FLT_MAX)
		{
			//window->SizeFull = size_target;
			//MarkIniSettingsDirty(window);
			newSize = size_target;
			changed = true;
		}
		if (pos_target.x != FLT_MAX)
		{
			//window->Pos = ImFloor(pos_target);
			//MarkIniSettingsDirty(window);
			newPosition = pos_target;
			changed = true;
		}

		//window->Size = window->SizeFull;
		return changed;
	}

}
