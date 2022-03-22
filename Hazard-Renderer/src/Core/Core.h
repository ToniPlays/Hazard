#pragma once

#ifdef HZR_PLATFORM_WINDOWS
		#define HZR_INCLUDE_OPENGL
		#define HZR_INCLUDE_VULKAN
#elif defined HZR_PLATFORM_MACOS
	#define HZR_INCLUDE_METAL
#endif

#include "Ref.h"
#include "UtilityCore.h"


#define THROW_EXCEPT(x) throw std::exception("[Hazard-Renderer]: " x)
#define ASSERT(x, m) if(!(x)) THROW_EXCEPT(m)
#define BIT(x) (1 << x)