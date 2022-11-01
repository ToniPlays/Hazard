
#include "AssetManagerDebugPanel.h"

namespace UI
{
	using namespace Hazard;
	void AssetManagerDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();
		const auto& allocStats = Memory::GetAllocationStats();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImUI::TextFieldWithHint(m_SearchValue, "Search...");

		ImVec2 size = ImGui::GetContentRegionAvail();
		const char* columns[] = { "Path", "Handle" };

		ImUI::Table("AssetPanel", columns, 2, size, [&]() {

			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
			float rowHeight = 24.0f;

			for (auto& [path, metadata] : AssetManager::GetMetadataRegistry())
			{
				if (!StringUtil::Contains(metadata.Path.string(), m_SearchValue)) continue;

				bool clicked = ImUI::TableRowClickable((const char*)path.c_str(), rowHeight);

				auto& n = path;
				auto& meta = metadata;

				ImUI::Group((const char*)&path, [&]() {
					ImUI::Separator({ 4.0, rowHeight }, meta.IsLoaded ? style.Colors.AxisY : style.Colors.AxisX);
					ImGui::SameLine();
					ImGui::Text(n.string().c_str());
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text(std::to_string(meta.Handle).c_str());
					});
			}
			});

	}
}