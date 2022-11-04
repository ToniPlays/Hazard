
#include "TopLevelAccelerationStructure.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/Vulkan/AccelerationStructure/VulkanTopLevelAccelerationStructure.h"

namespace HazardRenderer
{
	Ref<TopLevelAccelerationStructure> TopLevelAccelerationStructure::Create(TopLevelAccelerationStructureCreateInfo* info)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanTopLevelAccelerationStructure>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalFrameBuffer>::Create(info);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}
