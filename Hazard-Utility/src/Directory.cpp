#include "Directory.h"

#include "portable-file-dialogs.h"
#include <File.h>

std::string Directory::OpenFolderDialog(const std::string& title)
{
	auto f = pfd::select_folder(title, "", pfd::opt::none);
	if (f.result() == "") return "";
	return f.result();
}

bool Directory::Exists(const std::filesystem::path& dir)
{
	return std::filesystem::exists(dir);
}
bool Directory::Create(const std::filesystem::path& dir)
{
	return std::filesystem::create_directories(dir);
}

bool Directory::Rename(const std::filesystem::path& folder, const std::string& newName)
{
	std::filesystem::path newPath = folder.parent_path();
	std::filesystem::rename(folder, newPath / newName);
	return true;
}

std::vector<std::filesystem::path> Directory::GetAllInDirectory(const std::filesystem::path& path, bool recursive)
{
	std::vector<std::filesystem::path> result;
	if (!File::Exists(path)) return result;

	if (recursive)
	{
		for (const auto& iter : std::filesystem::recursive_directory_iterator(path))
			result.emplace_back(iter);
		return result;
	}

	for (const auto& iter : std::filesystem::directory_iterator(path))
		result.emplace_back(iter);

	return result;
}