#pragma once

#include "GUI/EditorWindow.h"
#include "Hazard.h"

namespace WindowElement {
	class Performance : public EditorWindow {
	public:
		Performance();
		~Performance();


		void Init() override;
		void OnWindowRender() override;

	private:
		Hazard::Rendering::RenderContext* context;
		Hazard::Rendering::RenderStats stats;
	};
}