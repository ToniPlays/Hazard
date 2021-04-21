#pragma once

#include <hzreditor.h>
#include "ScriptDebug.h"

#include "GUI/Library/Style.h"
#include "GUI/Library/Layout/Layout.h"
#include "GUI/Library/Input.h"
#include "GUI/EditorView.h"
#include "Scripting/CSharp/CSharpEngine.h"

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
		using namespace CSharp;
		Layout::Table(2, true, "##Data");
		Layout::Text("Assembly path");
		Layout::TableNext();
		Layout::Text("Assembly path goes BRRR");
		Layout::TableNext();
		Layout::Text("Script count");
		Layout::TableNext();
		Layout::Text("666");
		Layout::EndTable();
	}
}