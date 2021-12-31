#pragma once

#include "Scripting/CSharp/ScriptBindings.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind console calls internal
	class ConsoleBindings {
	public:
		static BindMap GetBindings();
		static void Console_Log_Native(MonoObject* object);
		static void Console_Info_Native(MonoObject* object);
		static void Console_Warn_Native(MonoObject* object);
		static void Console_Error_Native(MonoObject* object);
		static void Console_Critical_Native(MonoObject* object);
		static void Console_Trace_Native(MonoObject* object);
	};
}