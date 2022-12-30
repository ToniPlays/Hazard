#pragma once

#ifdef HZR_PLATFORM_WINDOWS
#define LBL_SHOW_IN_EXPLORER "Show in Explorer"
#elif HZR_PLATFORM_MACOS
#define LBL_SHOW_IN_EXPLORER "Show in Finder"
#else

#define LBL_SHOW_IN_EXPLORER "Show in ---"

#endif
