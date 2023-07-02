
#include "Backend/Core/GraphicsContext.h"
#include "GPUBuffer.h"
#include "Backend/Core/RenderLibrary.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

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
		case RenderAPI::Metal: return Ref<Metal::MetalIndexBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		return nullptr;
	}
}
