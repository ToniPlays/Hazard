#pragma once

#include <string>

enum class Severity
{
	Debug,
	Trace,
	Info,
	Warning,
	Error,
	Critical
};

std::string SeverityToString(const Severity& severity);