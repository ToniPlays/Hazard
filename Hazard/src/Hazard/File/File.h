#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

#include <string>
#include <cstdlib>

using std::filesystem::directory_iterator;

namespace Hazard {

	struct FolderData {
		std::string path;
		std::vector<std::filesystem::directory_entry> folders;
		std::vector<std::filesystem::directory_entry> files;
	};

	class File {
	public:

		static std::string OpenFileDialog(const std::string& filters);
		static std::string SaveFolderDialog();
		static std::string SaveFile(const std::string& filters);

		static bool Exists(const char* file);
		static std::vector<char> ReadBinaryFile(const char* path);
		static std::string ReadFile(const std::string& file);

		static std::string GetFileAbsolutePath(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static FolderData GetFolderFiles(const std::string& folder);
	};
}