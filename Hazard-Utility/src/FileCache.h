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

private:
	std::filesystem::path m_Path;
};