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

#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Core/HazardRuntimeError.h"
#include "Hazard/Core/Timer.h"
#include "Hazard/Logging/Logger.h"
#include "Hazard/Instrumentor.h"

#include "Hazard/File/File.h";

#include "Hazard/Events/Event.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/Events/KeyEvent.h"
#include "Hazard/Events/MouseEvent.h"

#include "Hazard/Math/Time.h"
#include "Hazard/Math/Math.h"
#include "Hazard/Math/Random.h"

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_SIMD_AVX2

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

//#define HZR_PROFILE

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define HZR_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define HZR_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define HZR_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define HZR_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define HZR_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define HZR_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define HZR_FUNC_SIG __func__
#else
#define HZR_FUNC_SIG "HZ_FUNC_SIG unknown!"
#endif

#define HZR_THROW(x, y) throw HazardRuntimeError(x, "\nFrom: " HZR_FUNC_SIG)

#define HZR_PROFILE 0

#if HZR_PROFILE 
	#define HZR_PROFILE_SESSION_BEGIN(x, y)			::Hazard::Instrumentor::Get().BeginSession(x, y)
	#define HZR_PROFILE_SESSION_END()				::Hazard::Instrumentor::Get().EndSession();
	#define HZR_PROFILE_SCOPE_LINE2(name, line)		constexpr auto fixedName##line = ::Hazard::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
													::Hazard::InstrumentationTimer timer##line(fixedName##line.Data)
	#define HZR_PROFILE_SCOPE_LINE(name, line)		HZR_PROFILE_SCOPE_LINE2(name, line)
	#define HZR_PROFILE_SCOPE(name)					HZR_PROFILE_SCOPE_LINE(name, __LINE__)
	#define HZR_PROFILE_FUNCTION()					HZR_PROFILE_SCOPE(HZR_FUNC_SIG)
#else
	#define HZR_PROFILE_SESSION_BEGIN(x, y)
	#define HZR_PROFILE_SESSION_END()
	#define HZR_PROFILE_SCOPE_LINE2(name, line)
	#define HZR_PROFILE_SCOPE_LINE(name, line)
	#define HZR_PROFILE_SCOPE(name)
	#define HZR_PROFILE_FUNCTION()
#endif


#if defined(HZR_DEBUG) || defined(HZR_GAME_ONLY)
	//Core logging macros
	#define HZR_CORE_INFO(...)				::Hazard::Logging::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define HZR_CORE_WARN(...)				::Hazard::Logging::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define HZR_CORE_ERROR(...)	 			::Hazard::Logging::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define HZR_CORE_FATAL(...)				::Hazard::Logging::Logger::GetCoreLogger()->critical(__VA_ARGS__)
	#define HZR_CORE_TRACE(...)				::Hazard::Logging::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define HZR_CORE_ASSERT(success, ...)	 if(!(success)) { HZR_CORE_ERROR(__VA_ARGS__); assert(false); }

	//Client logging macros
	#define HZR_INFO(...)					::Hazard::Logging::Logger::GetClientLogger()->info(__VA_ARGS__)
	#define HZR_WARN(...)					::Hazard::Logging::Logger::GetClientLogger()->warn(__VA_ARGS__)
	#define HZR_ERROR(...)					::Hazard::Logging::Logger::GetClientLogger()->error(__VA_ARGS__)
	#define HZR_FATAL(...)					::Hazard::Logging::Logger::GetClientLogger()->critical(__VA_ARGS__)
	#define HZR_TRACE(...)					::Hazard::Logging::Logger::GetClientLogger()->trace(__VA_ARGS__)
	#define HZR_ASSERT(success, ...)		if(!(success)) { HZR_CORE_ERROR(__VA_ARGS__); assert(false); }
	
#else

//Core logging macros
#define HZR_CORE_TRACE(...)
#define HZR_CORE_INFO(...)
#define HZR_CORE_WARN(...)
#define HZR_CORE_ERROR(...)
#define HZR_CORE_FATAL(...)
#define HZR_CORE_ASSERT(c, ...)

//Client logging macros
#define HZR_TRACE(...)
#define HZR_INFO(...)
#define HZR_WARN(...)
#define HZR_ERROR(...)
#define HZR_FATAL(...)
#define HZR_ASSERT(status, ...)

#endif // !

