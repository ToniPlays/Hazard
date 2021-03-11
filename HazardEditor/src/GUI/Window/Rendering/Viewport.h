#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"
#include "GUI/Window/TransformationGizmo.h"

#include "Editor/EditorCamera.h"

using namespace Hazard;
using namespace Hazard::Rendering;



namespace WindowElement {
	class Viewport : public EditorWindow {
	public:
		Viewport();
		~Viewport();
		
		void Init() override;
		void OnFrameBegin() override { gizmos.OnFrameBegin(); };
		void OnWindowRender() override;
		bool OnEvent(Event& e) override;

		TransformationGizmo& GetGizmo() { return gizmos; }

	private:
		bool KeyPressed(KeyPressedEvent& e);

	private:
		Editor::EditorCamera editorCamera = Editor::EditorCamera(60.0f, 16.0f / 9.0f, 0.001f, 1000.0f);

		RenderTexture* renderTexture;

		RenderEngine* renderer;
		TransformationGizmo gizmos;
		uint32_t width = 0, height = 0;

	};
}