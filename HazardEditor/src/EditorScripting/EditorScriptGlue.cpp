#include "EditorScriptGlue.h"
#include "HazardScript.h"
#include "Bindings/ApplicationBindings.h"
#include "Bindings/DisplayBindings.h"

namespace Editor::Bindings
{
	using namespace HazardScript;

	void EditorScriptGlue::Register(Ref<ScriptAssembly> assembly)
	{
		//Overwrite Display bindings
		BIND_ICALL(Application_Quit_Native);
		BIND_ICALL(Display_Width_Native);
		BIND_ICALL(Display_Height_Native);

	}
	void EditorScriptGlue::OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly) 
	{
		using namespace HazardScript;
		/*
		if (Application::HasModule<GUIManager>()) 
		{
			auto& menubar = Application::GetModule<GUIManager>().GetMenuBar();
			menubar.Reset();

			auto view = assembly->ViewAttributes<MenuBarItemAttribute>();

			for (auto& script : view)
				menubar.AddMenuItem(script->Get<MenuBarItemAttribute>()->Path, nullptr);
		}
		*/
	}
}
