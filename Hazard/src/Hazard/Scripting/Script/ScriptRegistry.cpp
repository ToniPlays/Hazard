#include "hzrpch.h"
#include "ScriptRegistry.h"

#include "../ScriptUtils.h"
#include "../ScriptEngine.h"
#pragma once

#include <mono/jit/jit.h>

#include "bindings.h"

namespace Hazard::Scripting {

	void ScriptRegistry::RegisterAll()
	{
		using namespace Hazard::Scripting::Bindings;
		Register(DebugBindings::GetBindings());
		Register(ApplicationBindings::GetBindings());
	}
	void ScriptRegistry::Register(std::vector<std::pair<const char*, void*>> methods)
	{
		for (auto [name, method] : methods) {
			mono_add_internal_call(name, method);
		}
	}
}
