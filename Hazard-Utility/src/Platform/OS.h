#pragma once

#include <filesystem>

class OS
{
public:
	static int SysCall(const char* command);
	static void* BackgroundProcess(const char* path, const char* arguments);
	static void WaitForProcess(void* handle);

	static bool HasEnv(const char* key);
	static const char* GetEnv(const char* key);
	static bool SetEnv(const char* key, const char* value);

	static void Dialog(const char* title, const char* description);
	
	static bool ShowLocation(const std::filesystem::path& path);
	static bool OpenDirectory(const std::filesystem::path& path);
	static bool OpenInDefault(const std::filesystem::path& path);
};