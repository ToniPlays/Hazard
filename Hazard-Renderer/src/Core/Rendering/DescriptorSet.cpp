#include "DescriptorSet.h"
#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer 
{
	Ref<DescriptorSet> DescriptorSet::Create(DescriptorSetCreateInfo* createInfo)
	{
		HZR_ASSERT(!createInfo->DebugName.empty(), "DescriptorSet DebugName is empty");

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLFrameBuffer>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanDescriptorSet>::Create(createInfo);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return Ref<Metal::MetalDescriptorSet>::Create(createInfo);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}
