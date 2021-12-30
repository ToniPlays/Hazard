#pragma once

#include <hzrpch.h>
#include "Buffers.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/Pipeline/VulkanBuffers.h"

namespace Hazard::Rendering {

	Ref<VertexBuffer> VertexBuffer::Create(VertexBufferCreateInfo* createInfo)
	{
		HZR_CORE_INFO("Creating VertexArray");
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLVertexBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanVertexBuffer>::Create(createInfo);
		}
		HZR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	Ref<IndexBuffer> IndexBuffer::Create(IndexBufferCreateInfo* createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLIndexBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanIndexBuffer>::Create(createInfo);
		}
		HZR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
	Ref<UniformBuffer> UniformBuffer::Create(UniformBufferCreateInfo* createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLUniformBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanUniformBuffer>::Create(createInfo);
		}
		HZR_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}
}
