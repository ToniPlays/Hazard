#pragma once
#include <hzrpch.h>
#include "VulkanBuffers.h"
#include "VulkanContext.h"
#include <glad/glad.h>

namespace Hazard::Rendering::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VertexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;

		auto device = VulkanContext::GetDevice()->GetDevice();
		VkDeviceSize bufferSize = info.Size;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_Size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanContext::GetDevice()->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);

		if(info.Data != nullptr)
			SetData(info.Data, info.Size);

	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		vkDestroyBuffer(device, m_Buffer, nullptr);
		vkFreeMemory(device, m_BufferMemory, nullptr);
	}
	void VulkanVertexBuffer::Bind()
	{
		auto buffer = VulkanContext::GetSwapchain()->GetCurrentDrawCommandBuffer();
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(buffer, 0, 1, &m_Buffer, offsets);
	}
	void VulkanVertexBuffer::Unbind()
	{

	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		VkDeviceSize bufferSize = size;
		void* gpuData;
		vkMapMemory(device, m_BufferMemory, 0, bufferSize, 0, &gpuData);
		memcpy(gpuData, data, static_cast<size_t>(bufferSize));
		vkUnmapMemory(device, m_BufferMemory);
	}
	VulkanIndexBuffer::VulkanIndexBuffer(const IndexBufferCreateInfo& info) : m_Size(info.Size)
	{
		m_Usage = info.Usage;
		SetData(info.Data, info.Size);

	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		vkDestroyBuffer(device, m_Buffer, nullptr);
		vkFreeMemory(device, m_BufferMemory, nullptr);
	}
	void VulkanIndexBuffer::Bind()
	{
		auto buffer = VulkanContext::GetSwapchain()->GetCurrentDrawCommandBuffer();
		VkDeviceSize offsets = { 0 };
		vkCmdBindIndexBuffer(buffer, m_Buffer, offsets, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::Unbind()
	{

	}
	void VulkanIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;

		auto device = VulkanContext::GetDevice()->GetDevice();
		VkDeviceSize bufferSize = sizeof(uint32_t) * size;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size * sizeof(uint32_t);
		bufferInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
			throw std::runtime_error("failed to create vertex buffer!");
		}
		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanContext::GetDevice()->FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(device, &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate vertex buffer memory!");
		}

		vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);

		void* gpuData;
		vkMapMemory(device, m_BufferMemory, 0, bufferSize, 0, &gpuData);
		memcpy(gpuData, data, static_cast<size_t>(size));
		vkUnmapMemory(device, m_BufferMemory);

	}
	VulkanUniformBuffer::VulkanUniformBuffer(const UniformBufferCreateInfo& createInfo) : m_Size(createInfo.Size), m_Binding(createInfo.Binding)
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
	void VulkanUniformBuffer::SetData(const void* data)
	{

	}
}