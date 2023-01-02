

#include "PlatformUtils.h"
#include "UtilityCore.h"

#ifdef HZR_PLATFORM_IOS

void PlatformUtils::Messagebox(const char* title, const char* description)
{
    
}

uint64_t PlatformUtils::GetMemoryUsage()
{
    return 0;
}

bool PlatformUtils::HasEnvVariable(const std::string& key)
{
    return false;

}
std::string PlatformUtils::GetEnvVariable(const std::string& key)
{
    return "";
}
bool PlatformUtils::SetEnvVariable(const std::string& key, const std::string& value)
{
    return false;

}
int PlatformUtils::SystemCall(const std::string& command)
{
    return -1;
}
#endif
