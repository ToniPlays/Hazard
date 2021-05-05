#pragma once
#include "Hazard.h"
#include "GUI/EditorWindow.h"

#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;

namespace WindowElement {

	class ShaderEditorWindow : public EditorWindow {
	public:
		ShaderEditorWindow();
		~ShaderEditorWindow();

		void OnFrameBegin() override { };
		void OnFrameEnd() override { };
		void OnWindowRender() override;

	private:
		ed::EditorContext* m_Context;
	};
}