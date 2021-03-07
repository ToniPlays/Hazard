#include "hzrpch.h"
#include "ScriptRegistry.h"

#include "../ScriptUtils.h"
#include "../ScriptEngine.h"

#include <mono/jit/jit.h>

namespace Hazard::Scripting {

	static std::string MonoStringToString(MonoString* string) {
		char* ptr = mono_string_to_utf8(string);
		std::string result(ptr);
		mono_free(ptr);
		return result;

	}

	static char* MonoObjectToChar(MonoObject* object) {
		if (object == nullptr) 
			return (char*)'Null';
		else {
			MonoString* string = mono_object_to_string(object, nullptr);
			return _strdup(MonoStringToString(string).c_str());
		}
	}

	void DebugTest(MonoObject* message) {
		HZR_CORE_WARN("C# Debug message: {0}", MonoObjectToChar(message));
	}
	void ScriptRegistry::RegisterAll()
	{
		HZR_CORE_TRACE("Mono internal calls init");
		mono_add_internal_call("Hazard.Debug::Log_Native", DebugTest);
	}
}
