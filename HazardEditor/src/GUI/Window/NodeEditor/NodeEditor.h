#pragma once

#include <imgui_node_editor.h>

namespace node = ax::NodeEditor;

class NodeEditor {
public:
	NodeEditor();
	~NodeEditor();

	void BeginEditor();
	void EndEditor();
private:
	node::EditorContext* context;
};