#include "Texture.h"

#include "Backend/Core/GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer {
	Ref<CubemapTexture> CubemapTexture::Create(CubemapTextureCreateInfo* createInfo)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLCubemapTexture>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanCubemapTexture>::Create(createInfo);
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
