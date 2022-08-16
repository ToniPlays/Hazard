#include "EditorScriptGlue.h"
#include "GUI/MainMenuBar.h"
#include "Core/GUIManager.h"
#include "HazardScript.h"
#include "Hazard/Scripting/Attributes/AllAttributes.h"

namespace Editor 
{
	using namespace Hazard;

	void EditorScriptGlue::Register(ScriptEngine* engine)
	{
		
	}
	void EditorScriptGlue::OnAssemblyLoaded(HazardScript::ScriptAssembly* assembly) 
	{
		using namespace HazardScript;
		auto& menubar = Application::GetModule<GUIManager>().GetMenuBar();
		menubar.Reset();

		auto view = assembly->ViewAttributes<MenuBarItemAttribute>();
		
		for (auto& script : view) {
			menubar.AddMenuItem(script->Get<MenuBarItemAttribute>().Path, nullptr);
		}
	}
}