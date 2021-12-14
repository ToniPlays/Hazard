#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"
#include "GUI/Window/Rendering/TransformationGizmo.h"
#include "Editor/EditorGrid.h"

#include "Editor/EditorCamera.h"
#include "Hazard/Rendering/WorldRenderer.h"

using namespace Hazard;
using namespace Hazard::Rendering;

namespace WindowElement {

	class Viewport : public EditorWindow {
	public:
		Viewport();
		~Viewport() = default;
		
		void Init() override;
		void OnUpdate() override;
		void OnFrameBegin() override { m_Gizmos.OnFrameBegin(); };
		void OnWindowRender() override;
		bool OnEvent(Event& e) override;

		TransformationGizmo& GetGizmo() { return m_Gizmos; }
		bool FocusOnEntity(ECS::Entity entity);

	private:
		bool KeyPressed(KeyPressedEvent& e);
		void DrawStatsWindow();

	private:
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, 16.0f / 9.0f, 0.001f, 1000.0f);
		Editor::Grid m_Grid;

		Ref<WorldRenderer> m_WorldRenderer;
		TransformationGizmo m_Gizmos;
		uint32_t m_Width = 0, m_Height = 0;
		bool m_DrawStats = false;

	};
}