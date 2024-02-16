#pragma once

#include "Hazard.h"
#include "Hazard/ImGUI/Panel.h"
#include "Hazard/Rendering/RendererDrawList.h"

namespace UI
{
	class RendererDebugPanel : public Hazard::ImUI::Panel
	{
	public:
		RendererDebugPanel() : Hazard::ImUI::Panel("Renderer debugger") {};

		void Update() override {};
		void OnPanelRender() override;
		bool OnEvent(Event& e) override { return false; };

	private:
		void DrawWorldDrawList(const Hazard::RendererDrawList& list);
		void RenderGeometryPass(const std::map<Hazard::MeshKey, Hazard::MeshData>& meshes);
	};
}