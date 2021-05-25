#pragma once
#include "GUI/EditorWindow.h"
#include "imgui.h"

namespace WindowElement {

	class ExportView : public EditorWindow {
	public:
		ExportView();
		~ExportView();

		void OnWindowRender() override;

	};
}