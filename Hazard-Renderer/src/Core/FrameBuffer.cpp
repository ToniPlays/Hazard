
#include "FrameBuffer.h"
#include "GraphicsContext.h"

#include "OpenGL/OpenGL.h"
#include "vulkan/VulkanCore.h"

namespace HazardRenderer
{
	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferCreateInfo* info)
	{
		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLFrameBuffer>::Create(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		//case RenderAPI::Vulkan: return Ref<Vulkan::VulkanFrameBuffer>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalFrameBuffer>::Create(info);
#endif
		default:
			return nullptr;
		}
		return nullptr;
	}
}
