
#include <hzrpch.h>
#include "Image2D.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"

namespace Hazard::Rendering 
{
    Ref<Image2D> Image2D::Create(Image2DCreateInfo* info)
    {
		switch (RenderContextCommand::GetRenderAPI())
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
