
#include "VulkanIndexBuffer.h"
#include "Backend/Core/Renderer.h"
#include "../VulkanContext.h"

namespace HazardRenderer::Vulkan
{
	VulkanIndexBuffer::VulkanIndexBuffer(IndexBufferCreateInfo* createInfo)
	{
		m_DebugName = createInfo->DebugName;
		m_Usage = createInfo->Usage;
		m_Size = createInfo->Size;

		if (!createInfo->Data) return;

		m_LocalData = Buffer::Copy(createInfo->Data, createInfo->Size);

		Ref<VulkanIndexBuffer> instance = this;

		Renderer::Submit([instance]() mutable {
			auto device = VulkanContext::GetLogicalDevice();
			VulkanAllocator allocator("VulkanVertexBuffer");

			VkBufferCreateInfo stagingInfo = {};
			stagingInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			stagingInfo.size = instance->m_Size;
			stagingInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			stagingInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;

			VkBuffer stagingBuffer;
			VmaAllocation stagingBufferAlloc = allocator.AllocateBuffer(stagingInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, stagingBuffer);

			//Copy data
			uint8_t* dstData = allocator.MapMemory<uint8_t>(stagingBufferAlloc);
			memcpy(dstData, instance->m_LocalData.Data, instance->m_Size);
			allocator.UnmapMemory(stagingBufferAlloc);

			VkBufferCreateInfo vertexBufferCreateInfo = {};
			vertexBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vertexBufferCreateInfo.size = instance->m_Size;
			vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			instance->m_BufferAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_IndexBuffer);

			VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);

			VkBufferCopy copyRegion = {};
			copyRegion.size = instance->m_Size;

			vkCmdCopyBuffer(commandBuffer, stagingBuffer, instance->m_IndexBuffer, 1, &copyRegion);

			device->FlushCommandBuffer(commandBuffer);

			allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);
			});
	}
	VulkanIndexBuffer::~VulkanIndexBuffer()
	{

		VkBuffer buffer = m_IndexBuffer;
		VmaAllocation alloc = m_BufferAllocation;

		Renderer::SubmitResourceFree([buffer, alloc]() mutable {
			VulkanAllocator allocator("VulkanIndexBuffer");
			allocator.DestroyBuffer(buffer, alloc);
			});

		m_LocalData.Release();
	}
	void VulkanIndexBuffer::SetData(uint32_t* data, size_t size)
	{
		__debugbreak();
	}
	void VulkanIndexBuffer::SetData_RT(uint32_t* data, size_t size)
	{
		__debugbreak();
	}
}