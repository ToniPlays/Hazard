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
	VertexBuffer* VertexBuffer::Create(VertexBufferCreateInfo* info)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLVertexBuffer(info);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanVertexBuffer(info);
		}
	}
	IndexBuffer* IndexBuffer::Create(IndexBufferCreateInfo* info)
	{
		switch (RenderCommand::GetAPI())
		{
		case RenderAPI::OpenGL: return new OpenGL::OpenGLIndexBuffer(info);
		//case RenderAPI::Vulkan: return new Vulkan::VulkanIndexBuffer(info);
		}
	}
}
