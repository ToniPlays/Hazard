#pragma once

#include "GUI/Core/Panel.h"
#include "TransformationGizmo.h"
#include "Editor/EditorCamera.h"
#include "Editor/EditorGrid.h"
#include "Hazard/Rendering/WorldRenderer.h"

namespace UI 
{
	class Viewport : public Panel {
	public:
		Viewport();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		bool FocusOnEntity(Entity& entity);
	private:
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
		bool KeyPressed(KeyPressedEvent& e);
		void DrawStatsWindow();
	private:
		uint32_t m_Width = 1280, m_Height = 720;
		TransformationGizmo m_Gizmos;
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, (float)m_Width / (float)m_Height, 0.03f, 1000.0f);

		Editor::Grid m_EditorGrid;

		Ref<WorldRenderer> m_Renderer;
		Entity m_SelectionContext;

		bool m_DrawStats = false;
		bool m_MouseOverWindow = false;
		bool m_WindowFocused = false;
	};
}