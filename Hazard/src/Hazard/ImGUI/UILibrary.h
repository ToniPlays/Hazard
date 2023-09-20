#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "ScopedVar.h"
#include "StyleManager.h"
#include "ImGuiUtils.h"
#include "Hazard/Core/Core.h"
#include "HazardRenderer.h"
#include "Hazard/RenderContext/Texture2D.h"
#include "Hazard/Core/Application.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

#include "UIElements/InputFloat.h"

#include "../ImGui_Backend/imgui_impl_vulkan.h"
#include <Hazard/RenderContext/RenderContextManager.h>

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
	static inline void FocusCurrentWindow()
	{
		ImGui::FocusWindow(ImGui::GetCurrentWindow());
	}
	static inline bool NavigatedTo()
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
		if (strcmp(text, "") == 0 || !ImGui::IsItemHovered()) return false;

		ScopedStyleStack padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f), ImGuiStyleVar_WindowRounding, 4.0f);
		ImGui::BeginTooltip();
		ImGui::Text("%s", text);
		ImGui::EndTooltip();
		return true;
	}

#pragma endregion
#pragma region Input

	static bool TextField_OLD(std::string& text, const char* id = "##InputField", bool isMixed = false)
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());
		bool changed = false;
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);

		if (ImGui::InputText(id, buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			text = buffer;
			changed |= true;
		}
		ImGui::PopItemFlag();
		return changed;
	}

	static bool TextArea(std::string& text, int minLines, int maxLines, bool isMixed = false)
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		bool changed = false;

		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);

		if (ImGui::InputTextMultiline("##InputField", buffer, sizeof(buffer), { 0, ImGui::GetTextLineHeight() * minLines }))
		{
			text = buffer;
			changed |= true;
		}
		ImGui::PopItemFlag();
		return changed;
	}

	static bool TextFieldWithHint(std::string& text, const char* hint, bool isMixed = false)
	{
		char buffer[512] = { 0 };
		strcpy(buffer, text.c_str());

		bool changed = false;

		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));

		if (ImGui::InputTextWithHint("##InputField", hint, buffer, sizeof(buffer)))
		{
			text = buffer;
			changed |= true;
		}
		ImGui::PopStyleVar();
		ImGui::PopItemFlag();

		return changed;
	}
	static bool InputInt(int& value, int clearValue, int min, int max, bool isMixed = false)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		bool changed = ImGui::DragInt("##int", &value, 1, min, max);
		ImGui::PopItemFlag();
		return changed;
	}
	static bool InputUInt(uint64_t& value, uint64_t clearValue = 0.0f, uint64_t min = 0, uint64_t max = 0, bool isMixed = false)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		bool changed = ImGui::DragInt("##uint", (int*)&value, 0.5f, min, max);
		ImGui::PopItemFlag();
		return changed;
	}

	static bool SliderFloat(float& value, float clearValue = 0.0f, float min = 0.0f, float max = 0.0f, bool isMixed = false)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		bool changed = ImGui::SliderFloat("#sliderFloat", &value, min, max);
		ImGui::PopItemFlag();
		return changed;
	}
	static bool InputDouble(double& value, double clearValue = 0.0, bool isMixed = false)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		bool changed = ImGui::InputDouble("##double", &value, 0.5);
		ImGui::PopItemFlag();
		return changed;
	}

	static bool InputInt(int& value, int clearValue = 0.0, bool isMixed = false)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		bool changed = ImGui::InputInt("##int", &value, 1);
		ImGui::PopItemFlag();
		return changed;
	}

	static bool SliderFloat(const char* name, float& value, float clearValue = 0.0f, float min = 0.0, float max = 0.0, bool isMixed = false)
	{
		bool modified = false;
		ImGui::Text("%s", name);
		ImGui::NextColumn();
		Group(name, [&]() {
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			modified = SliderFloat(value, clearValue, min, max);
		});
		ImGui::NextColumn();
		return modified;
	}
	static bool Checkbox(bool& value, bool isMixed = false)
	{
		return ImGui::Checkbox("##val", &value);
	}
	static bool Checkbox(const char* name, bool& value, bool isMixed = false)
	{
		bool modified = false;
		ImGui::Text("%s", name);
		ImGui::NextColumn();
		Group(name, [&]() {
			ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
			modified = Checkbox(value);
		});
		ImGui::NextColumn();
		return modified;
	}
	static bool ColorPicker(const char* id, Color& color, bool isMixed = false)
	{

		bool modified = false;
		ImVec4 col = { color.r, color.g, color.b, color.a };
		ImGui::PushItemFlag(ImGuiItemFlags_MixedValue, isMixed);
		modified |= ImGui::ColorEdit4(id, &color.r);
		ImGui::PopItemFlag();

		return modified;
	}
	static bool ColorPicker(const char* name, const char* id, Color& color, bool isMixed = false)
	{
		ShiftY(4.0f);
		ImGui::Text("%s", name);
		ImGui::NextColumn();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		bool modified = ColorPicker(id, color, isMixed);
		ImGui::NextColumn();

		return modified;
	}

#pragma endregion
#pragma region Buttons

	static bool ColoredButton(const char* label, ImVec4 backgroundColor, ImVec4 foregroundColor, ImVec2 buttonSize = { 16, 16 })
	{
		ScopedStyleColor textColor(ImGuiCol_Text, foregroundColor);
		ScopedStyleColor buttonColor(ImGuiCol_Button, backgroundColor);

		return ImGui::Button(label, buttonSize);
	}

#pragma endregion
#pragma region Images

	static ImTextureID GetImageID(Ref<HazardRenderer::Image2D> image, Ref<HazardRenderer::Sampler> sampler)
	{
		using namespace HazardRenderer;
		static std::unordered_map<Image2D*, ImTextureID> cache;

		if (cache.find(image.Raw()) != cache.end())
		{
			if (cache[image.Raw()] != nullptr)
				return cache[image.Raw()];
		}

		switch (GraphicsContext::GetRenderAPI())
		{
		#ifdef HZR_INCLUDE_OPENGL
			case RenderAPI::OpenGL:
				return (ImTextureID)image.As<OpenGL::OpenGLImage2D>()->GetID();
			#endif
			#ifdef HZR_INCLUDE_VULKAN
			case RenderAPI::Vulkan:
			{
				Ref<Vulkan::VulkanImage2D> vkImage = image.As<Vulkan::VulkanImage2D>();
				const VkDescriptorImageInfo& imageInfo = vkImage->GetImageDescriptor();
				const Ref<Vulkan::VulkanSampler>& vkSampler = sampler.As<Vulkan::VulkanSampler>();

				if (!vkImage->IsValid() && vkSampler->GetVulkanSampler())
				{
					auto& white = Application::GetModule<RenderContextManager>().GetDefaultResources().WhiteTexture;
					const VkDescriptorImageInfo& whiteImageDesc = white.As<Vulkan::VulkanImage2D>()->GetImageDescriptor();
					cache[white.Raw()] = ImGui_ImplVulkan_AddTexture(vkSampler->GetVulkanSampler(), whiteImageDesc.imageView, whiteImageDesc.imageLayout);
					return cache[white.Raw()];
				}

				ImTextureID id = ImGui_ImplVulkan_AddTexture(vkSampler->GetVulkanSampler(), imageInfo.imageView, imageInfo.imageLayout);
				cache[image.Raw()] = id;
				return id;
			}
		#endif
		#ifdef HZR_INCLUDE_METAL
			case RenderAPI::Metal:
			{
				cache[image.Raw()] = image.As<Metal::MetalImage2D>()->GetMetalTexture();
				return cache[image.Raw()];
			}
		#endif
			default:
				HZR_ASSERT(false, "Undefined ImageID");
				break;
		}

		return 0;
	}
	static void Image(Ref<HazardRenderer::Image2D> image, Ref<HazardRenderer::Sampler> sampler, ImVec2 size, ImVec2 t0 = { 0, 1 }, ImVec2 t1 = { 1, 0 })
	{
		ImGui::Image(GetImageID(image, sampler), size, t0, t1);
	}
	static bool TextureSlot(std::string& text, Ref<Hazard::Texture2DAsset> texture)
	{
		constexpr float size = 32.0f;

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - size - 5.0f);
		{
			ImUI::TextField_OLD(text);
		}
		ImGui::SameLine(0, 5);

		if (texture)
			Image(texture->GetSourceImageAsset()->Value.As<HazardRenderer::Image2D>(), texture->GetSampler(), { size, size });
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
	static bool TextureSlot(const char* name, std::string& text, Ref<Texture2DAsset> texture, T callback)
	{
		bool modified = false;
		ImUI::ShiftY(8.0f);

		ImGui::Text("%s", name);
		ImGui::NextColumn();
		Group(name, [&]() {
			modified = TextureSlot(text, texture);
			callback();
		});
		ImGui::NextColumn();
		return modified;
	}
#pragma endregion
#pragma region Treenodes

	template<typename T, typename Prop>
	static bool TreenodeWithOptions(const char* title, ImGuiTreeNodeFlags flags, T callback, Prop propCallback = nullptr)
	{
		return TreenodeWithOptions(title, flags, callback, propCallback, []() {});
	};

	template<typename T, typename Prop, typename DragSource>
	static bool TreenodeWithOptions(const char* title, ImGuiTreeNodeFlags flags, T callback, Prop propCallback = nullptr, DragSource dragSource = nullptr)
	{
		bool treeOpen = false;
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

			if (ImGui::Button((const char*)ICON_FK_LIST_UL, ImVec2{ lineHeight, lineHeight }))
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

		if (treeOpen)
		{
			callback();
			ImGui::TreePop();
		}

		return false;
	}

#pragma endregion
#pragma region Table

	template<typename T>
	static void Table(const char* tableName, const std::vector<std::string>& headers, ImVec2 size, T callback)
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

		if (!ImGui::BeginTable(tableName, headers.size(), flags, size))
			return;

		//Setup
		for (uint32_t i = 0; i < headers.size(); i++)
			ImGui::TableSetupColumn(headers[i].c_str());

		//Headers
		{
			const ImU32 activeColor = ColorWithMultiplier(bgColor, 1.3f);
			ScopedColourStack headerCol(ImGuiCol_HeaderHovered, activeColor, ImGuiCol_HeaderActive, activeColor);

			ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);
			ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);

			for (uint32_t i = 0; i < headers.size(); i++)
			{
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
	static bool TableRowTreeItem(const char* id, const char* text, bool selected, T callback)
	{
		bool clicked = false;
		//constexpr float edgeOffset = 4.0f;
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

		auto fillRowWithColour = [](const ImColor& colour) {
			for (int column = 0; column < ImGui::TableGetColumnCount(); column++)
				ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, colour, column);
		};

		ImVec4 bgCol = StyleManager::GetCurrent().BackgroundColor;
		ImVec4 textCol = StyleManager::GetCurrent().Window.Text;

		if (selected)
		{
			if (isWindowFocused || NavigatedTo())
			{
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
		//const ImVec2 label_size = ImGui::CalcTextSize(text, nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float text_offset_x = g.FontSize + padding.x * 2;           // Collapser arrow width + Spacing
		const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
		//const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
		ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
		//const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
		//const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		//const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

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
	static bool ContextMenu(T callback)
	{
		ScopedStyleStack padding(ImGuiStyleVar_WindowPadding, ImVec2(4.0f, 4.0f), ImGuiStyleVar_PopupRounding, 4.0f, ImGuiStyleVar_ItemSpacing, ImVec2(16.0f, 4.0f), ImGuiStyleVar_ChildBorderSize, 0);
		if (ImGui::BeginPopupContextWindow(0, ImGuiPopupFlags_MouseButtonRight))
		{
			callback();
			ImGui::EndPopup();
			return true;
		}
		return false;
	}
	static bool Submenu(const char* label, std::function<void()> callback)
	{
		if (!callback) return false;

		if (ImGui::BeginMenu(label))
		{
			callback();
			ImGui::EndMenu();
			return true;
		}
		return false;
	}
	static void MenuItem(const char* label, const std::function<void()> callback)
	{
		if (!callback) return;

		if (ImGui::MenuItem(label))
			callback();
	}
	static void Separator(ImVec2 size, ImVec4 color);

	static void MenuHeader(const char* label)
	{
		ImGui::PushID(label);
		const Style& style = StyleManager::GetCurrent();
		ImGui::TextColored(style.Window.TextDark, "%s", label);
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
		ImGui::BeginChild("#sep", size);
		ImGui::EndChild();
		ImGui::PopStyleColor();
	}

#pragma endregion
#pragma region DragDrop

	template<typename T, typename Callback>
	static bool DragSource(const char* type, T* data, Callback callback)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
		{
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
		return DragSource(Utils::AssetTypeToString(type), data, callback);
	}

	template<typename T, typename Callback>
	static bool DropTarget(const char* type, Callback callback)
	{
		bool accepted = false;
		if (ImGui::BeginDragDropTarget())
		{

			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(type);
			if (payload)
			{
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
			if (payload)
			{
				callback(*(T*)payload->Data);
				accepted = true;
			}

			ImGui::EndDragDropTarget();
		}
		return accepted;
	}
#pragma endregion
}
