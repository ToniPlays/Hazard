#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"
#include "GUI/Window/Rendering/TransformationGizmo.h"

#include "Editor/EditorCamera.h"

using namespace Hazard;
using namespace Hazard::Rendering;

namespace WindowElement {

	class Viewport : public EditorWindow {
	public:
		Viewport();
		~Viewport();
		
		void Init() override;
		void OnFrameBegin() override { m_Gizmos.OnFrameBegin(); };
		void OnWindowRender() override;
		bool OnEvent(Event& e) override;

		TransformationGizmo& GetGizmo() { return m_Gizmos; }

	private:
		bool KeyPressed(KeyPressedEvent& e);

	private:
		Editor::EditorCamera m_EditorCamera = Editor::EditorCamera(60.0f, 16.0f / 9.0f, 0.001f, 1000.0f);

		Ref<FrameBuffer> m_RenderTexture;

		RenderEngine* m_Renderer = nullptr;
		TransformationGizmo m_Gizmos;
		uint32_t m_Width = 0, m_Height = 0;

	};
}