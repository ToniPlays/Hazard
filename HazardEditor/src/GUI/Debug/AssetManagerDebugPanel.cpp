
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
		const char* columns[] = { "Path", "Type", "Pack handle", "Handle" };

		ImUI::Table("AssetPanel", columns, 4, size, [&]() {
			float rowHeight = 24.0f;

			auto& registry = AssetManager::GetMetadataRegistry();

			for (auto& [key, metadata] : registry)
			{
				if (!StringUtil::Contains(metadata.Key, m_SearchValue)) continue;

				ImUI::TableRowClickable((const char*)key.c_str(), rowHeight);

				auto& n = key;
				auto& meta = metadata;

				ImUI::Group((const char*)&key, [&]() {

					ImUI::Separator({ 4.0, rowHeight }, GetLoadStateColor(meta.LoadState));
					ImGui::SameLine();
					ImGui::Text("%s", n.string().c_str());
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text("%s", Hazard::Utils::AssetTypeToString(meta.Type));
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text("%s", std::to_string(meta.AssetPackHandle).c_str());
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
