#pragma once

#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class MeshAssetEditorPanel : public Hazard::ImUI::Panel
	{
	public:

		MeshAssetEditorPanel() = default;
		~MeshAssetEditorPanel() = default;

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

	private:
		
	};
}