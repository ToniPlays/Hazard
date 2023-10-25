
#include "Core/GraphicsContext.h"
#include "GPUBuffer.h"

#include "OpenGL/OpenGLCore.h"
#include "Vulkan/VulkanCore.h"
#include "Metal/MetalCore.h"

namespace HazardRenderer 
{
	Ref<GPUBuffer> GPUBuffer::Create(BufferCreateInfo* createInfo)
	{
		HZR_ASSERT(!createInfo->Name.empty(), "Unable to create buffer with no debug name");

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		//case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLGPUBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanGPUBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalGPUBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		return nullptr;
	}
}
