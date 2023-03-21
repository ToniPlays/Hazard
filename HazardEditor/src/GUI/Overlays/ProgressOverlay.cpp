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
			DrawProgressCard(graph->GetName().c_str(), graph->GetProgress());

		ImGui::Separator();
		for (auto& job : system.GetRunningJobs())
			DrawProgressCard(job->GetName().c_str(), job->GetProgress());

		ImGui::Separator();

		for (auto& job : system.GetQueuedJobs())
			DrawProgressCard(job->GetName().c_str(), job->GetProgress());
	}
	void ProgressOverlay::DrawProgressCard(const char* title, float progress)
	{
		std::string progressText = fmt::format("{0}/{1}", (uint32_t)(progress * 100.0f), 100);
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);

		float width = ImGui::CalcTextSize(progressText.c_str()).x;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		ImGui::Text("%s", title);
		ImGui::SameLine();
		ImGui::SetCursorPosX(panelWidth - width);
		ImGui::Text("%s", progressText.c_str());
		ImGui::PopFont();

		ImGui::ProgressBar(progress, { panelWidth, 24 });
	}
}
