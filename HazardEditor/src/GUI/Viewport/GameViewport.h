#pragma once

#include "Hazard.h"
#include "TransformationGizmo.h"
#include "Hazard/Rendering/WorldRenderer.h"

namespace UI
{
	class GameViewport : public Hazard::ImUI::Panel
	{
	public:
		GameViewport();
		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

	private:
		uint32_t m_Width = 1280, m_Height = 720;
		HazardRenderer::Camera* m_CurrentCamera = nullptr;

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
	};
}