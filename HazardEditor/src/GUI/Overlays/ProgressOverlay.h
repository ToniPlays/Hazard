#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Overlay.h"
#include "Core/EditorAssetManager.h"
#include "Jobs.h"

namespace UI 
{
	class ProgressOverlay : public Hazard::ImUI::Overlay
	{
	public:
		ProgressOverlay();
		~ProgressOverlay() = default;
		void Update() override;
		bool OnEvent(Event& e) override { return false; };
		void OnPanelRender() override;

		void DrawProgressCard(const std::string& graphName, const std::string& currentJob, float progress);

		//void AddProcesses(const std::string& type, const std::vector<JobGraph>& promises);
		//void AddProcess(const std::string& type, const JobGraph& promise);
	};
}