#pragma once

#include <iostream>
#include <vector>
#include <filesystem>

using std::filesystem::directory_iterator;

namespace Hazard::Utility {

	struct FolderData {
		std::string path;
		std::vector<std::filesystem::directory_entry> folders;
		std::vector<std::filesystem::directory_entry> files;
	};

	class File {
	public:
		static std::string OpenFileDialog(const std::string& filters);
		static std::string SaveFile(const std::string& filters);

		static std::string SaveFolderDialog();

		static std::string ReadFile(const std::string& file);
		static std::string GetFileAbsolutePath(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static FolderData GetFolderFiles(const std::string& folder);

		static std::vector<char> ReadBinaryFile(const char* path);
		static bool Exists(const char* file);
	};
}