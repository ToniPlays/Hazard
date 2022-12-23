#pragma once

#include <iostream>
#include <string>

class HazardRuntimeError : public std::exception
{
public:
	HazardRuntimeError(std::string error, std::string from) : error(error), from(from) {};
	const char* what() const noexcept override { return error.c_str(); }
    const std::string& where() { return from; }

private:
	std::string error = "";
	std::string from = "";
};
