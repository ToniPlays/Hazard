#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind Tah component calls internal
	class TagComponentBindings {
	public:
		static BindMap GetBindings();

		static MonoString* TagComponent_GetName_Native(uint32_t id);
		static void TagComponent_SetName_Native(uint32_t id, MonoString* newName);
	};
}