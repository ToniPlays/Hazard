#pragma once

#include <string>

namespace Hazard
{
	class PlatformUtils
	{
	public:
		static void Messagebox(const char* title, const char* description);
		static std::string OpenFileDialog(const char* filters);
		static std::string OpenFolderDialog(const std::string& title);
		static std::string SaveFileDialog(const char* filters);

		static bool HasEnvVariable(const std::string& key);
		static std::string GetEnvVariable(const std::string& key);
		static bool SetEnvVariable(const std::string& key, const std::string& value);
	};
}
