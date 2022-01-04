#include <hzrpch.h>

#ifdef HZR_PLATFORM_MACOS

#include "Hazard/Core/PlatformUtils.h"
#include "portable_file_dialog.h"

#if _WIN32
#define DEFAULT_PATH "C:\\"
#else
#define DEFAULT_PATH "/tmp"
#endif

namespace Hazard
{
    void PlatformUtils::Messagebox(const char* title, const char* description)
    {
        pfd::notify(title, description, pfd::icon::info);
    }
    std::string PlatformUtils::OpenFileDialog(const char* filters)
    {
        auto f = pfd::open_file("Open file", DEFAULT_PATH, { "" }, pfd::opt::none);
        
        return "";
    }
    std::string PlatformUtils::SaveFileDialog(const char* filters)
    {
        
        return "";
    }
    std::string PlatformUtils::SaveFolderDialog()
    {
        auto f = pfd::select_folder("Select folder", DEFAULT_PATH).result();
        return f;
    }
}
#endif
