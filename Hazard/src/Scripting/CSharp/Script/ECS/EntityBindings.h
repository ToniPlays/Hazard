#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind debug calls internal
	class EntityBindings {
	public:
		static BindMap GetBindings();
		static bool HasComponent(uint32_t id, void* type);
	private:
	};
}