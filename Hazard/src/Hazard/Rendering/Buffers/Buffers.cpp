#pragma once

#include <hzrpch.h>
#include "Buffers.h"
#include "../RenderCommand.h"

#include "Platform/Rendering/OpenGL/OpenGL.h"

namespace Hazard::Rendering {

	void* Buffer::Copy(const void* data, size_t size)
	{
		void* copiedData = malloc(size);
		memcpy(copiedData, data, size);
		return copiedData;
	}
	VertexBuffer* VertexBuffer::Create(const VertexBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLVertexBuffer(createInfo);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanVertexBuffer(info);
		}
	}
	IndexBuffer* IndexBuffer::Create(const IndexBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLIndexBuffer(createInfo);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanIndexBuffer(info);
		}
	}
	UniformBuffer* UniformBuffer::Create(const UniformBufferCreateInfo& createInfo)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLUniformBuffer(createInfo);
			//case RenderAPI::Vulkan: return new Vulkan::VulkanIndexBuffer(info);
		}
	}
}
