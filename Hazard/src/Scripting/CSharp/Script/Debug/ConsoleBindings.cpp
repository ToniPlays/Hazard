#pragma once
#include <hzrpch.h>
#include "ConsoleBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap ConsoleBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Console::Console_Log_Native",		Console_Log_Native);
		results.emplace_back("Hazard.Console::Console_Info_Native",		Console_Info_Native);
		results.emplace_back("Hazard.Console::Console_Warn_Native",		Console_Warn_Native);
		results.emplace_back("Hazard.Console::Console_Error_Native",	Console_Error_Native);
		results.emplace_back("Hazard.Console::Console_Critical_Native", Console_Critical_Native);
		results.emplace_back("Hazard.Console::Console_Trace_Native",	Console_Trace_Native);
		return results;
	}
	void ConsoleBindings::Console_Log_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Debug, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Console_Info_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Info, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Console_Warn_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Warning, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Console_Error_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Error, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Console_Critical_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Critical, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Console_Trace_Native(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Trace, ScriptUtils::MonoObjectToChar(obj));
	}
}