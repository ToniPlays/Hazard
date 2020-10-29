#pragma once

#ifdef HZR_PLATFORM_WINDOWS
	#if HZR_DYNAMIC_LINK
		#ifdef HZR_BUILD_DLL
			#define HAZARD_API __declspec(dllexport)
		#else
			#define HAZARD_API __declspec(dllimport)
		#endif
	#else
		#define HAZARD_API
	#endif
#else 
	#error Hazard supports only windows!
#endif // HRD_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
#define BIND_EVENT(x) std::bind(&x, this, std::placeholders::_1)

#if HZR_DEBUG
	#define HZR_BUILD_VERSION "Debug"
#elif defined HZR_DIST
	#define HZR_BUILD_VERSION "Distribution"
#elif defined(HZR_GAME_ONLY)
	#define HZR_BUILD_VERSION "Game"
#else
	#define HZR_BUILD_VERSION "Release"
#endif // HZR_DEBUG
