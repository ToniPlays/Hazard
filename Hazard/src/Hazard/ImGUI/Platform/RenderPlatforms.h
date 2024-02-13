#pragma once
#include "HazardRendererCore.h"

#ifdef HZR_INCLUDE_OPENGL
#include "OpenGL/GUIPlatformOpenGL.h"
#endif
#ifdef HZR_INCLUDE_VULKAN
#include "Vulkan/GUIPlatformVulkan.h"
#endif
