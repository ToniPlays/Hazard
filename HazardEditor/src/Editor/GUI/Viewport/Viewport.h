#pragma once

#include "Hazard.h"
#include "TransformationGizmo.h"
#include "Editor/EditorCamera.h"
#include "Editor/EditorGrid.h"
#include "Hazard/Rendering/Renderers/WorldRenderer.h"
#include "Hazard/ImGUI/Panel.h"

namespace UI 
{
	enum ViewportSettingsFlags
	{
		ViewportSettingsFlags_BoundingBox = BIT(0),
		ViewportSettingsFlags_CameraFrustum = BIT(1),
		ViewportSettingsFlags_LightIcons = BIT(2)
	};

	class Viewport : public Hazard::ImUI::Panel 
	{
	public:
		Viewport();

		void Update() override;
		void OnPanelRender() override;
		bool OnEvent(Event& e) override;

		bool FocusOnEntity(Entity& entity);
		std::vector<Entity> GetSelectionContext() { return m_SelectionContext; }

	private:
		bool OnSelectionContextChange(Events::SelectionContextChange& e);
		bool KeyPressed(KeyPressedEvent& e);
        void OnMouseClicked(const glm::vec2& mousePos);
		void DrawStatsWindow();
		void DrawSettingsWindow();

		void DrawLeftToolbar(ImVec2 corner);
		void DrawRightToolbar(ImVec2 size);
		void DrawPlaymodeButtons(ImVec2 size);

	private:
		uint32_t m_Width = 1280, m_Height = 720;
		TransformationGizmo m_Gizmos;
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, (float)m_Width / (float)m_Height, 0.01f, 1000.0f);

		Editor::Grid m_EditorGrid;
		std::vector<Entity> m_SelectionContext;

		Ref<HazardRenderer::RenderPass> m_RenderPass;
		Ref<HazardRenderer::FrameBuffer> m_FrameBuffer;
		Ref<HazardRenderer::Sampler> m_ImageSampler;
        
        Buffer m_MouseClickBuffer;

		bool m_DrawStats = false;
		bool m_DrawSettings = false;
		bool m_WindowFocused = false;
		uint32_t m_ViewportSettings = ViewportSettingsFlags_LightIcons | ViewportSettingsFlags_CameraFrustum;
	};
}
