
#include "RenderPass.h"
#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer
{
    Ref<RenderPass> RenderPass::Create(RenderPassCreateInfo* info)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLRenderPass>::Create(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanRenderPass>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalRenderPass>::Create(info);
#endif
		default:
			HZR_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
