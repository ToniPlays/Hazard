#pragma once

#include "../../ScriptUtils.h"
#include "../../ScriptCommand.h"

extern "C"
{
	typedef struct _MonoObject MonoObject;
}

namespace Hazard::Scripting::Bindings {

	//Bind debug calls internal

	class DebugBindings {
	public:
		static BindMap GetBindings();
		static void Log(MonoObject* object);
		static void Info(MonoObject* object);
		static void Warn(MonoObject* object);
		static void Error(MonoObject* object);
		static void Critical(MonoObject* object);
		static void Trace(MonoObject* object);
	};
}