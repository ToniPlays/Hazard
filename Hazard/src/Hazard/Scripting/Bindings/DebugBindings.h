#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Coral/NativeString.hpp"

#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() STRINGIFY_LINE("StackTrace: " __FUNCTION__ ":" LINE(__LINE__))

namespace Hazard
{
	static void Debug_Log_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Debug, message.ToString(), STACK_TRACE() });
	}
	static void Debug_Info_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Info, message.ToString(), STACK_TRACE() });
	}
	static void Debug_Warn_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Warning, message.ToString(), STACK_TRACE() });
	}
	static void Debug_Error_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Error, message.ToString(), STACK_TRACE() });
	}
	static void Debug_Critical_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Critical, message.ToString(), STACK_TRACE() });
	}
	static void Debug_Trace_Native(Coral::NativeString message)
	{
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Trace, message.ToString(), STACK_TRACE() });
	}
}
