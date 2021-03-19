#pragma once

#include "../../ScriptUtils.h"
#include "../../ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::Bindings {

	class ComponentBindings {

	public:
		static BindMap GetBindings();

		static MonoString* GetEntityName(uint64_t id);
		static void SetEntityName(uint64_t id, MonoString* value);
	};
}