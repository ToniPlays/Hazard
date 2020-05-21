#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <deque>

#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

//Hazard stuff
#include "Core/Application.h"
#include "Core/Core.h"

#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Maths/Color.h"
#include "Maths/Time.h"

#include "Modules/Analytics/Logger.h"
#include "Platform/Window.h"


#ifdef HZR_DEBUG
	//Core logging macros
	#define HZR_CORE_TRACE(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 0);
	#define HZR_CORE_INFO(...)    ::Hazard::Logger::CoreLog(__VA_ARGS__, 1);
	#define HZR_CORE_WARN(...)    ::Hazard::Logger::CoreLog(__VA_ARGS__, 2);
	#define HZR_CORE_ERROR(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 3);
	#define HZR_CORE_FATAL(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 4);

	//Client logging macros
	#define HZR_TRACE(...)   ::Hazard::Logger::ClientLog(__VA_ARGS__, 0);
	#define HZR_INFO(...)    ::Hazard::Logger::ClientLog(__VA_ARGS__, 1);
	#define HZR_WARN(...)    ::Hazard::Logger::ClientLog(__VA_ARGS__, 2);
	#define HZR_ERROR(...)   ::Hazard::Logger::ClientLog(__VA_ARGS__, 3);
	#define HZR_FATAL(...)   ::Hazard::Logger::ClientLog(__VA_ARGS__, 4);

	#define PROFILE_FN()          ::Hazard::Logger::Begin(__FUNCTION__);
	#define PROFILE_NAMED_FN(...) ::Hazard::Logger::Begin(__VA_ARGS__);


#else

	//Core logging macros
	#define HZR_CORE_TRACE(...)
	#define HZR_CORE_INFO(...)
	#define HZR_CORE_WARN(...)
	#define HZR_CORE_ERROR(...)
	#define HZR_CORE_FATAL(...)

	//Client logging macros
	#define HZR_TRACE(...)
	#define HZR_INFO(...)
	#define HZR_WARN(...)
	#define HZR_ERROR(...)
#define HZR_FATAL(...)

	#define PROFILE_FN();
	#define PROFILE_NAMED_FN();
#endif // !

