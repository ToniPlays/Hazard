#pragma once

#ifdef HZR_INCLUDE_OPENGL
#include "OpenGL/EditorPlatformOpenGL.h"
#endif
#ifdef HZR_INCLUDE_VULKAN
#include "Vulkan/EditorPlatformVulkan.h"
#endif
#ifdef HZR_INCLUDE_METAL
#include "Metal/EditorPlatformMetal.h"
#endif
