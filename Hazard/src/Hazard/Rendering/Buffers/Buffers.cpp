#pragma once

#include <hzrpch.h>
#include "Buffers.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"
#include "Platform/Rendering/Vulkan/VulkanBuffers.h"

namespace Hazard::Rendering {

	void* Buffer::Copy(const void* data, size_t size)
	{
		void* copiedData = malloc(size);
		memcpy(copiedData, data, size);
		return copiedData;
	}
	Ref<VertexBuffer> VertexBuffer::Create(const VertexBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLVertexBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanVertexBuffer>::Create(createInfo);
		}
	}
	Ref<IndexBuffer> IndexBuffer::Create(const IndexBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLIndexBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanIndexBuffer>::Create(createInfo);
		}
	}
	Ref<UniformBuffer> UniformBuffer::Create(const UniformBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return Ref<OpenGL::OpenGLUniformBuffer>::Create(createInfo);
		case RenderAPI::Vulkan: return Ref<Vulkan::VulkanUniformBuffer>::Create(createInfo);
		}
	}
}
