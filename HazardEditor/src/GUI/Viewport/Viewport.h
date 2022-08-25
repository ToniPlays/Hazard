#pragma once

#include "Hazard.h"
#include "TransformationGizmo.h"
#include "Editor/EditorCamera.h"
#include "Editor/EditorGrid.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"

namespace UI 
{
	class Viewport : public Hazard::ImUI::Panel {
	public:
		Viewport();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		bool FocusOnEntity(Entity& entity);
		Entity GetSelectionContext() { return m_SelectionContext; }

	private:
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
		bool KeyPressed(KeyPressedEvent& e);
		void DrawStatsWindow();

	private:
		uint32_t m_Width = 1280, m_Height = 720;
		TransformationGizmo m_Gizmos;
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, (float)m_Width / (float)m_Height, 0.03f, 100.0f);

		Editor::Grid m_EditorGrid;
		Entity m_SelectionContext;

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;

		bool m_DrawStats = false;
		bool m_MouseOverWindow = false;
		bool m_WindowFocused = false;

		uint32_t m_CurrentImage = 0;
	};
}