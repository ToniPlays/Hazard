#include "OS.h"

#ifdef HZR_PLATFORM_MACOS

#include <Profiling/MemoryDiagnostic.h>
#include "File.h"
#include "Directory.h"

#include <iostream>
#include <spawn.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <stdio.h>
#include <spdlog/fmt/fmt.h>

int OS::SysCall(const char* command)
{
    return system(command);
}
std::string OS::BackgroundProcess(const char* path, const char* arguments)
{
    std::string command = fmt::format("{} {}", path, arguments);
    std::array<char, 128> buffer;
    std::stringstream ss;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        std::cerr << "Could not run command" << std::endl;
        return nullptr;
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        ss << buffer.data();
    }

    pclose(pipe);
    return ss.str();
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

    SysCall(fmt::format("open {}", path.string()).c_str());
    
    return true;
}
bool OS::OpenInDefault(const std::filesystem::path& path)
{
    auto abs = File::GetFileAbsolutePath(path);
    if (!File::Exists(path)) return false;

    //TODO: Fix
    return false;// SysCall((std::string("explorer.exe ") + abs.string()).c_str()) == 0;
}

#endif
