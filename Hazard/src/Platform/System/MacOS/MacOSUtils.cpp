#include <hzrpch.h>

#ifdef HZR_PLATFORM_MACOS

#include "Hazard/Core/PlatformUtils.h"

namespace Hazard
{
    void PlatformUtils::Messagebox(const char* title, const char* description)
    {
       
    }
    std::string PlatformUtils::OpenFileDialog(const char* filters)
    {
        return "";
    }
    std::string PlatformUtils::SaveFileDialog(const char* filters)
    {
        
        return "";
    }
    std::string PlatformUtils::SaveFolderDialog()
    {
        return "";
    }
}
#endif
