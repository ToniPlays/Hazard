#pragma once
#include <hzrpch.h>
#include "DebugBindings.h"

namespace Hazard::Scripting::Bindings {

	BindMap DebugBindings::GetBindings()
	{
		BindMap results;
		results.emplace_back(std::pair("Hazard.Debug::Debug_Log_Native",		DebugBindings::Log));
		results.emplace_back(std::pair("Hazard.Debug::Debug_Info_Native",		DebugBindings::Info));
		results.emplace_back(std::pair("Hazard.Debug::Debug_Warn_Native",		DebugBindings::Warn));
		results.emplace_back(std::pair("Hazard.Debug::Debug_Error_Native",		DebugBindings::Error));
		results.emplace_back(std::pair("Hazard.Debug::Debug_Critical_Native",	DebugBindings::Critical));
		results.emplace_back(std::pair("Hazard.Debug::Debug_Trace_Native",		DebugBindings::Trace));
		return results;
	}
	void DebugBindings::Log(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Debug, ScriptUtils::MonoObjectToChar(object));
	}
	void DebugBindings::Info(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Info, ScriptUtils::MonoObjectToChar(object));
	}
	void DebugBindings::Warn(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Warning, ScriptUtils::MonoObjectToChar(object));
	}
	void DebugBindings::Error(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Error, ScriptUtils::MonoObjectToChar(object));
	}
	void DebugBindings::Critical(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Critical, ScriptUtils::MonoObjectToChar(object));
	}
	void DebugBindings::Trace(MonoObject* object)
	{
		ScriptCommand::SendDebugMessage(Severity::Trace, ScriptUtils::MonoObjectToChar(object));
	}
}
