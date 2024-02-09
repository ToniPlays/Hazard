
#include "PerformanceDebugPanel.h"
#include "Core/MessageFlags.h"
#include "Profiling/PerformanceProfiler.h"

#include "Hazard/ImGUI/UIElements/Table.h"

namespace UI
{
	using namespace Hazard;

	PerformanceDebugPanel::PerformanceDebugPanel() : Hazard::ImUI::Panel("PerformanceDebugPanel")
	{
		m_Open = false;
		m_SearchField.SetHint("Search...");
		m_SearchField.SetIcon((const char*)ICON_FK_SEARCH);
	}

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
			if (ImGui::BeginTabItem("Threading"))
			{
				DrawJobSystemView();
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
	}
	void PerformanceDebugPanel::DrawPerformanceTimers()
	{
		struct RowData
		{
			const char* Name;
			float Time;
			float MaxTime;
		};

		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		m_SearchField.Render();
		ImVec2 size = ImGui::GetContentRegionAvail();

		ImUI::Table<RowData> table("MemoryUsage", size);
		table.SetColumns({ "Category", "Time" });

		const auto timerMap = PerformanceProfiler::GetPerFrameData();
		table.Reserve(timerMap.size());
		table.RowHeight(24.0f);
		table.RowContent([style](uint32_t, const RowData& data) {
			ImUI::Separator({ 4.0, 24.0f }, data.Time < data.MaxTime ? style.Colors.AxisY : style.Colors.Warning);
			ImGui::SameLine();
			ImGui::Text("%s", data.Name);
			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%.4f ms", data.Time);
		});

		for (auto& [name, time] : timerMap)
			table.AddRow({ name, time.Time, time.MaxTime });

		PerformanceProfiler::Clear();
		table.Render();
	}
	void PerformanceDebugPanel::DrawMemoryView()
	{
	#ifdef HZR_MEM_DIAG
		const auto& allocStats = Memory::GetAllocationStats();
		m_SearchField.Render();

		ImGui::Text("Memory usage: %s", StringUtil::BytesToString(allocStats.TotalAllocated - allocStats.TotalFreed).c_str());
		ImGui::Text("Allocated: %s", StringUtil::BytesToString(allocStats.TotalAllocated).c_str());
		ImGui::Text("Freed: %s", StringUtil::BytesToString(allocStats.TotalFreed).c_str());

		ImVec2 size = ImGui::GetContentRegionAvail();

		struct MemoryItem
		{
			const char* Category;
			uint64_t Size;
		};

		const auto& allocStatsMap = Memory::Allocator::GetAllocationStats();

		std::vector<MemoryItem> sortedItems;
		sortedItems.reserve(allocStatsMap.size());
		for (auto& [category, stat] : allocStatsMap)
		{
			if (StringUtil::Contains(category, m_SearchField.GetValue()))
				sortedItems.push_back({ category, stat.TotalAllocated - stat.TotalFreed });
		}

		std::sort(sortedItems.begin(), sortedItems.end(), [](auto& a, auto& b) { return a.Size > b.Size; });

		ImUI::Table<MemoryItem> table("MemoryAlloc", size);
		table.SetColumns({ "Time", "Memory usage" });
		table.RowHeight(24.0f);
		table.Reserve(allocStatsMap.size());
		table.RowContent([this](uint32_t, const MemoryItem& item) {
			std::string title = std::string(item.Category);

			ImUI::Separator({ 4.0, 24.0f }, GetMemoryColor(item.Size));
			ImGui::SameLine();
			ImGui::Text("%s", title.substr(title.find_last_of('\\') + 1).c_str());
			ImGui::TableNextColumn();
			ImUI::ShiftX(4.0f);
			ImGui::Text("%s", StringUtil::BytesToString(item.Size).c_str());
		});

		for (auto& item : sortedItems)
			table.AddRow(item);

		table.Render();
	#else
		ImGui::Text("Memory debug not enabled");
	#endif
	}
	void PerformanceDebugPanel::DrawJobSystemView()
	{
		auto& threads = Application::Get().GetJobSystem().GetThreads();
		ImUI::Table<Ref<Thread>> table("Job status", ImGui::GetContentRegionAvail(), false);
		table.SetColumns({ "Thread", "Status", "Current job" });
		table.Reserve(threads.size());
		table.RowHeight(28.0f);
		table.RowContent([&](uint32_t, Ref<Thread> thread) {
			ImUI::Shift(4.0f, 4.0f);
			ImGui::Text("Thread %u", thread->GetThreadID());
			ImGui::TableNextColumn();

			ImUI::Shift(4.0f, 4.0f);
			ImGui::Text("%s", ThreadStatusToString(thread->GetStatus()).c_str());
			ImGui::TableNextColumn();

			Ref<Job> job = thread->GetCurrentJob();

			if (job)
			{
				ImUI::Shift(4.0f, 4.0f);
				ImGui::Text("%s", job->GetName().c_str());
			}
		});

		for (Ref<Thread> thread : threads)
			table.AddRow(thread);

		table.Render();
	}
	ImVec4 PerformanceDebugPanel::GetMemoryColor(uint64_t bytes)
	{
		const ImUI::Style& style = ImUI::StyleManager::GetCurrent();

		if (bytes < 1024 * 512)		return style.Colors.AxisY;
		if (bytes < 1048576 * 10)	return style.Colors.Warning;
		if (bytes < 1048576 * 50)	return style.Colors.AxisX;

		return style.Colors.AxisX;
	}
	std::string PerformanceDebugPanel::ThreadStatusToString(ThreadStatus status)
	{
		switch (status)
		{
			case ThreadStatus::None:		return "None";
			case ThreadStatus::Terminated:	return "Terminated";
			case ThreadStatus::Waiting:		return "Waiting";
			case ThreadStatus::Executing:	return "Executing";
		}
		return "";
	}
}
