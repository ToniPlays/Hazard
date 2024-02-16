#include "ProgressOverlay.h"

#include "Hazard/ImGUI/UILibrary.h"
#include "Hazard/Math/Time.h"

namespace UI
{
	ProgressOverlay::ProgressOverlay() : Overlay("Progress", { 400, 0 }, { 24, 32 }) 
	{
		using namespace Hazard;

		JobSystem& system = Hazard::Application::Get().GetJobSystem();
		system.Hook(JobSystem::Submit, [this](Ref<JobGraph> graph) {
			std::scoped_lock lock(m_JobMutex);
			m_JobGraphs.push_back({ Time::s_Time, 0.0, 0.0, 0.0, graph });
		});

		system.Hook(JobSystem::Failure, [this](Ref<JobGraph> graph) {
			std::scoped_lock lock(m_JobMutex);
			for (auto& graphs : m_JobGraphs)
			{
				if (graphs.Graph != graph) continue;
				graphs.TerminatedAt = Time::s_Time;
			}
		});

		system.Hook(JobSystem::Finished, [this](Ref<JobGraph> graph) {

			std::scoped_lock lock(m_JobMutex);
			for (auto& graphs : m_JobGraphs)
			{
				if (graphs.Graph != graph) continue;

				graphs.FinishedAt = Time::s_Time;
				graphs.RemoveAfter = graphs.FinishedAt + 2.5;
			}
		});
	}

	void ProgressOverlay::Update()
	{
		using namespace Hazard;
		m_Open = m_JobGraphs.size() > 0;
		if (!m_Open) return;

		double time = Time::s_Time;

		std::vector<JobGraphProgress> remaining;
		for (uint32_t i = 0; i < m_JobGraphs.size(); i++)
		{
			JobGraphProgress& progress = m_JobGraphs[i];
			if (progress.RemoveAfter >= time || progress.RemoveAfter == 0.0f)
				remaining.push_back(progress);
		}
		m_JobGraphs = remaining;
	}

	void ProgressOverlay::OnPanelRender()
	{
		std::scoped_lock lock(m_JobMutex);
		for (auto& info : m_JobGraphs)
		{
			Ref<JobGraph>& graph = info.Graph;
			DrawProgressCard(graph);
		}

		ImGui::Separator();
	}
	void ProgressOverlay::DrawProgressCard(Ref<JobGraph> graph)
	{
		const Hazard::ImUI::Style& style = Hazard::ImUI::StyleManager::GetCurrent();
		const std::string& currentJob = graph->GetCurrentStageInfo().Name;
		float progress = graph->GetProgress();

		std::string progressText = fmt::format("{0}/100", (uint32_t)(progress * 100.0f));

		float width = ImGui::CalcTextSize(progressText.c_str()).x;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		Hazard::ImUI::MenuHeader(graph->GetName().c_str(), width - 32);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::SetCursorPosX(panelWidth - width);
		ImGui::Text("%s", progressText.c_str());

		if (!currentJob.empty())
			ImGui::Text("%s", currentJob.c_str());

		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, graph->GetFlags() & JOB_GRAPH_FAILED ? style.Colors.AxisX : style.Colors.AxisZ);
		ImGui::ProgressBar(progress, { panelWidth, 32});
		ImGui::PopStyleColor();
	}
}
