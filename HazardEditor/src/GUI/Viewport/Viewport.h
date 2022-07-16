#pragma once

#include "GUI/Core/Panel.h"
#include "TransformationGizmo.h"
#include "Editor/EditorCamera.h"
#include "Hazard/Rendering/WorldRenderer.h"

namespace UI 
{
	class Viewport : public Panel {
	public:
		Viewport();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
	private:
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
		void DrawStatsWindow();
	private:
		uint32_t m_Width = 1280, m_Height = 720;
		TransformationGizmo m_Gizmos;
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, (float)m_Width / (float)m_Height, 0.03f, 100.0f);

		Ref<WorldRenderer> m_Renderer;
		Entity m_SelectionContext;

		bool m_DrawStats = false;
	};
}