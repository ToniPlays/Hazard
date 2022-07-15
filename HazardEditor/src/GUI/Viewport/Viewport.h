#pragma once

#include "GUI/Core/Panel.h"
#include "TransformationGizmo.h"
#include "Editor/EditorCamera.h"

namespace UI 
{
	class Viewport : public Panel {
	public:
		Viewport() : Panel("Viewport") {};

		void OnPanelRender() override;
		bool OnEvent(Event& e) override;
	private:
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
		void DrawStatsWindow();
	private:
		uint32_t m_Width = 0, m_Height = 0;
		TransformationGizmo m_Gizmos;
		Editor::EditorCamera m_EditorCamera;

		Entity m_SelectionContext;

		bool m_DrawStats = false;
	};
}