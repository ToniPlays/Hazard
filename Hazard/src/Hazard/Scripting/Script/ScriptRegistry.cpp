#pragma once
#include "hzrpch.h"
#include "ScriptRegistry.h"

#include <mono/jit/jit.h>

#include "bindings.h"

namespace Hazard::Scripting {

	void ScriptRegistry::RegisterAll()
	{
		using namespace Hazard::Scripting::Bindings;
		Register(DebugBindings::GetBindings());
		Register(ApplicationBindings::GetBindings());
		Register(ComponentBindings::GetBindings());
	}
	void ScriptRegistry::Register(BindMap methods)
	{
		for (auto& [name, method] : methods) {
			HZR_CORE_INFO("Registering method {0}", name);
			mono_add_internal_call(name, method);
		}
	}
}
