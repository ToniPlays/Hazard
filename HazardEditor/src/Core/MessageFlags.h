
#pragma once

#include "UtilityCore.h"
#include "Severity.h"

enum MessageFlags : uint32_t 
{
	MessageFlags_None = BIT(0),
	MessageFlags_Debug = BIT(1),
	MessageFlags_Info = BIT(2),
	MessageFlags_Warning = BIT(3),
	MessageFlags_Error = BIT(4),
	MessageFlags_Fatal = BIT(5),
	MessageFlags_Clearable = BIT(6)
};


static uint32_t GetMessageFlagsFromSeverity(const Severity& severity)
{
	switch (severity)
	{
	case Severity::Debug:		return MessageFlags_Debug | MessageFlags_Clearable;
	case Severity::Info:		return MessageFlags_Info | MessageFlags_Clearable;
	case Severity::Trace:		return MessageFlags_Info | MessageFlags_Clearable;
	case Severity::Warning:		return MessageFlags_Warning | MessageFlags_Clearable;
	case Severity::Error:		return MessageFlags_Error | MessageFlags_Clearable;
	case Severity::Critical:	return MessageFlags_Fatal;
	}
	return 0;
}