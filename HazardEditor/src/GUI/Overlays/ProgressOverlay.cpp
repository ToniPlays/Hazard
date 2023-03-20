#include "ProgressOverlay.h"

namespace UI
{
	ProgressOverlay::ProgressOverlay() : Overlay("Progress", { 300, 0 }, { 24, 32 })
	{
		Open();
	}
	void ProgressOverlay::Update()
	{
		for (auto& [type, promises] : m_CurrentProcesses)
		{
			if (promises.size() == 0) 
				continue;
			Open();
			return;
		}
		Close();
	}
	/*
	void ProgressOverlay::OnPanelRender()
	{
		for (auto& [type, jobs] : m_CurrentProcesses)
		{
			if (jobs.size() == 0) continue;

			if (DrawProgressCard(type.c_str(), jobs))
				m_CurrentProcesses[type].clear();
		}
		
	}
	bool ProgressOverlay::DrawProgressCard(const char* title, std::vector<JobPromise>& promises)
	{
		uint32_t completedJobs = 0;
		float totalProgress = 0;
		
		for (JobPromise& promise : promises)
		{
			completedJobs += promise.Status() >= JobStatus::Done;
			totalProgress += promise.Progress();
		}

		totalProgress /= (float)promises.size();

		std::string progress = fmt::format("{0}/{1}", completedJobs, promises.size());
		ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[2]);

		float width = ImGui::CalcTextSize(progress.c_str()).x;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		ImGui::Text("%s", title);
		ImGui::SameLine();
		ImGui::SetCursorPosX(panelWidth - width);
		ImGui::Text("%s", progress.c_str());
		ImGui::PopFont();

		ImGui::ProgressBar(totalProgress, { panelWidth, 24 });

		return completedJobs == promises.size();
	}
	void ProgressOverlay::AddProcesses(const std::string& type, const std::vector<JobPromise>& promises)
	{
		auto& a = m_CurrentProcesses[type];
		a.insert(a.end(), promises.begin(), promises.end());
	}
	void ProgressOverlay::AddProcess(const std::string& type, const JobPromise& promise)
	{
		auto& a = m_CurrentProcesses[type];
		a.push_back(promise);
	}*/
}
