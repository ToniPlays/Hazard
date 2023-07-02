#include "DescriptorSet.h"
#include "GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer 
{
	Ref<DescriptorSet> DescriptorSet::Create(DescriptorSetCreateInfo* createInfo)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLFrameBuffer>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanDescriptorSet>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalFrameBuffer>::Create(createInfo);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}