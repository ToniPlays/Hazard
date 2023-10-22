#include "Sampler.h"
#include "GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer
{
	Ref<Sampler> Sampler::Create(SamplerCreateInfo* createInfo)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLImage2D>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanSampler>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal:  return Ref<Metal::MetalSampler>::Create(createInfo);
#endif
		default:
			return nullptr;
		}
		return nullptr;
	}
}
