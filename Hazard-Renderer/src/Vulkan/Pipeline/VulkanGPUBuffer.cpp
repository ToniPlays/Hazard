
#include "VulkanGPUBuffer.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Renderer.h"
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

		Ref<VulkanGPUBuffer> instance = this;

		Renderer::SubmitResourceCreate([instance]() mutable {
			auto device = VulkanContext::GetLogicalDevice();
			VulkanAllocator allocator("VulkanGPUBuffer");

			uint32_t flags = instance->GetUsageFlags();

			VkBufferCreateInfo bufferCreateInfo = {};
			bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bufferCreateInfo.size = instance->m_Size;
			bufferCreateInfo.usage = VkUtils::GetUsageFlags(flags) | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

			instance->m_BufferAllocation = allocator.AllocateBuffer(bufferCreateInfo, (flags & BUFFER_USAGE_DYNAMIC) ? VMA_MEMORY_USAGE_CPU_TO_GPU : VMA_MEMORY_USAGE_GPU_ONLY, instance->m_VulkanBuffer);
			VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_BUFFER, fmt::format("VkBuffer {0}", instance->m_DebugName), instance->m_VulkanBuffer);
		});

		if (createInfo->Data && !(createInfo->UsageFlags & BUFFER_USAGE_DYNAMIC))
		{
			BufferCopyRegion region = {};
			region.Data = createInfo->Data;
			region.Size = createInfo->Size;
			region.Offset = 0;

			Ref<RenderCommandBuffer> cmdBuffer = RenderCommandBuffer::Create("Upload buffer", DeviceQueue::TransferBit, 1);
			cmdBuffer->Begin();
			cmdBuffer->CopyToBuffer(this, region);
			cmdBuffer->End();
			cmdBuffer->Submit();
		}
		else if(createInfo->Data)
		{
			BufferCopyRegion region = {};
			region.Data = createInfo->Data;
			region.Size = createInfo->Size;
			region.Offset = 0;

			SetData(region);
		}
	}
	VulkanGPUBuffer::~VulkanGPUBuffer()
	{
		HZR_PROFILE_FUNCTION();
		VkBuffer buffer = m_VulkanBuffer;
		VmaAllocation alloc = m_BufferAllocation;

		Renderer::SubmitResourceFree([buffer, alloc]() mutable {
			VulkanAllocator allocator("VulkanGPUBuffer");
			allocator.DestroyBuffer(buffer, alloc);
		});
	}
	CachedBuffer VulkanGPUBuffer::ReadData(const BufferCopyRegion& copyRegion)
	{
		CachedBuffer result;
		result.Allocate(copyRegion.Size);

		if (!m_BufferAllocation)
			return result;

		VulkanAllocator allocator("VulkanGPUBuffer");

		uint8_t* src = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		result.Write(src + copyRegion.Offset, copyRegion.Size);
		allocator.UnmapMemory(m_BufferAllocation);

		return result;
	}
	void VulkanGPUBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		Buffer buffer = Buffer::Copy(copyRegion.Data, copyRegion.Size);

		Ref<VulkanGPUBuffer> instance = this;
		Renderer::Submit([instance, offset = copyRegion.Offset, buffer]() mutable {

			VulkanAllocator allocator("VulkanGPUBuffer");

			uint8_t* dst = allocator.MapMemory<uint8_t>(instance->m_BufferAllocation);
			memcpy(dst + offset, buffer.Data, buffer.Size);
			allocator.UnmapMemory(instance->m_BufferAllocation);
			buffer.Release();
		});
	}
}
#endif
