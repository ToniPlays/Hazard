#pragma once
#include <hzrpch.h>
#include "DebugBindings.h"

namespace Hazard::Scripting::Bindings {

	BindMap DebugBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back("Hazard.Debug::Debug_Log_Native",		DebugBindings::Log);
		results.emplace_back("Hazard.Debug::Debug_Info_Native",		DebugBindings::Info);
		results.emplace_back("Hazard.Debug::Debug_Warn_Native",		DebugBindings::Warn);
		results.emplace_back("Hazard.Debug::Debug_Error_Native",	DebugBindings::Error);
		results.emplace_back("Hazard.Debug::Debug_Critical_Native",	DebugBindings::Critical);
		results.emplace_back("Hazard.Debug::Debug_Trace_Native",	DebugBindings::Trace);
		return results;
	}
	void DebugBindings::Log(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Debug, ScriptUtils::MonoObjectToChar(obj));
	}
	void DebugBindings::Info(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Info, ScriptUtils::MonoObjectToChar(obj));
	}
	void DebugBindings::Warn(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Warning, ScriptUtils::MonoObjectToChar(obj));
	}
	void DebugBindings::Error(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Error, ScriptUtils::MonoObjectToChar(obj));
	}
	void DebugBindings::Critical(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Critical, ScriptUtils::MonoObjectToChar(obj));
	}
	void DebugBindings::Trace(MonoObject* obj)
	{
		ScriptCommand::SendDebugMessage(Severity::Trace, ScriptUtils::MonoObjectToChar(obj));
	}
}
