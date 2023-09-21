#pragma once

#include "UtilityCore.h"
#include "Buffer/Buffer.h"
#include "Color.h"
#include <imgui_internal.h>
#include <functional>

#include <Hazard/ImGUI/ScopedVar.h>
#include <Hazard/ImGUI/StyleManager.h>
#include <Hazard/ImGUI/UILibrary.h>

namespace Hazard::ImUI
{
	template<typename T>
	class Table
	{
	public:
		Table() = default;
		Table(const std::string& name, ImVec2 size) : m_Name(name), m_Size(size) {};
		~Table() {}
		
		void RowHeight(float height) { m_RowHeight = height; }
		void SetColumns(const std::vector<std::string>& columns) { m_Columns = columns; }

		void Reserve(uint64_t count) { m_Rows.reserve(count); }
		void AddRow(const T& row) { m_Rows.push_back(row); }

		void RowContent(const std::function<void(const T&)> content) { m_Content = content; }

		bool DidSelect() const { return m_DidClick != 0; }
		uint64_t SelectedRow() const { return m_DidClick - 1; }
		const T& SelectedValue() { return m_Rows[m_DidClick - 1]; }

		void Render()
		{
			if (m_Size.x <= 0.0f || m_Size.y <= 0.0f) return;

			float edgeOffset = 4.0f;

			ImVec4 bgColor = StyleManager::GetCurrent().BackgroundColor;
			const ImU32 colRowAlt = ColorWithMultiplier(bgColor, 1.2f);

			ScopedStyleVar cellPadding(ImGuiStyleVar_CellPadding, ImVec2(4.0f, 0.0f));
			ScopedColourStack rowColor(ImGuiCol_TableRowBg, bgColor, ImGuiCol_TableRowBgAlt, colRowAlt, ImGuiCol_ChildBg, bgColor);

			ImGuiTableFlags flags = ImGuiTableFlags_NoPadInnerX
				| ImGuiTableFlags_Resizable
				| ImGuiTableFlags_Reorderable
				| ImGuiTableFlags_ScrollY
				| ImGuiTableFlags_RowBg;

			if (!ImGui::BeginTable(m_Name.c_str(), m_Columns.size(), flags, m_Size))
				return;

			for (uint32_t i = 0; i < m_Columns.size(); i++)
				ImGui::TableSetupColumn(m_Columns[i].c_str());

			//Headers
			{
				const ImU32 activeColor = ColorWithMultiplier(bgColor, 1.3f);
				ScopedColourStack headerCol(ImGuiCol_HeaderHovered, activeColor, ImGuiCol_HeaderActive, activeColor);

				ImGui::TableSetupScrollFreeze(ImGui::TableGetColumnCount(), 1);
				ImGui::TableNextRow(ImGuiTableRowFlags_Headers, 22.0f);

				for (uint32_t i = 0; i < m_Columns.size(); i++)
				{
					ImGui::TableSetColumnIndex(i);
					const char* columnName = ImGui::TableGetColumnName(i);
					ImGui::PushID(columnName);
					Shift(edgeOffset * 3.0f, edgeOffset * 2.0f);
					ImGui::TableHeader(columnName);
					Shift(-edgeOffset * 3.0f, -edgeOffset * 2.0f);
					ImGui::PopID();
				}

				ImGui::SetCursorPosX(ImGui::GetCurrentTable()->OuterRect.Min.x);
				Underline(true, 0.0f, 5.0f);
			}

			for (uint64_t i = 0; i < m_Rows.size(); i++)
			{
				ImGui::PushID(i);
				if (ImUI::TableRowClickable(i, m_RowHeight))
					m_DidClick = i + 1;
				m_Content(m_Rows[i]);
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

	private:
		std::string m_Name;
		ImVec2 m_Size;
		float m_RowHeight;
		uint32_t m_DidClick = 0;
		std::vector<std::string> m_Columns;
		std::vector<T> m_Rows;
		std::function<void(const T&)> m_Content;
	};
}