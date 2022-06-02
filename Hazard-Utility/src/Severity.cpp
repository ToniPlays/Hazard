#include "Severity.h"


std::string SeverityToString(const Severity& severity) {
	switch (severity)
	{
	case Severity::Debug:		return "Debug";
	case Severity::Trace:		return "Trace";
	case Severity::Info:		return "Info";
	case Severity::Warning:		return "Warning";
	case Severity::Error:		return "Error";
	case Severity::Critical:	return "Critical";
	}
	return "Unknown";
}