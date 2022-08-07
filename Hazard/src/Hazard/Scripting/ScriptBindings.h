#pragma once

#include "ScriptEngine.h"
#include "Hazard/Core/Application.h"

#define BIND_ICALL(x) engine->RegisterInternalCall("Hazard.InternalCalls::"#x, (void*)x)
#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() "StackTrace: " __FUNCTION__ ":" LINE(__LINE__) 

namespace Hazard
{

	using namespace HazardScript;
	static void Console_Log_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Debug, message, STACK_TRACE() });
	}
	static void Console_Info_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Info, message, STACK_TRACE() });
	}
	static void Console_Warn_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Warning, message, STACK_TRACE() });
	}
	static void Console_Error_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Error, message, STACK_TRACE() });
	}
	static void Console_Critical_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Critical, message, STACK_TRACE() });
	}
	static void Console_Trace_Native(MonoObject* obj)
	{
		std::string message = Mono::MonoObjectToChar(obj);
		Application::GetModule<ScriptEngine>().SendDebugMessage({ Severity::Trace, message, STACK_TRACE() });
	}


	class InternalCall : public IScriptGlue {
	public:
		virtual void Register(ScriptEngine* engine) {
			BIND_ICALL(Console_Log_Native);
			BIND_ICALL(Console_Info_Native);
			BIND_ICALL(Console_Warn_Native);
			BIND_ICALL(Console_Error_Native);
			BIND_ICALL(Console_Critical_Native);
			BIND_ICALL(Console_Trace_Native);
		}
		virtual void OnAssemblyLoaded(HazardScript::ScriptAssembly* assembly) {};
	};

}