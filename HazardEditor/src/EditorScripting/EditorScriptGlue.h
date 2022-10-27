#pragma once

#include "Hazard/Scripting/ScriptEngine.h"
#include "Hazard/Scripting/IScriptGlue.h"

namespace Editor 
{
	class EditorScriptGlue : public Hazard::IScriptGlue
	{
	public:
		virtual void Register(Hazard::ScriptEngine* engine);
		virtual void OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly);
	};
}