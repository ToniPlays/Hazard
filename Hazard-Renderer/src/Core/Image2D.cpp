
#include "Image2D.h"
#include "GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"

namespace HazardRenderer 
{
    Ref<Image2D> Image2D::Create(Image2DCreateInfo* info)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return new OpenGL::OpenGLImage2D(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return new Vulkan::VulkanImage2D(info);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return nullptr;
#endif
		default:
			return nullptr;
		}
		return nullptr;
    }
}
