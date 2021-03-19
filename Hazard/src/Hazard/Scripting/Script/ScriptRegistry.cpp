#pragma once
#include "hzrpch.h"
#include "ScriptRegistry.h"

#include <mono/jit/jit.h>

#include "bindings.h"

namespace Hazard::Scripting {

	void ScriptRegistry::RegisterAll()
	{
		using namespace Hazard::Scripting::Bindings;
		//Application bindings
		
		Register(ApplicationBindings::GetBindings());
		Register(DisplayBindings::GetBindings());
		
		//Other bindings
		Register(DebugBindings::GetBindings());
		Register(ComponentBindings::GetBindings());
	}
	void ScriptRegistry::Register(BindMap methods)
	{
		for (auto& [name, method] : methods) {
			mono_add_internal_call(name, method);
		}
	}
}
