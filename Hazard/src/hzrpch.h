#pragma once
#include <iostream>
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
#include "Hazard/Logging/Logger.h"

#include "Hazard/Events/Event.h"
#include "Hazard/Events/ApplicationEvent.h"
#include "Hazard/Events/KeyEvent.h"
#include "Hazard/Events/MouseEvent.h"

#include "Hazard/Math/Time.h"

#if defined(HZR_DEBUG) || defined(HZR_GAME_ONLY)
	//Core logging macros
	#define HZR_CORE_INFO(...)		 ::Hazard::Logging::Logger::GetCoreLogger()->info(__VA_ARGS__)
	#define HZR_CORE_WARN(...)		 ::Hazard::Logging::Logger::GetCoreLogger()->warn(__VA_ARGS__)
	#define HZR_CORE_ERROR(...)	 	 ::Hazard::Logging::Logger::GetCoreLogger()->error(__VA_ARGS__)
	#define HZR_CORE_FATAL(...)		 ::Hazard::Logging::Logger::GetCoreLogger()->critical(__VA_ARGS__)
	#define HZR_CORE_TRACE(...)		 ::Hazard::Logging::Logger::GetCoreLogger()->trace(__VA_ARGS__)
	#define HZR_CORE_ASSERT(success, ...)	 if(!success) HZR_CORE_ERROR(__VA_ARGS__)

	//Client logging macros
	#define HZR_INFO(...)			 ::Hazard::Logging::Logger::GetClientLogger()->info(__VA_ARGS__)
	#define HZR_WARN(...)			 ::Hazard::Logging::Logger::GetClientLogger()->warn(__VA_ARGS__)
	#define HZR_ERROR(...)			 ::Hazard::Logging::Logger::GetClientLogger()->error(__VA_ARGS__)
	#define HZR_FATAL(...)			 ::Hazard::Logging::Logger::GetClientLogger()->critical(__VA_ARGS__)
	#define HZR_TRACE(...)			 ::Hazard::Logging::Logger::GetClientLogger()->trace(__VA_ARGS__)
	#define HZR_ASSERT(success, ...) 

	#ifdef HZR_DEBUG

		#define PROFILE_FN()             ::Hazard::Logger::Push(__FUNCTION__)
		#define PROFILE_FN_NAMED(x)             ::Hazard::Logger::Push(x)
		#define PROFILE_FN_END()         ::Hazard::Logger::Pop();  
	#else
		#define PROFILE_FN();
		#define PROFILE_FN_NAMED(x)
		#define PROFILE_FN_END()
	#endif
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

#define PROFILE_FN();
#define PROFILE_FN_NAMED(x)
#define PROFILE_FN_END()

#endif // !

