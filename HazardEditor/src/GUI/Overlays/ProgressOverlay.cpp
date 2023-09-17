#include "ProgressOverlay.h"

namespace UI
{
	ProgressOverlay::ProgressOverlay() : Overlay("Progress", { 300, 0 }, { 24, 32 })
	{
		Open();
	}
	void ProgressOverlay::Update()
	{
		JobSystem& system = Hazard::Application::Get().GetJobSystem();

		if (system.GetQueuedGraphs().size() > 0 || system.GetQueuedJobs().size() > 0 || system.GetRunningJobs().size() > 0)
		{
			Open();
			return;
		}
		Close();
	}

	void ProgressOverlay::OnPanelRender()
	{
		JobSystem& system = Hazard::Application::Get().GetJobSystem();
		for (auto& graph : system.GetQueuedGraphs())
			DrawProgressCard(graph->GetName().c_str(), graph->GetCurrentStage()->GetName(), graph->GetProgress());

		ImGui::Separator();
		auto jobs = system.GetRunningJobs();
		for (auto& job : jobs)
		{
			if (job->GetJobGraph()) continue;
			DrawProgressCard(job->GetName().c_str(), ",", job->GetProgress());
		}

		ImGui::Separator();

		for (auto& job : system.GetQueuedJobs())
			DrawProgressCard(job->GetName().c_str(), "", job->GetProgress());
	}
	void ProgressOverlay::DrawProgressCard(const std::string& graphName, const std::string& currentJob, float progress)
	{
		std::string progressText = fmt::format("{0}/{1}", (uint32_t)(progress * 100.0f), 100);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

		float width = ImGui::CalcTextSize(progressText.c_str()).x;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		ImGui::Text("%s", graphName.c_str());
		ImGui::SameLine();
		ImGui::SetCursorPosX(panelWidth - width);
		ImGui::Text("%s", progressText.c_str());
		ImGui::PopFont();

		if (!currentJob.empty())
			ImGui::Text("%s", currentJob.c_str());

		ImGui::ProgressBar(progress, { panelWidth, 24 });
	}
}
