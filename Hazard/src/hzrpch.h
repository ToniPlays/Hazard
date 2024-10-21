#pragma once
#include <iostream>
#include <random>
#include <sstream>
#include <fstream>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <stdio.h> 
#include <utility>

#include "Hazard/Core/Core.h"
#include "Hazard/Core/HazardLoop.h"
#include "HazardRuntimeError.h"
#include "Hazard/Logging/Logger.h"

#include "Core/Core.h"
#include "File.h"

#include "Event.h"

#include "Hazard/Math/Time.h"
#include "MathCore.h"
#include "Random.h"

#ifdef HZR_PLATFORM_WINDOWS
//#include "optick.h"
#endif
#include <chrono>

using namespace std::chrono_literals;

#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#define HZR_THREAD_DELAY(x) std::this_thread::sleep_for(x);
//Return if false
#define HZR_GUARD(x) if(!(x)) return

#if (defined(HZR_DEBUG))
	//Core logging macros
	#define HZR_CORE_INFO(...)				::Hazard::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define HZR_CORE_WARN(...)				::Hazard::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define HZR_CORE_ERROR(...)	 			::Hazard::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define HZR_CORE_FATAL(...)				::Hazard::Logger::GetCoreLogger()->critical(__VA_ARGS__)
	#define HZR_CORE_TRACE(...)				::Hazard::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define HZR_CORE_ASSERT(success, ...)	 if(!(success)) { HZR_CORE_ERROR(__VA_ARGS__); assert(false); }

	//Client logging macros
	#define HZR_INFO(...)					::Hazard::Logger::GetClientLogger()->info(__VA_ARGS__)
	#define HZR_WARN(...)					::Hazard::Logger::GetClientLogger()->warn(__VA_ARGS__)
	#define HZR_ERROR(...)					::Hazard::Logger::GetClientLogger()->error(__VA_ARGS__)
	#define HZR_FATAL(...)					::Hazard::Logger::GetClientLogger()->critical(__VA_ARGS__)
	#define HZR_TRACE(...)					::Hazard::Logger::GetClientLogger()->trace(__VA_ARGS__)
	#define HZR_ASSERT(success, ...)		if(!(success)) { HZR_CORE_ERROR(__VA_ARGS__); assert(false); }
	
#else

//Core logging macros
#define HZR_CORE_TRACE(...)
#define HZR_CORE_INFO(...)
#define HZR_CORE_WARN(...)
#define HZR_CORE_ERROR(...)
#define HZR_CORE_FATAL(...)
#define HZR_CORE_ASSERT(c, ...) (c)

//Client logging macros
#define HZR_TRACE(...)
#define HZR_INFO(...)
#define HZR_WARN(...)
#define HZR_ERROR(...)
#define HZR_FATAL(...)
#define HZR_ASSERT(status, ...) (status)

#endif // !


#ifdef HZR_PLATFORM_MACOS
#define OPTICK_THREAD(x)
#define OPTICK_SHUTDOWN(x)


#endif
