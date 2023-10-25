
#include "RenderCommandBuffer.h"
#include "Core/GraphicsContext.h"
#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer {

	Ref<RenderCommandBuffer> RenderCommandBuffer::Create(const std::string& debugName, DeviceQueue queue, uint32_t count)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLRenderCommandBuffer>::Create(debugName, queue, count);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanRenderCommandBuffer>::Create(debugName, queue, count);
#endif
#ifdef HZR_INCLUDE_METAL
            case RenderAPI::Metal: return Ref<Metal::MetalRenderCommandBuffer>::Create(debugName, queue, count);
#endif
		default:
			HZR_ASSERT(false, "Unknown RendererAPI");
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
        case RenderAPI::Metal:  return Ref<Metal::MetalRenderCommandBuffer>::Create(debugName, true);
#endif
		default:
			HZR_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
	}
}
