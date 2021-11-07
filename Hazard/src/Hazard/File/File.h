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

		static bool Exists(const std::filesystem::path& path);
		static bool DirectoryExists(const std::filesystem::path& dir);

		static std::vector<char> ReadBinaryFile(const std::string& path);
		static uint32_t* ReadBinaryFile(const std::filesystem::path& path, size_t& fileSize);
		static bool ReadBinaryFileUint32(const std::filesystem::path& path, std::vector<uint32_t>& buffer);
		static std::string ReadFile(const std::string& file);

		static void WriteFile(const std::string& dest, const std::string& source);
		static bool WriteBinaryFile(const std::filesystem::path& path, std::vector<uint32_t> data);
		static bool WriteBinaryFile(const std::filesystem::path& path, void* data, size_t size);
		static bool CopyFileTo(const std::string& source, const std::string& dest);
		static bool IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo);

		static std::string GetFileAbsolutePath(const std::string& file);
		static std::string GetDirectoryOf(const std::string& file);
		static std::string GetName(const std::string& file);
		static std::string GetNameNoExt(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static bool CreateDir(const std::filesystem::path& dir);

		static FolderData GetFolderFiles(const std::string& folder);
	};
}