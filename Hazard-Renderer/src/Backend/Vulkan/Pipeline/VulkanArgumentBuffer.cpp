#include "VulkanArgumentBuffer.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"
#include "../Core/VulkanAllocator.h"

namespace HazardRenderer::Vulkan
{
	VulkanArgumentBuffer::VulkanArgumentBuffer(ArgumentBufferCreateInfo* createInfo)
	{
		m_DebugName = createInfo->Name;
		m_Size = createInfo->Size;

		Invalidate();

		if (createInfo->Data)
		{
			BufferCopyRegion region = {};
			region.Size = createInfo->Size;
			region.Data = createInfo->Data;
			region.Offset = 0;
			SetData(region);
		}
	}
	VulkanArgumentBuffer::~VulkanArgumentBuffer()
	{

	}
	void VulkanArgumentBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		m_LocalData.Release();
		m_LocalData = Buffer::Copy(copyRegion.Data, copyRegion.Size);

		Ref<VulkanArgumentBuffer> instance = this;
		Renderer::Submit([instance, copyRegion]() mutable {
			instance->SetData_RT(copyRegion);
			});
	}
	void VulkanArgumentBuffer::Invalidate()
	{
		Ref<VulkanArgumentBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	void VulkanArgumentBuffer::Invalidate_RT()
	{
		VulkanAllocator allocator("VulkanArgumentBuffer");

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_Size;
		bufferInfo.usage = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		m_BufferAllocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_VulkanBuffer);
	}
	void VulkanArgumentBuffer::SetData_RT(const BufferCopyRegion& copyRegion)
	{
		if (m_LocalData.Size == 0) return;

		VulkanAllocator allocator("VulkanArgumentBuffer");

		uint8_t* dst = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(dst + copyRegion.Offset, m_LocalData.Data, copyRegion.Size);
		allocator.UnmapMemory(m_BufferAllocation);

		m_LocalData.Release();

		std::cout << "Argument buffer data written" << std::endl;
	}
}
#endif