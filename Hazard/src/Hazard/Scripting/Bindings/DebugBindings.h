#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"

#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() STRINGIFY_LINE("StackTrace: " __FUNCTION__ ":" LINE(__LINE__))

namespace Hazard
{
	static void Debug_Log_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Debug, message, STACK_TRACE() });
	}
	static void Debug_Info_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Info, message, STACK_TRACE() });
	}
	static void Debug_Warn_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Warning, message, STACK_TRACE() });
	}
	static void Debug_Error_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Error, message, STACK_TRACE() });
	}
	static void Debug_Critical_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Critical, message, STACK_TRACE() });
	}
	static void Debug_Trace_Native(Coral::String message)
	{
        auto& engine = Application::Get().GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Trace, message, STACK_TRACE() });
	}
}
