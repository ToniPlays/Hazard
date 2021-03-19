#pragma once

#include "Hazard/Scripting/ScriptUtils.h"
#include "Hazard/Scripting/ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::Bindings {

	//Bind aplication functions
	class InputBindings {
	public:
		static BindMap GetBindings();
	private:
		static bool KeyDown(uint16_t key);
		static bool KeyPressed(uint16_t key);
		static bool KeyReleased(uint16_t key);
	};
}