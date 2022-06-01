#pragma once

#include "GUI/EditorWindow.h"
#include "Hazard.h"

namespace WindowElement {
	class Performance : public EditorWindow {
	public:
		Performance();
		~Performance() = default;

		void Init() override;
		void OnWindowRender() override;

	private:
		//HazardRenderer::RenderContext* context = nullptr;
	};
}