#pragma once

#include <filesystem>

class FileCache
{
public:
	FileCache(const std::filesystem::path& path) : m_Path(path)
	{
		if (!File::DirectoryExists(path))
			File::CreateDir(path);
	}

	const std::filesystem::path& GetCachePath() const { return m_Path; }

	bool HasFile(const std::filesystem::path& file) 
	{ 
		return File::Exists(m_Path / file) && !File::IsDirectory(m_Path / file);
	}

private:
	std::filesystem::path m_Path;
};