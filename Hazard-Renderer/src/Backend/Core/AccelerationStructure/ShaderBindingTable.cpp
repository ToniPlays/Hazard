
#include "ShaderBindingTable.h"
#include "Backend/Core/GraphicsContext.h"
#include "Backend/Vulkan/VulkanCore.h"

namespace HazardRenderer
{
	Ref<ShaderBindingTable> ShaderBindingTable::Create(ShaderBindingTableCreateInfo* info)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanShaderBindingTable>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalShaderBindingTable>::Create(info);
#endif
		default:
			HZR_ASSERT(false, "Unknown RenderAPI");
		}
		return nullptr;
	}
}
