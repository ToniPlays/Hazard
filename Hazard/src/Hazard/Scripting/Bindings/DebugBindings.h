#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"

#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() "StackTrace: " __FUNCTION__ ":" LINE(__LINE__) 

namespace Hazard
{
	using namespace HazardScript;
	static void Debug_Log_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToString(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Debug, message, STACK_TRACE() });
	}
	static void Debug_Info_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToString(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Info, message, STACK_TRACE() });
	}
	static void Debug_Warn_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToString(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Warning, message, STACK_TRACE() });
	}
	static void Debug_Error_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToString(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Error, message, STACK_TRACE() });
	}
	static void Debug_Critical_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Critical, message, STACK_TRACE() });
	}
	static void Debug_Trace_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Trace, message, STACK_TRACE() });
	}
}