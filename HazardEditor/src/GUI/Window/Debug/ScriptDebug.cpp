#pragma once

#include <hzreditor.h>
#include "ScriptDebug.h"

#include "GUI/Library/Style.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/EditorView.h"

using namespace WindowLayout;
using namespace Hazard::Scripting;

namespace WindowElement {

	ScriptDebug::ScriptDebug() : EditorWindow("ScriptDebugger")
	{

	}
	ScriptDebug::~ScriptDebug()
	{

	}
	void ScriptDebug::Init()
	{

	}
	void ScriptDebug::OnWindowRender()
	{
		auto engines = Application::GetModule<ScriptEngineManager>().GetScriptEngines();
		for (std::pair<ScriptType, ScriptEngine*> engine : engines) {
			Layout::Treenode(engine.second->GetStats().name, Appereance::Style::GetTreeNodeDefaultFlags(), [&]() {
				Layout::Table(2);
				Layout::SetColumnWidth(150);
				Layout::Text("Assembly path");
				Layout::TableNext();
				Layout::Text(engine.second->GetStats().assemblyPath);

				Layout::EndTable();
			});
		}
	}
}