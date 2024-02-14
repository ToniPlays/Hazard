
#include "AssetManagerDebugPanel.h"
#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
	using namespace Hazard;

	void AssetManagerDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImUI::TextFieldWithHint(m_SearchValue, "Search...");

		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::Table<AssetMetadata> table("AssetPanel", size);
		table.SetColumns({ "Path", "Source file", "Handle", "Type" });
		table.Reserve(AssetManager::GetMetadataRegistry().size());
		table.RowHeight(24.0f);
		table.RowContent([&](uint32_t, const AssetMetadata& metadata) {
			ImUI::Separator({ 4.0, 24.0f }, GetLoadStateColor(metadata.LoadState));
			ImGui::SameLine();
			ImGui::Text("%s", metadata.FilePath.string().c_str());
			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", metadata.SourceFile.string().c_str());

			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", std::to_string(metadata.Handle).c_str());

			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", Hazard::Utils::AssetTypeToString(metadata.Type));
		});

		for (auto& [key, metadata] : AssetManager::GetMetadataRegistry())
		{
			if (!StringUtil::Contains(metadata.FilePath.string(), m_SearchValue) && !StringUtil::Contains(std::to_string(metadata.Handle), m_SearchValue)) continue;

			table.AddRow(metadata);
		}
		table.Render();
	}
	ImColor AssetManagerDebugPanel::GetLoadStateColor(const LoadState& state)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
		switch (state)
		{
			case LoadState::None: return style.Colors.AxisX;
			case LoadState::Loading: return style.Colors.Warning;
			case LoadState::Loaded: return style.Colors.AxisY;
		}
		return style.Colors.AxisY;
	}
}
