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
		void OnPanelRender() override;

		bool DrawProgressCard(const char* title, std::vector<JobPromise>& promises);

		void AddProcesses(AssetType type, const std::vector<JobPromise>& promises);

	private:
		std::unordered_map<AssetType, std::vector<JobPromise>> m_CurrentProcesses;
	};
}