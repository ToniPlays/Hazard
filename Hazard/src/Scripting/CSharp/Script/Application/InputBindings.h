#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
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