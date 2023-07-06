
#include "VulkanGPUBuffer.h"
#ifdef HZR_INCLUDE_VULKAN
#include "Backend/Core/Renderer.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanGPUBuffer::VulkanGPUBuffer(BufferCreateInfo* createInfo)
	{
		HZR_PROFILE_FUNCTION();
		m_DebugName = createInfo->Name;
		m_Size = createInfo->Size;
		m_UsageFlags = createInfo->UsageFlags;

		if (createInfo->Data && !(createInfo->UsageFlags & BUFFER_USAGE_DYNAMIC))
		{
			m_LocalData = Buffer::Copy(createInfo->Data, createInfo->Size);

			Ref<VulkanGPUBuffer> instance = this;
			Renderer::SubmitResourceCreate([instance]() mutable {
				auto device = VulkanContext::GetLogicalDevice();
				VulkanAllocator allocator("VulkanGPUBuffer");

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

				VkBufferCreateInfo bufferCreateInfo = {};
				bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferCreateInfo.size = instance->m_Size;
				bufferCreateInfo.usage = VkUtils::GetUsageFlags(instance->GetUsageFlags()) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				instance->m_BufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_GPU_ONLY, instance->m_Buffer);
				VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_BUFFER, fmt::format("VkBuffer {0}", instance->m_DebugName), instance->m_Buffer);

				VkCommandBuffer commandBuffer = device->GetCommandBuffer(true);

				VkBufferCopy copyRegion = {};
				copyRegion.size = instance->m_Size;

				vkCmdCopyBuffer(commandBuffer, stagingBuffer, instance->m_Buffer, 1, &copyRegion);
				device->FlushCommandBuffer(commandBuffer);
				allocator.DestroyBuffer(stagingBuffer, stagingBufferAlloc);

			});
		}
		else
		{
			Ref<VulkanGPUBuffer> instance = this;
			Renderer::SubmitResourceCreate([instance]() mutable {
				auto device = VulkanContext::GetLogicalDevice();
				VulkanAllocator allocator("VulkanGPUBuffer");

				VkBufferCreateInfo bufferCreateInfo = {};
				bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
				bufferCreateInfo.size = instance->m_Size;
				bufferCreateInfo.usage = VkUtils::GetUsageFlags(instance->GetUsageFlags()) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

				instance->m_BufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, instance->m_Buffer);
				VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_BUFFER, fmt::format("VkBuffer {0}", instance->m_DebugName), instance->m_Buffer);
			});
			if (createInfo->Data)
			{
				BufferCopyRegion region = {};
				region.Data = createInfo->Data;
				region.Size = createInfo->Size;
				region.Offset = 0;

				SetData(region);
			}
		}
	}
	VulkanGPUBuffer::~VulkanGPUBuffer()
	{
		HZR_PROFILE_FUNCTION();
		VkBuffer buffer = m_Buffer;
		VmaAllocation alloc = m_BufferAllocation;

		Renderer::SubmitResourceFree([buffer, alloc]() mutable {
			VulkanAllocator allocator("VulkanGPUBuffer");
			allocator.DestroyBuffer(buffer, alloc);
		});

		m_LocalData.Release();
	}
	Buffer VulkanGPUBuffer::ReadData(const BufferCopyRegion& copyRegion)
	{
		Buffer result;
		result.Allocate(copyRegion.Size);
		result.ZeroInitialize();

		if (!m_BufferAllocation)
			return result;

		VulkanAllocator allocator("VulkanGPUBuffer");

		uint8_t* dst = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(result.Data, dst + copyRegion.Offset, copyRegion.Size);
		allocator.UnmapMemory(m_BufferAllocation);

		return result;
	}
	void VulkanGPUBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		Buffer buffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);

		Ref<VulkanGPUBuffer> instance = this;
		Renderer::Submit([instance, region = copyRegion, buffer]() mutable {
			instance->m_LocalData = buffer;
			instance->SetData_RT(region);
		});
	}
	void VulkanGPUBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();

		if (m_LocalData.Size == 0) return;

		VulkanAllocator allocator("VulkanGPUBuffer");

		uint8_t* dst = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(dst + copyRegion.Offset, m_LocalData.Data, copyRegion.Size);
		allocator.UnmapMemory(m_BufferAllocation);
		m_LocalData.Release();
	}
}
#endif