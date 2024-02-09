#pragma once

#include <filesystem>
#include <Directory.h>

class FileCache
{
public:
	FileCache(const std::filesystem::path& path) : m_Path(path)
	{
		if (!Directory::Exists(path))
			Directory::Create(path);
	}

	const std::filesystem::path& GetCachePath() const { return m_Path; }

	bool HasFile(const std::filesystem::path& file) 
	{ 
		return File::Exists(m_Path / file) && !File::IsDirectory(m_Path / file);
	}
	std::filesystem::path Get(const std::string& file)
	{
		return GetCachePath() / file;
	}
	bool WriteFile(const std::filesystem::path& file, void* data, uint64_t size)
	{
		auto filePath = m_Path / file;
		Directory::Create(filePath.parent_path());
		return File::WriteBinaryFile(filePath, data, size);
	}

private:
	std::filesystem::path m_Path;
};