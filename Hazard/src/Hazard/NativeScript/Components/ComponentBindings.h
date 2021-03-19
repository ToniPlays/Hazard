#pragma once

#include "Hazard/Scripting/ScriptUtils.h"
#include "Hazard/Scripting/ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::Bindings {

	class ComponentBindings {

	public:
		static BindMap GetBindings();

		static MonoString* GetEntityName(uint32_t id);
		static void SetEntityName(uint32_t id, MonoString* value);

		static bool GetEntityActive(uint32_t id);
		static void SetEntityActive(uint32_t id, bool value);
	};
}