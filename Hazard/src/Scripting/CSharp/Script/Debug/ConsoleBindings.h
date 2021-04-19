#pragma once

#include "../../ScriptRegistery.h"

namespace Hazard::Scripting::CSharp::Bindings {
	//Bind console calls internal
	class ConsoleBindings {
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