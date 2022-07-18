#pragma once

#include "ScriptEngine.h"
#include "Hazard/Core/Application.h"

#define BIND_ICALL(name, x) engine->RegisterInternalCall(name, (void*)x)

namespace Hazard::Internal
{
	using namespace HazardScript;
	static void ConsoleLog(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Debug, message, stackTrace });
	}
	static void ConsoleInfo(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Info, message, stackTrace });
	}
	static void ConsoleWarn(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Warning, message, stackTrace });
	}
	static void ConsoleError(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Error, message, stackTrace });
	}
	static void ConsoleCritical(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Critical, message, stackTrace });
	}
	static void ConsoleTrace(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		std::string stackTrace = "StackTrace: ";
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Trace, message, stackTrace });
	}

	static void RegisterInternalCalls(ScriptEngine* engine)
	{
		BIND_ICALL("Hazard.Console::Console_Log_Native", ConsoleLog);
		BIND_ICALL("Hazard.Console::Console_Info_Native", ConsoleInfo);
		BIND_ICALL("Hazard.Console::Console_Warn_Native", ConsoleWarn);
		BIND_ICALL("Hazard.Console::Console_Error_Native", ConsoleError);
		BIND_ICALL("Hazard.Console::Console_Critical_Native", ConsoleCritical);
		BIND_ICALL("Hazard.Console::Console_Trace_Native", ConsoleTrace);
	}
}