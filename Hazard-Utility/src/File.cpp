
#include "UtilityCore.h"
#include "File.h"
#include "Utility/StringUtil.h"
#include "portable-file-dialogs.h"
#include <format>

#include "Platform/PlatformUtils.h"
#include "spdlog/fmt/fmt.h"

#ifdef HZR_PLATFORM_WINDOWS
#include <shlobj.h>
#include <cstdlib>
#include <Windows.h>
#endif

#ifdef HZR_PLATFORM_IOS
#include <CoreFoundation/CFBundle.h>
#endif
#include <Directory.h>

std::string File::OpenFileDialog()
{
	auto f = pfd::open_file("Open file", "");
	if (f.result().size() == 0) return "";
	return f.result()[0];
}

std::string File::OpenFileDialog(const std::vector<std::string>& filters)
{
	auto f = pfd::open_file("Open file", "", filters, pfd::opt::none);
	if (f.result().size() == 0) return "";
	return f.result()[0];
}
std::string File::SaveFile(const std::vector<std::string>& filters, const std::filesystem::path& defaultPath)
{
	auto f = pfd::save_file("Save file", defaultPath.string(), filters, true);
	return f.result();
}

bool File::Exists(const std::filesystem::path& path)
{
#ifdef HZR_PLATFORM_IOS
	CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	char resourcePath[PATH_MAX];
	if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)resourcePath,
										 PATH_MAX))
	{
		bool exists = resourceURL != NULL;
		if (exists)
		{
			CFRelease(resourceURL);
		}
		return exists;
	}
#else
	return std::filesystem::exists(path);
#endif
}
bool File::IsDirectory(const std::filesystem::path& path)
{
	return std::filesystem::is_directory(path);
}

std::filesystem::path File::FindAvailableName(const std::filesystem::path& directory, const std::string& name, const std::string& extension)
{
	std::filesystem::path path = directory / fmt::format("{}{}.{}", name, "", extension);
	uint64_t index = 0;

	while (File::Exists(path))
	{
		index++;
		path = directory / fmt::format("{}{}.{}", name, index, extension);
	}
	return path;
}

bool File::WriteBinaryFile(const std::filesystem::path& path, void* data, uint64_t size)
{
	if (size == 0) return false;

	std::ofstream out(path, std::ios::out | std::ios::binary);

	if (out.is_open())
	{
		out.write((char*)data, size);
		out.flush();
		out.close();
		return true;
	}
	return false;
}
bool File::CopyFileTo(const std::filesystem::path& source, const std::filesystem::path& dest)
{
	std::filesystem::path destFolder = GetDirectoryOf(dest);

	if (!Directory::Exists(destFolder))
		Directory::Create(destFolder);

	return std::filesystem::copy_file(source, dest);
}
bool File::IsNewerThan(const std::filesystem::path& file, const std::filesystem::path& compareTo)
{
	if (!Exists(file)) return false;
	if (!Exists(compareTo)) return true;

	auto fTime = std::filesystem::last_write_time(file);
	auto sTime = std::filesystem::last_write_time(compareTo);

	return fTime > sTime;
}

bool File::WriteFile(const std::filesystem::path& file, const std::string& content)
{
	std::ofstream f(file);
	f << content;
	f.close();
	return true;
}

bool File::Move(const std::filesystem::path& src, const std::filesystem::path& dst)
{
	if (!File::Exists(dst))
		return false;

	std::filesystem::rename(std::filesystem::canonical(src), std::filesystem::canonical(dst));
	return true;
}

std::string File::ReadFile(const std::filesystem::path& file)
{
	std::string result;
	std::ifstream ifs(File::GetFileAbsolutePath(file), std::ios::in | std::ios::binary | std::ios::ate);
	HZR_ASSERT(ifs.is_open(), "Cannot open file: " + GetFileAbsolutePath(file).string());

	uint64_t size = ifs.tellg();
	result.resize(size);
	ifs.seekg(0, std::ios::beg);
	ifs.read(&result[0], size);

	return result;
}
CachedBuffer File::ReadBinaryFile(const std::filesystem::path& path)
{
	std::ifstream stream(path, std::ios::binary | std::ios::ate);

	if (!stream.is_open())
		return CachedBuffer(0);

	auto size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	if (size == 0) return CachedBuffer();

	CachedBuffer result((uint32_t)size);

	stream.read((char*)result.GetData(), size);
	stream.close();
	return result;
}

std::filesystem::path File::GetFileAbsolutePath(const std::filesystem::path& file)
{
#ifdef HZR_PLATFORM_IOS
	CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
	char resourcePath[PATH_MAX];
	if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)resourcePath,
										 PATH_MAX))
	{
		bool exists = resourceURL != NULL;
		if (exists)
		{
			CFRelease(resourceURL);
		}
		return resourcePath;
	}
#else
	return std::filesystem::absolute(file);
#endif
}
std::filesystem::path File::GetDirectoryOf(const std::filesystem::path& file)
{
	return file.parent_path();
}
std::string File::GetName(const std::filesystem::path& file)
{
	std::filesystem::path path(file);
	return path.filename().string();
}
std::string File::GetNameNoExt(const std::filesystem::path& file)
{
	std::string name = GetName(file);
	return name.substr(0, name.find_last_of('.'));
}
std::filesystem::path File::GetPathNoExt(const std::filesystem::path& file)
{
	return file.string().substr(0, file.string().find_last_of('.'));
}
std::string File::GetFileExtension(const std::filesystem::path& file)
{
	return file.extension().string();
}

void File::Copy(const std::filesystem::path& source, const std::filesystem::path& dest, CopyOptions options)
{
	std::filesystem::copy(source, dest, (std::filesystem::copy_options)options);
}

bool File::OpenInExplorer(const std::filesystem::path& path)
{
	auto abs = GetFileAbsolutePath(path);
	if (!Exists(abs)) return false;

	std::string cmd = "explorer.exe /select,\"" + abs.string() + "\"";
	return SystemCall(cmd) == 0;
}
bool File::OpenDirectoryInExplorer(const std::filesystem::path& path)
{
	auto abs = GetFileAbsolutePath(path);
	if (!Exists(path)) return false;
#ifdef HZR_PLATFORM_WINDOWS

	ShellExecute(NULL, L"explore", abs.c_str(), NULL, NULL, SW_SHOWNORMAL);
	return true;
#else
	SystemCall((std::string("open ") + abs.string()).c_str());
	return true;
#endif
}

bool File::OpenInDefaultApp(const std::filesystem::path& file)
{
	auto abs = GetFileAbsolutePath(file);
	if (!Exists(file)) return false;

#ifdef HZR_PLATFORM_WINDOWS
	SystemCall((std::string("explorer.exe ") + abs.string()).c_str());
	return true;
#else
	SystemCall((std::string("open ") + abs.string()).c_str());
	return true;
#endif
}

int File::SystemCall(const std::string& command)
{
	return PlatformUtils::SystemCall(command);
}

int File::CreateSubprocess(const std::string& path, const std::string& arguments, bool background)
{
#ifdef HZR_PLATFORM_WINDOWS
	STARTUPINFOA si;
	PROCESS_INFORMATION pi;

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	int id = CreateProcessA(
		path.c_str(),
		(LPSTR)arguments.c_str(),
		NULL,
		NULL,
		FALSE,
		background ? CREATE_NO_WINDOW : CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi
	);

	return id;
#else
	return 0;
#endif
}
void File::WaitForSubprocess(void* id)
{
#ifdef HZR_PLATFORM_WINDOWS
	WaitForSingleObject(id, 0);
#else

#endif
}

bool File::HasEnvinronmentVar(const std::string& key)
{
	return PlatformUtils::HasEnvVariable(key);
}
std::string File::GetEnvironmentVar(const std::string& key)
{
	return PlatformUtils::GetEnvVariable(key);
}
bool File::SetEnvironmentVar(const std::string& key, const std::string& value)
{
	return PlatformUtils::SetEnvVariable(key, value);
}
