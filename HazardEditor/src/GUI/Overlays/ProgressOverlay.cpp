#include "ProgressOverlay.h"

#include "Hazard/ImGUI/UILibrary.h"

namespace UI
{
	ProgressOverlay::ProgressOverlay() : Overlay("Progress", { 400, 0 }, { 24, 32 })
	{
		Open();
	}
	void ProgressOverlay::Update()
	{
		JobSystem& system = Hazard::Application::Get().GetJobSystem();

		if (system.GetQueuedGraphs().size() > 0 || system.GetQueuedJobs().size() > 0)
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
		{
			if (!graph->HasFinished())
				DrawProgressCard(graph->GetName().c_str(), graph->GetCurrentStage()->GetName(), graph->GetProgress());
		}

		ImGui::Separator();
		auto threads = system.GetThreads();
		for (auto& thread : threads)
		{
			if (!thread->GetCurrentJob()) continue;
			auto job = thread->GetCurrentJob();

			if (job->GetJobGraph()) continue;

			DrawProgressCard(job->GetName().c_str(), ",", job->GetProgress());
		}

		ImGui::Separator();

		for (auto& job : system.GetQueuedJobs())
			DrawProgressCard(job->GetName().c_str(), "", job->GetProgress());
	}
	void ProgressOverlay::DrawProgressCard(const std::string& graphName, const std::string& currentJob, float progress)
	{
		std::string progressText = fmt::format("{0}/100", (uint32_t)(progress * 100.0f));

		float width = ImGui::CalcTextSize(progressText.c_str()).x;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);
		Hazard::ImUI::MenuHeader(graphName.c_str(), width - 32);
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::SetCursorPosX(panelWidth - width);
		ImGui::Text("%s", progressText.c_str());

		if (!currentJob.empty())
			ImGui::Text("%s", currentJob.c_str());

		ImGui::ProgressBar(progress, { panelWidth, 32 });
	}
}
