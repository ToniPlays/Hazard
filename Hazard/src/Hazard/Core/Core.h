#pragma once

#ifndef HZR_CUSTOM_ENTRY
    //Platform selection
    #ifdef HZR_PLATFORM_WINDOWS
        #define HZR_PLATFORM "Windows"
            #if _WIN64
                #define HAZARD_SYSTEM_BIT "x64"
            #elif _WIN32
                #define HAZARD_SYSTEM_BIT "x86"
            #endif
    #elif defined(HZR_PLATFORM_MACOS)
        #define HZR_PLATFORM "macOS"
        #define HAZARD_SYSTEM_BIT "arm64"
    #else
        #error Unsupported platform!
    #endif // HZR_PLATFORM_WINDOWS

#endif

//Hazard BUILD_VERSION
#if HZR_DEBUG
	#define HZR_BUILD_VERSION "Debug"
#elif defined HZR_DIST
	#define HZR_BUILD_VERSION "Distribution"
#else
	#define HZR_BUILD_VERSION "Release"
#endif // HZR_DEBUG


#define BIT(x) (uint32_t)(1 << (x))
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)
#define CHECK_BIT(val, bit) val & (1 << bit);


#include <iostream>
#include <vector>
#include <unordered_map>

#include "Hazard/Events/Event.h"

