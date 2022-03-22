
#include "RenderCommandBuffer.h"
#include "OpenGL/OpenGL.h"
#include "vulkan/VulkanCore.h"

namespace HazardRenderer {

	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(const std::string& debugName, uint32_t count)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLRenderCommandBuffer>::Create(count, debugName);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanRenderCommandBuffer>::Create(count, debugName);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return Ref<Metal::MetalRenderCommandBuffer>::Create(count, debugName);
#endif
		default:
			ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
	}

	Ref<RenderCommandBuffer> RenderCommandBuffer::CreateFromSwapchain(const std::string& debugName)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLRenderCommandBuffer>::Create(debugName, true);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanRenderCommandBuffer>::Create(debugName, true);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalRenderCommandBuffer>::Create(debugName, true);
#endif
		default:
			ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
	}
}
