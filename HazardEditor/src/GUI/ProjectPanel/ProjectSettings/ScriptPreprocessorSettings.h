#pragma once
#include "UtilityCore.h"

#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
	using namespace Hazard;
	static void DrawScriptDefineList(std::vector<std::string>* defines)
	{
		ImVec2 size = ImGui::GetContentRegionAvail();
		if (ImGui::Button("New", { 150, 24 }))
			defines->push_back("");

		ImUI::Table<uint32_t> table("Define table", { size.x, 0 }, false);
		table.SetColumns({ "Name" });
		table.RowHeight(28.0f);
		table.RowContent([defines](uint32_t index) mutable {
			ImUI::TextField defineName(defines->at(index));
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
			table.AddRow(i);

		table.Render();
	}
}