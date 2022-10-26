
#include "Backend/Core/Core.h"
#include "GraphicsContext.h"
#include "Window.h"

#ifdef HZR_INCLUDE_OPENGL
#include "Backend/OpenGL/OpenGLCore.h"
#endif
#ifdef HZR_INCLUDE_VULKAN
#include "Backend/Vulkan/VulkanCore.h"
#endif
#ifdef HZR_INCLUDE_METAL
#include "Backend/Metal/MetalContext.h"
#endif

namespace HazardRenderer {

	GraphicsContext* GraphicsContext::Create(WindowProps* props)
	{
		s_CurrentAPI = props->SelectedAPI;
		switch (s_CurrentAPI)
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL:		return hnew OpenGL::OpenGLContext(props);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan:		return hnew Vulkan::VulkanContext(props);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal:      return hnew Metal::MetalContext(props);
#endif
        default:
            return nullptr;
		}
		return nullptr;
	}
}
