#pragma once

#include "../../ScriptRegistery.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}
namespace Hazard::Scripting::CSharp::Bindings {
	//Bind aplication functions
	class ApplicationBindings {
	public:
		static BindMap GetBindings();
	};
}