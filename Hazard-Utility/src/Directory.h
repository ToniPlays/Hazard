#pragma once

#include <string>
#include <filesystem>

class Directory
{
public:
	static std::string OpenFolderDialog(const std::string& title = "Select folder");
	static bool Exists(const std::filesystem::path& dir);
	static bool Create(const std::filesystem::path& dir);
	static bool Rename(const std::filesystem::path& folder, const std::string& newName);
	static std::vector<std::filesystem::path> GetAllInDirectory(const std::filesystem::path& path, bool recursive = false);
};