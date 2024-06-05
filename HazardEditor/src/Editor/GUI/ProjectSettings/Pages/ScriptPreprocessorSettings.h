#pragma once
#include "UtilityCore.h"

#include "Hazard/ImGUI/UIElements/Table.h"
#include <Editor/EditorScriptManager.h>

namespace UI
{
	using namespace Hazard;
	static void DrawScriptDefineList(std::vector<std::string>* defines)
	{
		ImVec2 size = ImGui::GetContentRegionAvail();
		ImGui::SetCursorPosX(size.x - 154);
		if (ImGui::Button("New", { 150, 32 }))
			defines->push_back("");

		ImUI::Table<std::string> table("Define table", { size.x, 0 }, false);
		table.SetColumns({ "Name" });
		table.RowHeight(28.0f);
		table.RowContent([defines](uint32_t index, std::string value) mutable {
			ImUI::TextField defineName(value);
			defineName.SetHint("Define name");
			defineName.Render();
			defines->at(index) = defineName.GetValue();

			if (defineName.DidClear())
			{
				Application::Get().SubmitMainThread([defines, index]() mutable {
					defines->erase(defines->begin() + index);
				});
			}
		});

		for (uint32_t i = 0; i < defines->size(); i++)
			table.AddRow(defines->at(i));

		table.Render();
	}
	static void DrawScriptPredefinedList(const std::vector<Editor::ScriptPreprocessor>& defines)
	{
		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::Table<Editor::ScriptPreprocessor> table("Predefined", { size.x, 0 }, false);
		table.SetColumns({ "Name", "Availability" });
		table.RowHeight(18.0f);
		table.RowContent([](uint32_t, Editor::ScriptPreprocessor define) mutable {
			ImUI::Shift(4.0f, 4.0f);
			ImGui::Text("%s", define.Name.c_str());
			ImGui::TableNextColumn();

			ImUI::Shift(4.0f, 4.0f);
			ImGui::Text("%s", define.Availability.c_str());
			ImGui::TableNextColumn();
		});

		for (auto& define : defines)
			table.AddRow(define);

		table.Render();
	}
}
