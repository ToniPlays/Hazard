#pragma once

#include "Buffer.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <string>
#include <cstdlib>

using std::filesystem::directory_iterator;

enum class CopyOptions {
	None = (int)std::filesystem::copy_options::none,
	OverwriteExisting = (int)std::filesystem::copy_options::overwrite_existing,
	SkipExisting = (int)std::filesystem::copy_options::skip_existing,
	UpdateExisting = (int)std::filesystem::copy_options::update_existing,
	Recursive = (int)std::filesystem::copy_options::recursive,
	DirectoriesOnly = (int)std::filesystem::copy_options::directories_only
};

class File {
public:
	static std::string OpenFileDialog();
	static std::string OpenFileDialog(const std::vector<std::string>& filters);
	static std::string OpenFolderDialog(const std::string& title = "Select folder");
	static std::string SaveFile(const char* filters);

	static bool Exists(const std::filesystem::path& path);
	static bool DirectoryExists(const std::filesystem::path& dir);
	static bool IsDirectory(const std::filesystem::path& path);

	static std::vector<char> ReadBinaryFileChar(const std::filesystem::path& path);
	static Buffer ReadBinaryFile(const std::filesystem::path& path);
	static bool ReadBinaryFileUint32(const std::filesystem::path& path, std::vector<uint32_t>& buffer);
	static std::string ReadFile(const std::filesystem::path& file);

	static void WriteFile(const std::filesystem::path& dest, const std::filesystem::path& source);
	static bool WriteBinaryFile(const std::filesystem::path& path, std::vector<uint32_t> data);
	static bool WriteBinaryFile(const std::filesystem::path& path, void* data, size_t size);
	static bool CopyFileTo(const std::filesystem::path& source, const std::filesystem::path& dest);
	static bool IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo);

	static bool NewFile(const std::filesystem::path& file, const std::string& content = "");


	static std::filesystem::path GetFileAbsolutePath(const std::filesystem::path& file);
	static std::string GetDirectoryOf(const std::filesystem::path& file);
	static std::string GetName(const std::filesystem::path& file);
	static std::string GetNameNoExt(const std::filesystem::path& file);
	static std::string GetPathNoExt(const std::filesystem::path& file);
	static std::string GetFileExtension(const std::filesystem::path& file);

	static std::vector<std::filesystem::path> GetAllInDirectory(const std::filesystem::path& path);

	static bool CreateDir(const std::filesystem::path& dir);
	static void Copy(const std::filesystem::path& source, const std::filesystem::path& dest, CopyOptions options);

	static bool OpenInExplorer(const std::filesystem::path& file);
	static bool OpenDirectoryInExplorer(const std::filesystem::path& path);

	static int SystemCall(const std::string& command);
	static int CreateSubprocess(const std::string& path, const std::string& arguments);
	static void WaitForSubprocess(int id);


	static bool HasEnvinronmentVar(const std::string& key);
	static std::string GetEnvironmentVar(const std::string& key);
	static bool SetEnvironmentVar(const std::string& key, const std::string& value);
};
