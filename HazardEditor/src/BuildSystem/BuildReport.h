#pragma once

#include "Hazard.h"

namespace Exporter {
	enum class BuildMessageSeverity {
		Info = 0,
		Warning,
		Error,
		Fatal
	};

	struct BuildMessage {
		std::string Message;
		std::string Context;
		BuildMessageSeverity Severity;

		BuildMessage(std::string message, std::string context, BuildMessageSeverity severity) : Message(message), Context(context), Severity(severity) {}
	};

	struct BuildReport
	{
		size_t BuildTime;
		size_t TotalSize;
		bool Result;

		std::vector<BuildMessage> Messages;

		void AddMessage(BuildMessage message) { Messages.push_back(message); }
	};
}