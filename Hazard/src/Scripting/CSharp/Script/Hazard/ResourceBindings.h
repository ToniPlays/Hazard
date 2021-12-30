#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {

	class ResourceBindings {
	public:
		static BindMap GetBindings();
	private:
		static uint32_t Resource_Create_Native(void* type);
	};
}