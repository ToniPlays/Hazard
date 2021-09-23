#pragma once
#include <hzrpch.h>
#include "VulkanBuffer.h"
#include "Platform/Rendering/Vulkan/VKContext.h"
#include "Platform/Rendering/Vulkan/Core/VulkanAllocator.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(uint32_t size, VertexBufferUsage usage) : m_Size(size)
	{
		auto device = VKContext::GetDevice();
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = size;
		createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		
		VulkanAllocator allocator("VertexBuffer");
		auto allocBuffer = allocator.AllocateBuffer(&createInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, &m_Buffer);

	}
	VulkanVertexBuffer::VulkanVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage) : m_Size(size)
	{
		m_LocalData = Buffer::Copy(data, size);

		auto device = VKContext::GetDevice();
		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = m_Size;
		createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		VulkanAllocator allocator("VertexBuffer");
		auto allocBuffer = allocator.AllocateBuffer(&createInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, &m_Buffer);

		void* dest = allocator.MapMemory<void>(allocBuffer);
		memcpy(dest, m_LocalData, m_Size);
		allocator.UnmapMemory(allocBuffer);
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		auto device = VKContext::GetDevice();
		vkDestroyBuffer(device->GetDevice(), m_Buffer, nullptr);
	}
	void VulkanVertexBuffer::Bind()
	{

	}
	VulkanIndexBuffer::VulkanIndexBuffer(uint32_t size) : m_Size(size) {}
	VulkanIndexBuffer::VulkanIndexBuffer(void* data, uint32_t size) : m_Size(size)
	{
		m_LocalData = Buffer::Copy(data, m_Size);

		VkBufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		createInfo.size = m_Size;
		createInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		VulkanAllocator allocator("IndexBuffer");
		auto allocBuffer = allocator.AllocateBuffer(&createInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, &m_Buffer);

		void* destBuffer = allocator.MapMemory<void>(allocBuffer);
		memcpy(destBuffer, m_LocalData, m_Size);
		allocator.UnmapMemory(allocBuffer);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		auto device = VKContext::GetDevice();
		vkDestroyBuffer(device->GetDevice(), m_Buffer, nullptr);
	}
	void VulkanIndexBuffer::Bind(CommandBuffer* buffer)
	{
		vkCmdBindIndexBuffer(*buffer->GetBuffer(), m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
}