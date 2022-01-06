#include "hzrpch.h"
#include "GraphicsContext.h"
#include "Platform/System/Window.h"

#ifdef HZR_INCLUDE_OPENGL
#include "Platform/Rendering/OpenGL/OpenGL.h"
#endif
#ifdef HZR_INCLUDE_VULKAN
#include "Platform/Rendering/Vulkan/Vulkan.h"
#endif
#ifdef HZR_INCLUDE_METAL
#include "Platform/Rendering/Metal/MetalContext.h"
#endif

namespace Hazard::Rendering {

	GraphicsContext* GraphicsContext::Create(RenderAPI API, WindowProps* props)
	{
		switch (API)
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:		return new OpenGL::OpenGLContext(props);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:		return new Vulkan::VulkanContext(props);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal:      return new Metal::MetalContext(props);
#endif
        default:
            return nullptr;
		}
		return nullptr;
	}
}
