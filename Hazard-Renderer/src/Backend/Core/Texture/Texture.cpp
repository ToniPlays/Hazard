#include "Texture.h"

#include "Backend/OpenGL/OpenGLCore.h"

namespace HazardRenderer {
	Ref<CubemapTexture> CubemapTexture::Create(CubemapTextureCreateInfo* createInfo)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLCubemapTexture>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		//case RenderAPI::Vulkan: return Ref<Vulkan::VulkanImage2D>::Create(info);
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
