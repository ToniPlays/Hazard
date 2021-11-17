#pragma once


#include "Hierarchy.h"
#include "GUI/EditorView.h"

#include "Library/Input.h"
#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Layout/ContextMenus.h"
#include "GUI/Window/Rendering/WorldEnvironmentData.h"
#include "GUI/Window/DragDropUtils.h"
#include "Library/Layout/Dockspace.h"

using namespace WindowLayout;

namespace WindowElement {

	Hierarchy::Hierarchy() : EditorWindow(ICON_FK_ALIGN_LEFT " Hierarchy") {}

	bool Hierarchy::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Events::SelectionContextChange>(BIND_EVENT(Hierarchy::SelectionContextChange));
		return dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT(Hierarchy::KeyPressEvent));
	}
	void Hierarchy::OnWindowRender()
	{
		Ref<World> world = ECS::WorldCommand::GetCurrentWorld();
		if (!world) return;

		const float edgeOffset = 4.0f;
		Layout::ShiftX(edgeOffset * 3.0f);
		Layout::ShiftY(edgeOffset * 2.0f);

		Layout::ItemWidth(ImGui::GetContentRegionAvail().x - edgeOffset * 3.0f);
		Input::InputField(m_SearchValue, ICON_FK_SEARCH "Search..");

		ScopedStyle cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));
		ImVec4 bgColor = Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Background));
		const ImU32 colRowAlt = Style::ColorWithMultiplier(bgColor, 1.2f);
		ScopedColour tableBG(ImGuiCol_TableRowBgAlt, colRowAlt);

		//Entity table
		{
			ScopedColour tableBG(ImGuiCol_ChildBg, bgColor);

			ImGuiTableFlags tableFlags = ImGuiTableFlags_NoPadInnerX
				| ImGuiTableFlags_Resizable
				| ImGuiTableFlags_Reorderable
				| ImGuiTableFlags_ScrollY;
			constexpr int columns = 3;

			ImGui::BeginTable("EntityTable", columns, tableFlags, ImGui::GetContentRegionAvail());

			ImGui::TableSetupColumn("Entity");
			ImGui::TableSetupColumn("Type");
			ImGui::TableSetupColumn("Modifiers");

			//Header
			{
				const ImU32 activeColor = Style::ColorWithMultiplier(bgColor, 1.3f);
				ScopedColourStack headerCol(ImGuiCol_HeaderHovered, activeColor, ImGuiCol_HeaderActive, activeColor);

				ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);
				ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);

				for (uint32_t i = 0; i < columns; i++) {
					ImGui::TableSetColumnIndex(i);
					const char* columnName = ImGui::TableGetColumnName(i);
					Layout::IDGroup(columnName, [&]() {
						Layout::Shift(edgeOffset * 3.0f, edgeOffset * 2.0f);
						ImGui::TableHeader(columnName);
						Layout::Shift(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
						});
				}
				ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
				Layout::Underline(true, 0.0f, 5.0f);
			}
			//Entity list
			{
				for (auto entity : world->GetWorldRegistry().view<TagComponent>()) {
					Entity e(entity, world.Raw());
					DrawEntity(e);
				}
			}

			ImGui::EndTable();
		}

	}
	bool Hierarchy::SelectionContextChange(Events::SelectionContextChange& e)
	{
		m_SelectionContext = e.GetEntity();
		return false;
	}
	void Hierarchy::DrawEntity(Entity entity)
	{
		TagComponent c = entity.GetTag();

		if (!Hazard::StringUtil::IsMatching(c.m_Tag, m_SearchValue))
			return;

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

		const bool selected = entity == m_SelectionContext;
		ImGuiTreeNodeFlags flags = (selected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		const std::string entityIDStr = c.m_Tag + std::to_string(entity);
		ImGui::PushClipRect(rowAreaMin, rowAreaMax, false);

		bool isRowHovered, held;// = ImGui::ItemHoverable(ImRect(rowAreaMin, rowAreaMax), (uint64_t)(uint32_t)entity);
		bool isRowClicked = ImGui::ButtonBehavior(ImRect(rowAreaMin, rowAreaMax), ImGui::GetID(entityIDStr.c_str()),
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

		ImVec4 bgCol = Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Background));
		ImVec4 textCol = Style::ColorAsImVec4(Style::GetStyleColor(ColorType::Text));

		if (selected)
		{
			if (isWindowFocused || Layout::NavigatedTo()) {
				fillRowWithColour(Style::ColorWithMultiplier(bgCol, 1.2f));
			}
			else
			{
				const ImColor col = Style::ColorWithMultiplier(bgCol, 0.9f);
				fillRowWithColour(col);
			}
		}

		if (selected)
			ImGui::PushStyleColor(ImGuiCol_Text, textCol);

		ImGuiContext& g = *GImGui;
		auto& style = ImGui::GetStyle();
		const ImVec2 label_size = ImGui::CalcTextSize(entityIDStr.c_str(), nullptr, false);
		const ImVec2 padding = ((flags & ImGuiTreeNodeFlags_FramePadding)) ? style.FramePadding : ImVec2(style.FramePadding.x, ImMin(window->DC.CurrLineTextBaseOffset, style.FramePadding.y));
		const float text_offset_x = g.FontSize + padding.x * 2;           // Collapser arrow width + Spacing
		const float text_offset_y = ImMax(padding.y, window->DC.CurrLineTextBaseOffset);                    // Latch before ItemSize changes it
		const float text_width = g.FontSize + (label_size.x > 0.0f ? label_size.x + padding.x * 2 : 0.0f);  // Include collapser
		ImVec2 text_pos(window->DC.CursorPos.x + text_offset_x, window->DC.CursorPos.y + text_offset_y);
		const float arrow_hit_x1 = (text_pos.x - text_offset_x) - style.TouchExtraPadding.x;
		const float arrow_hit_x2 = (text_pos.x - text_offset_x) + (g.FontSize + padding.x * 2.0f) + style.TouchExtraPadding.x;
		const bool is_mouse_x_over_arrow = (g.IO.MousePos.x >= arrow_hit_x1 && g.IO.MousePos.x < arrow_hit_x2);

		bool previousState = ImGui::TreeNodeBehaviorIsOpen(ImGui::GetID(entityIDStr.c_str()));

		const bool opened = ImGui::TreeNodeWithIcon(nullptr, ImGui::GetID(entityIDStr.c_str()), flags, c.m_Tag.c_str(), nullptr);

		if (isRowClicked)
		{
			ImGui::FocusWindow(ImGui::GetCurrentWindow());
			Events::SelectionContextChange e(entity);
			EditorView::GetInstance().OnEvent(e);
		}

		if (opened) ImGui::TreePop();

		if (selected)
			ImGui::PopStyleColor();

		//Type
		ImGui::TableNextColumn();

		Layout::ShiftX(edgeOffset);
		ImGui::Text("Entity");

		//Modifiers
		ImGui::TableNextColumn();

		ScopedStyle frame(ImGuiStyleVar_FrameBorderSize, 0);
		Color col = Style::GetStyleColor(c.m_Visible ? ColorType::Debug : ColorType::Secondary);
		ScopedColour textColor(ImGuiCol_Text, Style::ColorAsImVec4(col));

		Layout::ShiftX(edgeOffset);
		ImGui::Text(ICON_FK_EYE);
	}
	bool Hierarchy::KeyPressEvent(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == Key::Delete && m_SelectionContext) {
			m_SelectionContext.GetWorld().DestroyEntity(m_SelectionContext);
			return true;
		}
		if (e.GetKeyCode() == Key::D && m_SelectionContext && Hazard::Input::IsKeyDown(Key::LeftControl)) {
			m_SelectionContext.GetWorld().CreateEntity(m_SelectionContext);
			return true;
		}
		return false;
	}
}