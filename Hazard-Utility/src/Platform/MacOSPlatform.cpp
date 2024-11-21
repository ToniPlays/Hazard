#include "OS.h"

#ifdef HZR_PLATFORM_MACOS

#include <Profiling/MemoryDiagnostic.h>
#include "File.h"
#include "Directory.h"

#include <spawn.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>

extern char** environ;

int OS::SysCall(const char* command)
{
    return system(command);
}
void* OS::BackgroundProcess(const char* path, const char* arguments)
{
    pid_t pid;
    posix_spawnattr_t attr;
    
    std::istringstream iss(arguments);
    std::vector<std::string> tokens;
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    
    std::vector<char*> argv;
    for (auto& t : tokens)
        argv.push_back(t.data());
    
    posix_spawnattr_init(&attr);
    posix_spawnattr_setflags(&attr, POSIX_SPAWN_SETSID);
    int status = posix_spawnp(&pid, path, nullptr, &attr, argv.data(), environ);
    
    posix_spawnattr_destroy(&attr);
    
    return (void*)pid;
    
}
void OS::WaitForProcess(void* handle)
{
    
}
bool OS::HasEnv(const char* key)
{
    return GetEnv(key) != nullptr;
}
const char* OS::GetEnv(const char* key)
{
    return getenv(key);
}
bool OS::SetEnv(const char* key, const char* value)
{
    return setenv(key, value, 1) == 0;
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
