
#include "PerformanceDebugPanel.h"
#include "Core/MessageFlags.h"

namespace UI
{
	using namespace Hazard;
	void PerformanceDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		if (ImGui::BeginTabBar("##perf"))
		{
			if (ImGui::BeginTabItem("Memory"))
			{
				DrawMemoryView();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	void PerformanceDebugPanel::DrawMemoryView()
	{
#ifdef HZR_MEM_DIAG

		const auto& allocStats = Memory::GetAllocationStats();

		ImGui::Text("Memory usage: %s", StringUtil::BytesToString(allocStats.TotalAllocated - allocStats.TotalFreed).c_str());

		ImVec2 size = ImGui::GetContentRegionAvail();
		const char* columns[] = { "Category", "Allocated" };

		ImUI::Table("Memory usage", columns, 2, size, [&]() {

			float rowHeight = 24.0f;

			
			const auto& allocStatsMap = Memory::Allocator::GetAllocationStats();

			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

			for (auto& [category, stat] : allocStatsMap)
			{
				bool clicked = ImUI::TableRowClickable((const char*)category, rowHeight);

				const auto& c = category;

				size_t TotalUsed = stat.TotalAllocated - stat.TotalFreed;

				ImUI::Group((const char*)&category, [c, rowHeight, style, TotalUsed]() {

					std::string title = std::string(c);

					ImUI::Separator({ 4.0, rowHeight }, style.Colors.AxisY);
					ImGui::SameLine();
					ImGui::Text(title.substr(title.find_last_of('\\') + 1).c_str());
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text(StringUtil::BytesToString(TotalUsed).c_str());
					});
			}
			});
#else
		ImGui::Text("Memory debug not enabled");
#endif
	}
}