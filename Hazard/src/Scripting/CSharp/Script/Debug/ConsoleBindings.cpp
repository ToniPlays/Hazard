#pragma once
#include <hzrpch.h>
#include "ConsoleBindings.h"
#include "Hazard/Scripting/ScriptCommand.h"
#include "../../ScriptUtils.h"

namespace Hazard::Scripting::CSharp::Bindings {

	BindMap ConsoleBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Console::Console_Log_Native", Log);
		results.emplace_back("Hazard.Console::Console_Info_Native", Info);
		results.emplace_back("Hazard.Console::Console_Warn_Native", Warn);
		results.emplace_back("Hazard.Console::Console_Error_Native", Error);
		results.emplace_back("Hazard.Console::Console_Critical_Native", Critical);
		results.emplace_back("Hazard.Console::Console_Trace_Native", Trace);
		return results;
	}
	void ConsoleBindings::Log(MonoObject* obj)
	{
		HZR_CORE_INFO("Loogging data");
		ScriptCommand::SendDebugMessage(Severity::Debug, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Info(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Info, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Warn(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Warning, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Error(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Error, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Critical(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Critical, ScriptUtils::MonoObjectToChar(obj));
	}
	void ConsoleBindings::Trace(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Trace, ScriptUtils::MonoObjectToChar(obj));
	}
}