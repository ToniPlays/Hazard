#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <stdio.h>

#include "Hazard/Utils/File.h"
#include "Hazard/Utils/Maths/Time.h"
#include "Hazard/Core/ApplicationInfo.h"
#include "Hazard/Core/HazardLoop.h"
#include "Hazard/Core/Events/Event.h"
#include "Hazard/Core/Events/ApplicationEvent.h"
#include "Hazard/Core/Events/MouseEvent.h"
#include "Hazard/Core/Events/KeyEvent.h"

#include "Hazard/Modules/Logging/Logger.h"


#ifdef HZR_DEBUG
//Core logging macros
#define HZR_CORE_INFO(...)    ::Hazard::Logger::CoreLog(__VA_ARGS__, 0);
#define HZR_CORE_WARN(...)    ::Hazard::Logger::CoreLog(__VA_ARGS__, 1);
#define HZR_CORE_ERROR(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 2);
#define HZR_CORE_FATAL(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 3);
#define HZR_CORE_TRACE(...)   ::Hazard::Logger::CoreLog(__VA_ARGS__, 4);

//Client logging macros
#define HZR_TRACE(...)        ::Hazard::Logger::ClientLog(__VA_ARGS__, 0);
#define HZR_INFO(...)         ::Hazard::Logger::ClientLog(__VA_ARGS__, 1);
#define HZR_WARN(...)         ::Hazard::Logger::ClientLog(__VA_ARGS__, 2);
#define HZR_ERROR(...)        ::Hazard::Logger::ClientLog(__VA_ARGS__, 3);
#define HZR_FATAL(...)        ::Hazard::Logger::ClientLog(__VA_ARGS__, 4);

#define PROFILE_FN()             ::Hazard::Logger::Begin(__FUNCTION__);
#define PROFILE_NAMED_FN(...)    ::Hazard::Logger::Begin(__VA_ARGS__);
#define HZR_ASSERT(success, ...) ::Hazard::Logger::Assert(success, __VA_ARGS__);


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
#define HZR_ASSERT();

#endif // !

