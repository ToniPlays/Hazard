#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind aplication functions
	class InputBindings {
	public:
		static BindMap GetBindings();
	private:
		static bool Input_KeyDown_Native(uint16_t key);
		static bool Input_KeyPressed_Native(uint16_t key);
		static bool Input_KeyReleased_Native(uint16_t key);
	};
}