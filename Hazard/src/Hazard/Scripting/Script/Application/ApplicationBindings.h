#pragma once

#include "../../ScriptUtils.h"
#include "../../ScriptCommand.h"


extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::Bindings {

	class ApplicationBindings {
	public:
		static BindMap GetBindings();
	};
}