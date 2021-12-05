#pragma once


#include "ScriptEngineDebug.h"

#include "Library/Style.h"
#include "Library/Layout/Layout.h"
#include "Library/Input.h"
#include "GUI/EditorView.h"
#include "GUI/Window/Tabs/EditorMainTab.h"
#include "Hazard/Scripting/ScriptRegistry.h"

using namespace WindowLayout;
using namespace Hazard::Scripting;

namespace WindowElement 
{
	ScriptEngineDebug::ScriptEngineDebug() : EditorWindow("Script engine debugger")
	{

	}
	void ScriptEngineDebug::Init()
	{

	}
	void ScriptEngineDebug::OnWindowRender()
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed;

		for (auto& [type, scriptEngine] : ScriptCommand::GetEngines()) {

			ScriptRegistry registry = scriptEngine->GetRegistry();

			Layout::Treenode(ScriptTypeToString(type), flags, [&]() 
				{
				for (auto& [moduleName, script] : registry.GetScripts()) {
					std::stringstream ss;
					ss << script.ClassName << " (" << script.Count << ")";
					ImGui::Text(ss.str().c_str());
				}

				for (auto& [handle, data] : registry.GetRegisteredInstances()) {
					std::stringstream ss;
					UUID entityHandle = handle;
					ss << "Instance: " << data.Handle;
					Layout::Treenode(ss.str().c_str(), flags, [&]() {
						ImGui::Text("Instance scripts:");

						for (auto& script : registry.GetInstanceData(entityHandle).Scripts) 
						{
							ss.str("");
							ss << script->GetModuleName() << ", handle: " << script->GetHandle();
							ImGui::Text(ss.str().c_str());
						}
					});
				}
			});
		}
	}
}