#include "OS.h"

#ifdef HZR_PLATFORM_MACOS

#include <Profiling/MemoryDiagnostic.h>
#include "File.h"
#include "Directory.h"

int OS::SysCall(const char* command)
{
    return system(command);
}
void* OS::BackgroundProcess(const char* path, const char* arguments)
{
    return nullptr;
}
void OS::WaitForProcess(void* handle)
{
    
}
bool OS::HasEnv(const char* key)
{
    return false;
}
const char* OS::GetEnv(const char* key)
{
    return "";
}
bool OS::SetEnv(const char* key, const char* value)
{
    return false;
}
void OS::Dialog(const char* title, const char* description)
{
    
}

bool OS::ShowLocation(const std::filesystem::path& path)
{
    auto abs = File::GetFileAbsolutePath(path);
    if (!File::Exists(abs)) return false;

    //TODO: Fix
    std::string cmd = "explorer.exe /select,\"" + abs.string() + "\"";
    return false;
}
bool OS::OpenDirectory(const std::filesystem::path& path)
{
    auto abs = File::GetFileAbsolutePath(path);
    if (!File::Exists(path)) return false;

    return false;
}
bool OS::OpenInDefault(const std::filesystem::path& path)
{
    auto abs = File::GetFileAbsolutePath(path);
    if (!File::Exists(path)) return false;

    //TODO: Fix
    return false;// SysCall((std::string("explorer.exe ") + abs.string()).c_str()) == 0;
}

#endif
