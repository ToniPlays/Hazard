#pragma once
#include "Hazard/Core/Core.h"

class HazardRuntimeError : public std::exception
{
public:
	HazardRuntimeError(std::string error, std::string from) : error(error), from(from) {};
#ifdef HZR_PLATFORM_MACOS
	const char* what() const noexcept override { return error.c_str(); }
#else
	const char* what() const override { return error.c_str(); }
#endif
    const std::string& where() { return from; }

private:
	std::string error = "";
	std::string from = "";
};
