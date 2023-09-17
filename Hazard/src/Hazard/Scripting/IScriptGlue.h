
#pragma once
#include "Core/Metadata/ScriptAssembly.h"

#define BIND_ICALL(x) assembly->AddInternalCall("Hazard.InternalCalls", #x, &x)

namespace Hazard
{
	class IScriptGlue {
	public:
		IScriptGlue() = default;
		virtual ~IScriptGlue() = default;

		virtual void Register(Ref<HazardScript::ScriptAssembly> assembly) = 0;
		virtual void OnAssemblyLoaded(Ref<HazardScript::ScriptAssembly> assembly) = 0;
	};
}
