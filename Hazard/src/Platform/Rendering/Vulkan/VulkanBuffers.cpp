#pragma once
#include <hzrpch.h>
#include "VulkanBuffers.h"

#include <glad/glad.h>

namespace Hazard::Rendering::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VertexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;
		m_Layout = *info.Layout;
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
	}
	void VulkanVertexBuffer::Bind()
	{

	}
	void VulkanVertexBuffer::Unbind()
	{

	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{

	}
	VulkanIndexBuffer::VulkanIndexBuffer(const IndexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
	}
	void VulkanIndexBuffer::Bind()
	{
	}
	void VulkanIndexBuffer::Unbind()
	{
	}
	void VulkanIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;
	}
	VulkanUniformBuffer::VulkanUniformBuffer(const UniformBufferCreateInfo& createInfo)
	{

	}
	VulkanUniformBuffer::~VulkanUniformBuffer()
	{

	}
	void VulkanUniformBuffer::Bind()
	{

	}
	void VulkanUniformBuffer::Unbind()
	{

	}
}