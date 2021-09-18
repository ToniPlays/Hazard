#pragma once
#include "GUI/EditorWindow.h"
#include "imgui.h"

namespace WindowElement {

	class ExportView : public EditorWindow {
	public:
		ExportView();
		~ExportView() = default;

		void OnWindowRender() override;

	};
}