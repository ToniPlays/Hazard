#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <stdio.h>

#include "Hazard/Utils/Maths/Time.h"
#include "Hazard/Utils/Loaders/MeshLoader.h"
#include "Hazard/Utils/Maths/Color.h"

#include "Hazard/Utils/Maths/Vector/Vector2.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Loaders/File.h"

#include "Hazard/Core/HazardLoop.h"

#include "Hazard/Core/Events/Event.h"
#include "Hazard/Core/Events/ApplicationEvent.h"
#include "Hazard/Core/Events/MouseEvent.h"
#include "Hazard/Core/Events/KeyEvent.h"

#include "Hazard/Modules/Logging/Logger.h"


#ifdef HZR_DEBUG
//Core logging macros
#define HZR_CORE_INFO(...)		 ::Hazard::Logger::CoreLog(__VA_ARGS__, 0)
#define HZR_CORE_WARN(...)		 ::Hazard::Logger::CoreLog(__VA_ARGS__, 1)
#define HZR_CORE_ERROR(...)	 	 ::Hazard::Logger::CoreLog(__VA_ARGS__, 2)
#define HZR_CORE_FATAL(...)		 ::Hazard::Logger::CoreLog(__VA_ARGS__, 3)
#define HZR_CORE_TRACE(...)		 ::Hazard::Logger::CoreLog(__VA_ARGS__, 4)

//Client logging macros
#define HZR_INFO(...)			 ::Hazard::Logger::ClientLog(__VA_ARGS__, 0)
#define HZR_WARN(...)			 ::Hazard::Logger::ClientLog(__VA_ARGS__, 1)
#define HZR_ERROR(...)			 ::Hazard::Logger::ClientLog(__VA_ARGS__, 2)
#define HZR_FATAL(...)			 ::Hazard::Logger::ClientLog(__VA_ARGS__, 3)
#define HZR_TRACE(...)			 ::Hazard::Logger::ClientLog(__VA_ARGS__, 4)
#define HZR_ASSERT(success, ...) ::Hazard::Logger::Assert(success, __VA_ARGS__)

#define PROFILE_FN()             ::Hazard::Logger::Push(__FUNCTION__)
#define PROFILE_FN_NAMED(x)             ::Hazard::Logger::Push(x)
#define PROFILE_FN_END()         ::Hazard::Logger::Pop();  

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
#define PROFILE_FN_NAMED(x)
#define PROFILE_FN_END()
#define HZR_ASSERT()

#endif // !

