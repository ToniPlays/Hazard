#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Overlay.h"
#include "Core/EditorAssetManager.h"

namespace UI 
{
	class ProgressOverlay : public Hazard::ImUI::Overlay
	{
	public:
		ProgressOverlay();
		~ProgressOverlay() = default;
		void Update() override;
		bool OnEvent(Event& e) override { return false; };
		void OnPanelRender() override {};

		//bool DrawProgressCard(const char* title, std::vector<JobGraph>& promises);

		//void AddProcesses(const std::string& type, const std::vector<JobGraph>& promises);
		//void AddProcess(const std::string& type, const JobGraph& promise);

	private:
		std::unordered_map<std::string, std::vector<JobGraph>> m_CurrentProcesses;
	};
}