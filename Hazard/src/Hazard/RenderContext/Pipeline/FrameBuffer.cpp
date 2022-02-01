
#include <hzrpch.h>
#include "FrameBuffer.h"
#include "../RenderContextCommand.h"
#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"
#include "Platform/Rendering/Metal/Metal.h"

namespace Hazard::Rendering 
{
	Ref<FrameBuffer> FrameBuffer::Create(FrameBufferCreateInfo* info)
	{
		switch (RenderContextCommand::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLFrameBuffer>::Create(info);
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanFrameBuffer>::Create(info);
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
