
#pragma once

namespace Hazard {
	class IScriptGlue {
	public:
		IScriptGlue() = default;
		virtual ~IScriptGlue() = default;

		virtual void Register(ScriptEngine* engine) = 0;
		virtual void OnAssemblyLoaded(HazardScript::ScriptAssembly* assembly) = 0;
	};
}