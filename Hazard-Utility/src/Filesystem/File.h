#pragma once

#include "UtilityCore.h"
#include "Ref.h"
#include <fstream>
#include <cstdlib>

using std::filesystem::directory_iterator;

enum class CacheStatus
{
	None,
	Exists,
	Outdated
};

enum class CopyOptions 
{
	None = (int)std::filesystem::copy_options::none,
	OverwriteExisting = (int)std::filesystem::copy_options::overwrite_existing,
	SkipExisting = (int)std::filesystem::copy_options::skip_existing,
	UpdateExisting = (int)std::filesystem::copy_options::update_existing,
	Recursive = (int)std::filesystem::copy_options::recursive,
	DirectoriesOnly = (int)std::filesystem::copy_options::directories_only
};

class CachedBuffer;

class File 
{
public:
	static std::string OpenFileDialog();
	static std::string OpenFileDialog(const std::vector<std::string>& filters);
	static std::string SaveFile(const std::vector<std::string>& filters, const std::filesystem::path& defaultPath = "");

	static bool Exists(const std::filesystem::path& path);
	static bool IsDirectory(const std::filesystem::path& path);
	static std::filesystem::path FindAvailableName(const std::filesystem::path& directory, const std::string& name, const std::string& extension);
    static std::filesystem::path Relative(const std::filesystem::path& base, const std::filesystem::path& path);

	static Ref<CachedBuffer> ReadBinaryFile(const std::filesystem::path& path);
	static std::string ReadFile(const std::filesystem::path& file);

	static bool WriteBinaryFile(const std::filesystem::path& path, void* data, uint64_t size);
	static bool CopyFileTo(const std::filesystem::path& source, const std::filesystem::path& dest);
	static bool IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo);

	static bool WriteFile(const std::filesystem::path& file, const std::string& content = "");
	static bool Move(const std::filesystem::path& src, const std::filesystem::path& dst);

	static std::filesystem::path GetFileAbsolutePath(const std::filesystem::path& file);
	static std::filesystem::path GetDirectoryOf(const std::filesystem::path& file);
	static std::string GetName(const std::filesystem::path& file);
	static std::string GetNameNoExt(const std::filesystem::path& file);
	static std::filesystem::path GetPathNoExt(const std::filesystem::path& file);
	static std::string GetFileExtension(const std::filesystem::path& file);

	static void Copy(const std::filesystem::path& source, const std::filesystem::path& dest, CopyOptions options);
};
