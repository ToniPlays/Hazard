#pragma once

#include <string>

class PlatformUtils
{
public:
	static void Messagebox(const char* title, const char* description);

	static bool HasEnvVariable(const std::string& key);
	static std::string GetEnvVariable(const std::string& key);
	static bool SetEnvVariable(const std::string& key, const std::string& value);
};
