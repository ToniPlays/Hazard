
#include "VulkanVertexBuffer.h"
#include "../VulkanContext.h"
#include "Backend/Core/Renderer.h"

namespace HazardRenderer::Vulkan
{
	VulkanVertexBuffer::VulkanVertexBuffer(VertexBufferCreateInfo* createInfo)
	{
		HZR_PROFILE_FUNCTION();
		m_DebugName = createInfo->DebugName;
		m_Usage = createInfo->Usage;
		m_Size = createInfo->Size;
		m_Layout = *createInfo->Layout;

		if (createInfo->Data)
		{
			m_LocalData = Buffer::Copy(createInfo->Data, createInfo->Size);

			Ref<VulkanVertexBuffer> instance = this;
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
				vertexBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				instance->m_BufferAllocation = allocator.AllocateBuffer(vertexBufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VertexBuffer);

				VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);

				VkBufferCopy copyRegion = {};
				copyRegion.size = instance->m_Size;

				vkCmdCopyBuffer(commandBuffer, stagingBuffer, instance->m_VertexBuffer, 1, &copyRegion);

				device->FlushCommandBuffer(commandBuffer);

				allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);

				});
		}
		else
		{

			Ref<VulkanVertexBuffer> instance = this;
			Renderer::Submit([instance]() mutable {
				auto device = VulkanContext::GetLogicalDevice();
				VulkanAllocator allocator("VulkanVertexBuffer");

				VkBufferCreateInfo bufferCreateInfo = {};
				bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferCreateInfo.size = instance->m_Size;
				bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

				instance->m_BufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_VertexBuffer);

				});
		}
	}
	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		HZR_PROFILE_FUNCTION();
		VkBuffer buffer = m_VertexBuffer;
		VmaAllocation alloc = m_BufferAllocation;
		Renderer::SubmitResourceFree([buffer, alloc]() mutable {
			VulkanAllocator allocator("VulkanVertexBuffer");
			allocator.DestroyBuffer(buffer, alloc);
			});
		m_LocalData.Release();
	}
	void VulkanVertexBuffer::SetData(const void* data, size_t size)
	{
		HZR_PROFILE_FUNCTION();

		m_LocalData = Buffer::Copy(data, size);
		Ref<VulkanVertexBuffer> instance = this;

		Renderer::Submit([instance, size]() mutable {
			instance->SetData_RT(instance->m_LocalData.Data, size);
			});
	}
	void VulkanVertexBuffer::SetData_RT(const void* data, size_t size)
	{
		HZR_PROFILE_FUNCTION();

		VulkanAllocator allocator("VulkanVertexBuffer");
		uint8_t* dst = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(dst, data, size);
		allocator.UnmapMemory(m_BufferAllocation);

	}
}
