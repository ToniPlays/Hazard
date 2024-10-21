#include "Cubemap.h"

#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer 
{
	Ref<Cubemap> Cubemap::Create(CubemapCreateInfo* createInfo)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLCubemapTexture>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanCubemap>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal:  return Ref<Metal::MetalCubemap>::Create(createInfo);
#endif
		default:
			return nullptr;
		}
		return nullptr;
	}
}
