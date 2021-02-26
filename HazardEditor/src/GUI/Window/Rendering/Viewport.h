#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

using namespace Hazard;
using namespace Hazard::Rendering;

namespace WindowElement {
	class Viewport : public EditorWindow {
	public:
		Viewport();
		~Viewport();
		
		void Init() override;
		void OnWindowRender() override;
	
	private:
		RenderEngine* renderer;
		
	};
}