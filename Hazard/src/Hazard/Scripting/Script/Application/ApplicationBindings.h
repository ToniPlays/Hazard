#pragma once

#include "../../ScriptUtils.h"
#include "../../ScriptCommand.h"


extern "C"
{
	typedef struct _MonoObject MonoObject;
}


namespace Hazard::Scripting::Bindings {

	//Bind aplication functions

	class ApplicationBindings {
	public:
		static BindMap GetBindings();
	};
}