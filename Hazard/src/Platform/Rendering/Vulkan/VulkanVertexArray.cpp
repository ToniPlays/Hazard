#pragma once

#include <hzrpch.h>
#include "VulkanVertexArray.h"

namespace Hazard::Rendering::Vulkan
{

	VulkanVertexArray::VulkanVertexArray(const VertexArrayCreateInfo& info)
	{
		AddBuffer(VertexBuffer::Create(*info.VertexBuffer));
		SetIndexBuffer(IndexBuffer::Create(*info.IndexBuffer));
	}
	VulkanVertexArray::~VulkanVertexArray()
	{

	}
	void VulkanVertexArray::Bind()
	{

	}
	void VulkanVertexArray::Unbind()
	{

	}
	void VulkanVertexArray::AddBuffer(VertexBuffer* buffer)
	{
		m_Buffers.push_back(buffer);
	}
	void VulkanVertexArray::SetIndexBuffer(IndexBuffer* buffer)
	{
		m_IndexBuffer = buffer;
	}
}