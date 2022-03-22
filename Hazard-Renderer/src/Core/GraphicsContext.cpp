
#include "Core/Core.h"
#include "GraphicsContext.h"
#include "Window.h"

#ifdef HZR_INCLUDE_OPENGL
#include "OpenGL/OpenGL.h"
#endif
#ifdef HZR_INCLUDE_VULKAN
#include "Vulkan/VulkanCore.h"
#endif
#ifdef HZR_INCLUDE_METAL
#include "Metal/MetalContext.h"
#endif

namespace HazardRenderer {

	GraphicsContext* GraphicsContext::Create(WindowProps* props)
	{
		s_CurrentAPI = props->SelectedAPI;
		switch (s_CurrentAPI)
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
