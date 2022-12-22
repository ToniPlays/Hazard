
#pragma once
#include "Core/Metadata/ScriptAssembly.h"

namespace Hazard
{
	class IScriptGlue {
	public:
		IScriptGlue() = default;
		virtual ~IScriptGlue() = default;

		virtual void Register(ScriptEngine* engine) = 0;
		virtual void OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly) = 0;
	};
}
