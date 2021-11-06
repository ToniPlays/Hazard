#pragma once


//Platform selection
#ifdef HZR_PLATFORM_WINDOWS
	#define HZR_PLATFORM "Windows"
		#if _WIN64
			#define HAZARD_SYSTEM_BIT "x64"
		#elif _WIN32
			#define HAZARD_SYSTEM_BIT "x86"
		#endif

#else 
	#error Hazard only supports windows!
#endif // HRD_PLATFORM_WINDOWS


//Hazard BUILD_VERSION
#if HZR_DEBUG
	#define HZR_BUILD_VERSION "Debug"
#elif defined HZR_DIST
	#define HZR_BUILD_VERSION "Distribution"
#else
	#define HZR_BUILD_VERSION "Release"
#endif // HZR_DEBUG


#define BIT(x) (1 << x)
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)
#define PATH_TO(x) FileSystem::getPath


#include <iostream>
#include <vector>
#include <unordered_map>

#include "Hazard/Events/Event.h"
#include "Ref.h"

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
using byte = uint8_t;
