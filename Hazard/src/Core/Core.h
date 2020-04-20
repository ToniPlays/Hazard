#pragma once

#ifdef HZR_PLATFORM_WINDOWS

	#ifdef HZR_BUILD_DLL
		#define HAZARD_API __declspec(dllexport)
	#else
		#define HAZARD_API __declspec(dllimport)
	#endif
#else 
	#error Hazard supports only windows!
#endif // HRD_PLATFORM_WINDOWS

#define BIT(x) (1 << x)
#define GLFW_INCLUDE_NONE