#pragma once

#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Scripting/IScriptGlue.h"

namespace Editor::Bindings
{
	class EditorScriptGlue : public Hazard::IScriptGlue
	{
	public:
		virtual void Register(Ref<HazardScript::ScriptAssembly> assembly);
		virtual void OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly);
	};
}
