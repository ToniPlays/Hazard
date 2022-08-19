#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include "Color.h"
#include "HazardRuntimeError.h"

#define HZR_PROFILE 1
#define HZR_OPTICK 1


#if HZR_OPTICK
	#include <optick.h>
#endif

#define THROW_EXCEPT(x) throw std::runtime_error(std::string("[Hazard-Utility]: " x))
#ifdef HZR_DEBUG
#define HZR_ASSERT(x, m) if(!(x)) { std::cout << (m) << std::endl; assert(false); }
#else
#define HZR_ASSERT(x, m) (x)
#endif

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


#if HZR_PROFILE 
#if !HZR_OPTICK
#define HZR_PROFILE_SESSION_BEGIN(x, y)			::Instrumentor::Get().BeginSession(x, y)
#define HZR_PROFILE_SESSION_END()				::Instrumentor::Get().EndSession();
#define HZR_PROFILE_SCOPE_LINE2(name, line)		constexpr auto fixedName##line = ::InstrumentorUtils::CleanupOutputString(name, "__cdecl ");\
														::InstrumentationTimer timer##line(fixedName##line.Data)
#define HZR_PROFILE_SCOPE_LINE(name, line)		HZR_PROFILE_SCOPE_LINE2(name, line)
#define HZR_PROFILE_SCOPE(name, ...)			HZR_PROFILE_SCOPE_LINE(name, __LINE__)
#define HZR_PROFILE_FUNCTION(...)				HZR_PROFILE_SCOPE(HZR_FUNC_SIG)
#define HZR_PROFILE_FRAME(...)					HZR_PROFILE_FUNCTION("Frame")
#else

#define HZR_PROFILE_SESSION_BEGIN(x, y)			
#define HZR_PROFILE_SESSION_END()				
#define HZR_PROFILE_SCOPE_LINE2(name, line)		

#define HZR_PROFILE_SCOPE_LINE(name, line)		
#define HZR_PROFILE_SCOPE(name, ...)			OPTICK_EVENT_DYNAMIC(name, __VA_ARGS__)
#define HZR_PROFILE_FUNCTION(...)				OPTICK_EVENT(__VA_ARGS__)	
#define HZR_PROFILE_FRAME(...)					OPTICK_FRAME(__VA_ARGS__)
#endif

#else
#define HZR_PROFILE_SESSION_BEGIN(x, y)
#define HZR_PROFILE_SESSION_END()
#define HZR_PROFILE_SCOPE_LINE2(name, line)
#define HZR_PROFILE_SCOPE_LINE(name, line)
#define HZR_PROFILE_SCOPE(name, ...)
#define HZR_PROFILE_FUNCTION(...)
#define HZR_PROFILE_FRAME(...)
#endif


#define HZR_THROW(x) throw HazardRuntimeError(x, HZR_FUNC_SIG)

#define BIT(x) (1 << (x))

template<typename T>
using Scope = std::unique_ptr<T>;
template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
using byte = uint8_t;
