
#include "AccelerationStructure.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/Vulkan/AccelerationStructure/VulkanTopLevelAS.h"
#include "Backend/Vulkan/AccelerationStructure/VulkanBottomLevelAS.h"

namespace HazardRenderer
{
	Ref<TopLevelAS> TopLevelAS::Create(AccelerationStructureCreateInfo* info)
	{
		HZR_ASSERT(info->Level == AccelerationStructureLevel::Top, "Wrong structure level");
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanTopLevelAS>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return nullptr; //Ref<Metal::MetalFrameBuffer>::Create(info);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
	Ref<BottomLevelAS> BottomLevelAS::Create(AccelerationStructureCreateInfo* info)
	{
		HZR_ASSERT(info->Level == AccelerationStructureLevel::Bottom, "Wrong structure level");
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanBottomLevelAS>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return nullptr; //Ref<Metal::MetalFrameBuffer>::Create(info);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}
