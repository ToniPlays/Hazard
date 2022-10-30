
#include "PerformanceDebugPanel.h"
#include "Core/MessageFlags.h"
#include "Profiling/PerformanceProfiler.h"

namespace UI
{
	using namespace Hazard;
	void PerformanceDebugPanel::OnPanelRender()
	{
		HZR_PROFILE_FUNCTION();

		if (ImGui::BeginTabBar("##perf"))
		{
			if (ImGui::BeginTabItem("Function timers"))
			{
				DrawPerformanceTimers();
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Memory"))
			{
				DrawMemoryView();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	void PerformanceDebugPanel::DrawPerformanceTimers()
	{
		const auto& allocStats = Memory::GetAllocationStats();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImUI::TextFieldWithHint(m_MemorySearchVal, "Search...");

		ImVec2 size = ImGui::GetContentRegionAvail();
		const char* columns[] = { "Category", "Time" };

		ImUI::Table("MemoryUsage", columns, 2, size, [&]() {

			float rowHeight = 24.0f;

			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();
			const auto timerMap = PerformanceProfiler::GetPerFrameData();
			PerformanceProfiler::Clear();

			for (auto& [name, time] : timerMap)
			{
				bool clicked = ImUI::TableRowClickable((const char*)name, rowHeight);

				auto& n = name;
				auto& t = time;

				ImUI::Group((const char*)&name, [&]() {
					ImUI::Separator({ 4.0, rowHeight }, style.Colors.AxisY);
					ImGui::SameLine();
					ImGui::Text(n);
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text("%.6f ms", t);
					});
			}
			});

	}
	void PerformanceDebugPanel::DrawMemoryView()
	{
#ifdef HZR_MEM_DIAG
		const auto& allocStats = Memory::GetAllocationStats();
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvailWidth());
		ImUI::TextFieldWithHint(m_MemorySearchVal, "Search...");

		ImGui::Text("Memory usage: %s", StringUtil::BytesToString(allocStats.TotalAllocated - allocStats.TotalFreed).c_str());
		ImGui::Text("Allocated: %s", StringUtil::BytesToString(allocStats.TotalAllocated).c_str());
		ImGui::Text("Freed: %s", StringUtil::BytesToString(allocStats.TotalFreed).c_str());

		ImVec2 size = ImGui::GetContentRegionAvail();
		const char* columns[] = { "Time", "Function" };

		ImUI::Table("MemoryAlloc", columns, 2, size, [&]() {

			float rowHeight = 24.0f;

			const auto& allocStatsMap = Memory::Allocator::GetAllocationStats();
			const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

			struct MemoryItem
			{
				const char* Category;
				size_t Size;
			};

			std::vector<MemoryItem> sortedItems;
			sortedItems.reserve(allocStatsMap.size());
			for (auto& [category, stat] : allocStatsMap)
			{
				if (StringUtil::Contains(category, m_MemorySearchVal))
					sortedItems.push_back({ category, stat.TotalAllocated - stat.TotalFreed });
			}

			std::sort(sortedItems.begin(), sortedItems.end(), [](auto& a, auto& b) { return a.Size > b.Size; });

			for (auto& item : sortedItems)
			{
				bool clicked = ImUI::TableRowClickable((const char*)item.Category, rowHeight);

				ImUI::Group((const char*)&item.Category, [&]() {

					std::string title = std::string(item.Category);

					ImUI::Separator({ 4.0, rowHeight }, GetMemoryColor(item.Size));
					ImGui::SameLine();
					ImGui::Text(title.substr(title.find_last_of('\\') + 1).c_str());
					ImGui::TableNextColumn();
					ImUI::ShiftX(4.0f);
					ImGui::Text(StringUtil::BytesToString(item.Size).c_str());
					});
			}
			});
#else
		ImGui::Text("Memory debug not enabled");
#endif
	}
	ImVec4 PerformanceDebugPanel::GetMemoryColor(size_t bytes)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		if (bytes < 1024 * 512)		return style.Colors.AxisY;
		if (bytes < 1048576 * 10)	return style.Colors.Warning;
		if (bytes < 1048576 * 50)	return style.Colors.AxisX;

		return style.Colors.AxisX;
	}
}