#pragma once

#include "UtilityCore.h"

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