
#include <hzrpch.h>
#include "Buffers.h"
#include "../RenderContextCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Vulkan.h"
#include "Platform/Rendering/Metal/Metal.h"
#include "../RenderLibrary.h"

namespace Hazard::Rendering {

	Ref<VertexBuffer> VertexBuffer::Create(VertexBufferCreateInfo* createInfo)
	{
		HZR_CORE_ASSERT(!createInfo->DebugName.empty(), "Unable to create buffer with no debug name");

		if (createInfo->IsShared && RenderLibrary::HasVertexBuffer(createInfo->DebugName)) {
			return RenderLibrary::GetVertexBuffer(createInfo->DebugName);
		}

		Ref<VertexBuffer> buffer = nullptr;
		switch (RenderContextCommand::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: buffer = Ref<OpenGL::OpenGLVertexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: buffer = Ref<Vulkan::VulkanVertexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: buffer =  Ref<Metal::MetalVertexBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		if (createInfo->IsShared) {
			RenderLibrary::AddVertexBuffer(buffer);
		}
		else AssetManager::AddRuntimeResource(buffer);

		buffer->m_Type = ResourceType::VertexBuffer;
		return buffer;
	}
	Ref<IndexBuffer> IndexBuffer::Create(IndexBufferCreateInfo* createInfo)
	{
		HZR_CORE_ASSERT(!createInfo->DebugName.empty(), "Unable to create buffer with no debug name");

		if (createInfo->IsShared && RenderLibrary::HasIndexBuffer(createInfo->DebugName)) {
			return RenderLibrary::GetIndexBuffer(createInfo->DebugName);
		}
		Ref<IndexBuffer> buffer = nullptr;

		switch (RenderContextCommand::GetRenderAPI())
		{
#ifdef HZR_INCLUDE_OPENGL
		case RenderAPI::OpenGL: buffer = Ref<OpenGL::OpenGLIndexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_VULKAN
		case RenderAPI::Vulkan: buffer = Ref<Vulkan::VulkanIndexBuffer>::Create(createInfo); break;
#endif
#ifdef HZR_INCLUDE_METAL
		case RenderAPI::Metal: return Ref<Metal::MetalIndexBuffer>::Create(createInfo); break;
#endif
		default:
			return nullptr;
		}

		if (createInfo->IsShared) {
			RenderLibrary::AddIndexBuffer(buffer);
		}
		else AssetManager::AddRuntimeResource(buffer);

		buffer->m_Type = ResourceType::IndexBuffer;
		return buffer;
	}
	Ref<UniformBuffer> UniformBuffer::Create(UniformBufferCreateInfo* createInfo)
	{
		if (createInfo->IsShared && RenderLibrary::HasUniformBuffer(createInfo->Name)) {
			Ref<UniformBuffer> buffer = RenderLibrary::GetUniformBuffer(createInfo->Name);
			HZR_CORE_ASSERT(buffer->GetBinding() == createInfo->Binding, "UniformBuffer: {0} binding does not match", createInfo->Name);
			return buffer;
		}

		Ref<UniformBuffer> buffer = nullptr;

		switch (RenderContextCommand::GetRenderAPI())
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

		if (createInfo->IsShared) {
			RenderLibrary::AddUniformBuffer(buffer);
		}
		else AssetManager::AddRuntimeResource(buffer);

		buffer->m_Type = ResourceType::UniformBuffer;
		return buffer;
	}
}