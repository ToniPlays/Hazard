#include "DescriptorSet.h"
#include "GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

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
            case RenderAPI::Metal: return nullptr;
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}
