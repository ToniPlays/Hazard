#pragma once

#include "Hazard/Scripting/ScriptUtils.h"
#include "Hazard/Scripting/ScriptCommand.h"


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