#pragma once

#include "Hazard.h"
#include "TransformationGizmo.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"
#include "HazardRendererCore.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI
{
	class GameViewport : public Hazard::ImUI::Panel
	{
	public:
		GameViewport();
		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		uint32_t m_Width = 1280, m_Height = 720;
		bool m_HasCamera = false;

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
	};
}