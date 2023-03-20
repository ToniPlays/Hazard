
#include "AssetManagerDebugPanel.h"

namespace UI
{
	using namespace Hazard;

	void AssetManagerDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImUI::TextFieldWithHint(m_SearchValue, "Search...");

		ImVec2 size = ImGui::GetContentRegionAvail();
		const char* columns[] = { "Path", "Handle" };

		ImUI::Table("AssetPanel", columns, 2, size, [&]() {

			
			float rowHeight = 24.0f;

			for (auto& [path, metadata] : AssetManager::GetMetadataRegistry())
			{
				if (!StringUtil::Contains(metadata.Key, m_SearchValue)) continue;

				ImUI::TableRowClickable((const char*)path.c_str(), rowHeight);

				auto& n = path;
				auto& meta = metadata;

				ImUI::Group((const char*)&path, [&]() {

					ImUI::Separator({ 4.0, rowHeight }, GetLoadStateColor(meta.LoadState));
					ImGui::SameLine();
					ImGui::Text("%s", n.c_str());
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text("%s", std::to_string(meta.Handle).c_str());
					});
			}
			});

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
