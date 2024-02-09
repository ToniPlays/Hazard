#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Overlay.h"
#include "Core/EditorAssetManager.h"
#include "Jobs.h"

namespace UI 
{
	struct JobGraphProgress
	{
		double SubmitTime;
		double FinishedAt;
		double RemoveAfter;
		double TerminatedAt;
		Ref<JobGraph> Graph;
	};

	class ProgressOverlay : public Hazard::ImUI::Overlay
	{
	public:
		ProgressOverlay();
		~ProgressOverlay() = default;

		void Update() override;
		bool OnEvent(Event& e) override { return false; };
		void OnPanelRender() override;

		void DrawProgressCard(Ref<JobGraph> graph);

	private:
		std::vector<JobGraphProgress> m_JobGraphs;
	};
}