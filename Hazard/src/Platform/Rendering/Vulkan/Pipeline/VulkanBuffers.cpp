#pragma once
#include <hzrpch.h>
#include "VulkanBuffers.h"
#include "../VulkanContext.h"
#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(VertexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		m_Usage = info->Usage;
		auto device = VulkanContext::GetDevice();

		VulkanAllocator allocator("VertexBuffer");
		
		if (info->Data == nullptr) {

			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.size = info->Size;
			createInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_TO_CPU, m_Buffer);
			m_LocalData.Allocate(m_Size);
		}
		else 
		{
			m_LocalData = Buffer::Copy(info->Data, info->Size);

			VkBufferCreateInfo stagingCreateInfo = {};
			stagingCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingCreateInfo.size = info->Size;
			stagingCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
			stagingCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
			memcpy(destData, m_LocalData.Data, m_LocalData.Size);
			allocator.UnmapMemory(stagingBufferAlloc);

			VkBufferCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			createInfo.size = info->Size;
			createInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

			m_Allocation = allocator.AllocateBuffer(createInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

			VkCommandBuffer copyCmd = device->GetCommandBuffer(true);
			VkBufferCopy copyRegion = {};
			copyRegion.size = m_LocalData.Size;

			vkCmdCopyBuffer(copyCmd, stagingBuffer, m_Buffer, 1, &copyRegion);
			device->FlushCommandBuffer(copyCmd);
			allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);

		}
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		HZR_CORE_INFO("Destroying VertexBuffer");
		m_LocalData.Release();
		VkBuffer buffer = m_Buffer;
		VmaAllocation allocation = m_Allocation;

		VulkanAllocator allocator("VertexBuffer");
		allocator.DestroyBuffer(buffer, allocation);
	}
	void VulkanVertexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		auto vkCmdBuffer = cmdBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindVertexBuffers(vkCmdBuffer, 0, 1, &m_Buffer, offsets);
	}
	void VulkanVertexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanVertexBuffer::SetData(const void* data, uint32_t size)
	{
		memcpy(m_LocalData.Data, (uint8_t*)data, size);
		RT_SetData(m_LocalData.Data, size);
	}
	void VulkanVertexBuffer::RT_SetData(const void* data, uint32_t size)
	{
		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy(pData, (uint8_t*)data, size);
		allocator.UnmapMemory(m_Allocation);
	}
	VulkanIndexBuffer::VulkanIndexBuffer(IndexBufferCreateInfo* info) : m_Size(info->Size)
	{
		m_DebugName = info->DebugName;
		if (info->Data == nullptr) return;
		SetData(info->Data, info->Size);
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{
		VkBuffer buffer = m_Buffer;
		VmaAllocation allocation = m_Allocation;

		VulkanAllocator allocator("IndexBuffer");
		allocator.DestroyBuffer(buffer, allocation);
	}
	void VulkanIndexBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		auto vkCmdBuffer = cmdBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
		VkDeviceSize offsets[1] = { 0 };
		vkCmdBindIndexBuffer(vkCmdBuffer, m_Buffer, 0, VK_INDEX_TYPE_UINT32);
	}
	void VulkanIndexBuffer::Unbind(Ref<RenderCommandBuffer> cmdBuffer)
	{

	}
	void VulkanIndexBuffer::SetData(uint32_t* data, uint32_t size)
	{
		m_Size = size;
		auto device = VulkanContext::GetDevice();
		VulkanAllocator allocator("IndexBuffer");
		m_LocalData = Buffer::Copy(data, m_Size * sizeof(uint32_t));

		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = m_LocalData.Size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		VkBuffer stagingBuffer;
		VmaAllocation stagingBufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

		// Copy data to staging buffer
		uint8_t* destData = allocator.MapMemory<uint8_t>(stagingBufferAllocation);
		memcpy(destData, m_LocalData.Data, m_LocalData.Size);
		allocator.UnmapMemory(stagingBufferAllocation);

		VkBufferCreateInfo indexBufferCreateInfo = {};
		indexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		indexBufferCreateInfo.size = m_LocalData.Size;
		indexBufferCreateInfo.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		m_Allocation = allocator.AllocateBuffer(indexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, m_Buffer);

		VkCommandBuffer copyCmd = device->GetCommandBuffer(true);
		VkBufferCopy copyRegion = {};
		copyRegion.size = m_LocalData.Size;
		vkCmdCopyBuffer(
			copyCmd,
			stagingBuffer,
			m_Buffer,
			1,
			&copyRegion);

		device->FlushCommandBuffer(copyCmd);

		allocator.DestroyBuffer(stagingBuffer, stagingBufferAllocation);
	}
	VulkanUniformBuffer::VulkanUniformBuffer(UniformBufferCreateInfo* createInfo) : m_Size(createInfo->Size), 
		m_Binding(createInfo->Binding), m_Usage(createInfo->Usage)
	{
		m_Name = createInfo->Name;
		m_LocalData = new uint8_t[m_Size];
		RT_Invalidate();
	}
	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}
	void VulkanUniformBuffer::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		
	}
	void VulkanUniformBuffer::Unbind()
	{

	}
	void VulkanUniformBuffer::SetData(const void* data)
	{
		memcpy(m_LocalData, data, m_Size);
		RT_SetData(m_LocalData);
	}
	void VulkanUniformBuffer::RT_SetData(const void* data)
	{
		VulkanAllocator allocator("VulkanUniformBuffer");
		uint8_t* pData = allocator.MapMemory<uint8_t>(m_Allocation);
		memcpy(pData, (const uint8_t*)data, m_Size);
		allocator.UnmapMemory(m_Allocation);
	}
	void VulkanUniformBuffer::Release()
	{
		if (!m_Allocation) return;

		VulkanAllocator allocator("UniformBuffer");
		allocator.DestroyBuffer(m_Buffer, m_Allocation);

		m_Buffer = nullptr;
		m_Allocation = nullptr;

		delete[] m_LocalData;
		m_LocalData = nullptr;
	}
	void VulkanUniformBuffer::RT_Invalidate()
	{
		Release();
		auto device = VulkanContext::GetDevice()->GetDevice();

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.pNext = nullptr;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.size = m_Size;

		VulkanAllocator allocator("UniformBuffer");
		m_Allocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_ONLY, m_Buffer);
	}
}