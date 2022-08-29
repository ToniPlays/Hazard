#pragma once

#ifdef HZR_PLATFORM_WINDOWS
    #define HZR_INCLUDE_OPENGL
	#define HZR_INCLUDE_VULKAN
#elif defined HZR_PLATFORM_MACOS
	#define HZR_INCLUDE_METAL
#else
#pragma error Unsupported platform apparently
#endif

#ifndef HZR_RESOURCE_HANDLER
#define HZR_RESOURCE_HANDLER RefCount
#endif

#include "Ref.h"
#include "UtilityCore.h"
