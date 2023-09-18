#pragma once

#include "HazardScript.h"
#include "Hazard/Core/Application.h"
#include "../ScriptEngine.h"
#include "Coral/CSString.hpp"

#define STRINGIFY_LINE(x) #x
#define LINE(x) STRINGIFY_LINE(x)
#define STACK_TRACE() STRINGIFY_LINE("StackTrace: " __FUNCTION__ ":" LINE(__LINE__))

namespace Hazard
{
	static void Debug_Log_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Debug, msg, STACK_TRACE() });
	}
	static void Debug_Info_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Info, msg, STACK_TRACE() });
	}
	static void Debug_Warn_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Warning, msg, STACK_TRACE() });
	}
	static void Debug_Error_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Error, msg, STACK_TRACE() });
	}
	static void Debug_Critical_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Critical, msg, STACK_TRACE() });
	}
	static void Debug_Trace_Native(const CharType* message)
	{
		std::string msg = Coral::StringHelper::ConvertWideToUtf8(message);
        auto& engine = Application::GetModule<ScriptEngine>();
        engine.SendDebugMessage({ Severity::Trace, msg, STACK_TRACE() });
	}
}
