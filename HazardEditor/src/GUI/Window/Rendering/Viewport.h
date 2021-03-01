#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"
#include "GUI/Window/TransformationGizmo.h"

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
	private:
		RenderEngine* renderer;
		TransformationGizmo gizmos;
		uint32_t width = 0, height = 0;
	private:
		bool KeyPressed(KeyPressedEvent& e);
	};
}