
#include "RenderPass.h"
#include "Backend/Core/GraphicsContext.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/Metal.h"

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
			HZR_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
    }
}