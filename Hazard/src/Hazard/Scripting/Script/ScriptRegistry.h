#pragma once

#include "../ScriptUtils.h"
#include "../ScriptEngine.h"

namespace Hazard::Scripting {

	class ScriptRegistry {
	public:
		static void RegisterAll();
	private:
		static void Register(BindMap methods);
	};
}