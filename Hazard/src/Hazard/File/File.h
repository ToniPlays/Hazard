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
		static std::string OpenFileDialog(const char* filters);
		static std::string SaveFolderDialog();
		static std::string SaveFile(const char* filters);

		static bool Exists(const std::string& file);
		static bool DirectoryExists(const std::string& dir);
		static std::vector<char> ReadBinaryFile(const std::string& path);
		static std::vector<uint32_t> ReadBinaryFileUint32(const std::string& path);
		static std::string ReadFile(const std::string& file);

		static void WriteFile(const std::string& dest, const std::string& source);
		static void WriteBinaryFile(const std::string& dest, std::vector<uint32_t> data);
		static bool CopyFileTo(const std::string& source, const std::string& dest);

		static std::string GetFileAbsolutePath(const std::string& file);
		static std::string GetDirectoryOf(const std::string& file);
		static std::string GetName(const std::string& file);
		static std::string GetNameNoExt(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static bool CreateDir(const std::string& dir);

		static FolderData GetFolderFiles(const std::string& folder);
	};
}