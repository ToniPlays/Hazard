
#include "RenderPass.h"
#include "Core/GraphicsContext.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/Metal.h"

namespace HazardRenderer {

    Ref<RenderPass> RenderPass::Create(RenderPassCreateInfo* info)
    {
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return new OpenGL::OpenGLRenderPass(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return new Vulkan::VulkanRenderPass(info);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return new Metal::MetalRenderPass(info);
#endif
		default:
			ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}
