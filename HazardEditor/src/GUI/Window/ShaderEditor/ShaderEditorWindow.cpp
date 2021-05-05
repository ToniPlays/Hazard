#pragma once

#include <hzreditor.h>
#include "ShaderEditorWindow.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Style.h"

using namespace WindowLayout;


namespace WindowElement {

	ShaderEditorWindow::ShaderEditorWindow() : EditorWindow(ICON_FK_WRENCH " Shader")
	{
		m_Context = ed::CreateEditor();
		SetActive(false);
	}
	ShaderEditorWindow::~ShaderEditorWindow()
	{
		ed::DestroyEditor(m_Context);
	}
	void ShaderEditorWindow::OnWindowRender()
	{
		ed::SetCurrentEditor(m_Context);
		ed::Begin("ShaderEditor");
		
		int uniqueId = 1;
		// Start drawing nodes.
		ed::BeginNode(uniqueId++);
		ImGui::Text("Node A");
		ed::BeginPin(uniqueId++, ed::PinKind::Input);
		ImGui::Text("-> In");
		ed::EndPin();
		ImGui::SameLine();
		ed::BeginPin(uniqueId++, ed::PinKind::Output);
		ImGui::Text("Out ->");
		ed::EndPin();
		ed::EndNode();

		ed::End();
		ed::SetCurrentEditor(nullptr);
	}
}