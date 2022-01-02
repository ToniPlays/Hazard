
#include <hzrpch.h>
#include "FrameBuffer.h"
#include "../RenderCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"


namespace Hazard::Rendering 
{
	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferCreateInfo* info)
	{
		switch (RenderCommand::GetAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLFrameBuffer>::Create(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanFrameBuffer>::Create(info);
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return nullptr;
#endif
		default:
			HZR_CORE_ASSERT(false, "Unknown RendererAPI");
			return nullptr;
		}
		return nullptr;
	}
}
