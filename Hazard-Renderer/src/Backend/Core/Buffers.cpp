
#include "Backend/Core/GraphicsContext.h"
#include "Buffers.h"
#include "Backend/Core/RenderLibrary.h"

#include "Backend/OpenGL/OpenGLCore.h"
#include "Backend/Vulkan/VulkanCore.h"
#include "Backend/Metal/MetalCore.h"

namespace HazardRenderer {

	Ref<VertexBuffer> VertexBuffer::Create(VertexBufferCreateInfo* createInfo)
	{
		HZR_ASSERT(!createInfo->Name.empty(), "Unable to create buffer with no debug name");

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLVertexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanVertexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalVertexBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(IndexBufferCreateInfo* createInfo)
	{
		HZR_ASSERT(!createInfo->Name.empty(), "Unable to create buffer with no debug name");

		/*if (createInfo->IsShared && RenderLibrary::HasIndexBuffer(createInfo->DebugName)) {
			return RenderLibrary::GetIndexBuffer(createInfo->DebugName);
		}*/
		Ref<IndexBuffer> buffer = nullptr;

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLIndexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanIndexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalIndexBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		return nullptr;
	}
	Ref<UniformBuffer> UniformBuffer::Create(UniformBufferCreateInfo* createInfo)
	{
		if (RenderLibrary::HasUniformBuffer(createInfo->Name))
		{
			Ref<UniformBuffer> buffer = RenderLibrary::GetUniformBuffer(createInfo->Name);
            buffer->AddUsageFlags(createInfo->Usage);
			return buffer;
		}

		Ref<UniformBuffer> buffer = nullptr;

		switch (GraphicsContext::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: buffer = Ref<OpenGL::OpenGLUniformBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: buffer = Ref<Vulkan::VulkanUniformBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: buffer = Ref<Metal::MetalUniformBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		RenderLibrary::AddUniformBuffer(buffer);

		return buffer;
	}

    Ref<ArgumentBuffer> ArgumentBuffer::Create(ArgumentBufferCreateInfo* createInfo)
    {
        switch (GraphicsContext::GetRenderAPI())
        {
    #ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: return nullptr; //Ref<OpenGL::OpenGLStorageBuffer>::Create(createInfo); break;
    #endif
    #ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: return new Vulkan::VulkanArgumentBuffer(createInfo); break;
    #endif
    #ifdef HZR_INCLUDE_METAL
        case RenderAPI::Metal: return Ref<Metal::MetalStorageBuffer>::Create(createInfo); break;
    #endif
        default:
            return nullptr;
        }
        
        return nullptr;
    }
}
