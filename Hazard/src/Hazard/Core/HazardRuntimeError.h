#pragma once
#include "Hazard/Core/Core.h"

class HazardRuntimeError : public std::exception
{
public:
	HazardRuntimeError(std::string error, std::string from) : error(error), from(from) {};
	const char* what() const override { return (error + "\n" + from).c_str(); }

private:
	std::string error = "";
	std::string from = "";
};