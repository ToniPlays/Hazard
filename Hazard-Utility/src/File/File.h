#pragma once

#include "Buffer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <string>
#include <cstdlib>

using std::filesystem::directory_iterator;

namespace HazardUtility {

	enum class CopyOptions { 
		None = (int)std::filesystem::copy_options::none,
		OverwriteExisting = (int)std::filesystem::copy_options::overwrite_existing,
		SkipExisting = (int)std::filesystem::copy_options::skip_existing,
		UpdateExisting = (int)std::filesystem::copy_options::update_existing,
		Recursive = (int)std::filesystem::copy_options::recursive,
		DirectoriesOnly = (int)std::filesystem::copy_options::directories_only
	};

	struct FolderData {
		std::string Path;
		std::vector<std::filesystem::directory_entry> Folders;
		std::vector<std::filesystem::directory_entry> Files;
	};

	class File {
	public:
		static std::string OpenFileDialog(const char* filters);
		static std::string OpenFolderDialog(const std::string& title = "Select folder");
		static std::string SaveFile(const char* filters);

		static bool Exists(const std::filesystem::path& path);
		static bool DirectoryExists(const std::filesystem::path& dir);

		static std::vector<char> ReadBinaryFile(const std::string& path);
		static Buffer ReadBinaryFile(const std::filesystem::path& path);
		static bool ReadBinaryFileUint32(const std::filesystem::path& path, std::vector<uint32_t>& buffer);
		static std::string ReadFile(const std::filesystem::path& file);

		static void WriteFile(const std::string& dest, const std::string& source);
		static bool WriteBinaryFile(const std::filesystem::path& path, std::vector<uint32_t> data);
		static bool WriteBinaryFile(const std::filesystem::path& path, void* data, size_t size);
		static bool CopyFileTo(const std::string& source, const std::string& dest);
		static bool IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo);

		static std::string GetFileAbsolutePath(const std::string& file);
		static std::string GetDirectoryOf(const std::string& file);
		static std::string GetName(const std::string& file);
		static std::string GetNameNoExt(const std::string& file);
		static std::string GetPathNoExt(const std::string& file);
		static std::string GetFileExtension(const std::string& file);

		static bool CreateDir(const std::filesystem::path& dir);
		static void Copy(const std::filesystem::path& source, const std::filesystem::path& dest, CopyOptions options);

		static FolderData GetFolderFiles(const std::string& folder);

		static bool HasEnvinronmentVar(const std::string& key);
		static std::string GetEnvironmentVar(const std::string& key);
		static bool SetEnvironmentVar(const std::string& key, const std::string& value);
	};
}