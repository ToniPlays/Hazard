
#include "AccelerationStructure.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/Vulkan/AccelerationStructure/VulkanAccelerationStructure.h"

namespace HazardRenderer
{
	Ref<AccelerationStructure> AccelerationStructure::Create(AccelerationStructureCreateInfo* info)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanAccelerationStructure>::Create(info);
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
