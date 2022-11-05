#include "VulkanUniformBuffer.h"
#ifdef HZR_INCLUDE_VULKAN

#include "spdlog/fmt/fmt.h"

#include "Backend/Core/Renderer.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "MathCore.h"

namespace HazardRenderer::Vulkan
{
	VulkanUniformBuffer::VulkanUniformBuffer(UniformBufferCreateInfo* createInfo)
	{
		HZR_PROFILE_FUNCTION();
		m_Name = createInfo->Name;
		m_Binding = createInfo->Binding;
		m_Size = Math::Max<uint32_t>(256, createInfo->Size);
		m_Usage = createInfo->Usage;
		m_LocalData.Allocate(m_Size * 1024);

		Ref<VulkanUniformBuffer> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		HZR_PROFILE_FUNCTION();
		Release();
	}
	void VulkanUniformBuffer::SetData(const BufferCopyRegion& copyRegion)
	{
		HZR_PROFILE_FUNCTION();
		uint32_t frameIndex = VulkanContext::GetFrameIndex();

		if (m_FrameIndex != frameIndex)
		{
			m_CurrentBufferDataIndex = 0;
			m_FrameIndex = frameIndex;
		}

		m_LocalData.TryWrite(copyRegion.Data, copyRegion.Size, m_CurrentBufferDataIndex);

		if (m_CurrentBufferDataIndex != 0)
		{
			m_CurrentBufferDataIndex += m_Size;
			return;
		}

		Ref<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->SetData_RT(instance->m_LocalData.Data, instance->m_LocalData.Size);
			});

		m_CurrentBufferDataIndex += m_Size;
	}
	void VulkanUniformBuffer::Release()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_BufferAllocation) return;

		VkBuffer buffer = m_UniformBuffer;
		VmaAllocation alloc = m_BufferAllocation;

		Renderer::SubmitResourceFree([buffer, alloc]() mutable {
			VulkanAllocator allocator("VulkanUniformBuffer");
			allocator.DestroyBuffer(buffer, alloc);
			});

		m_UniformBuffer = nullptr;
		m_BufferAllocation = nullptr;

		m_LocalData.Release();
	}
	void VulkanUniformBuffer::Release_RT()
	{
		HZR_PROFILE_FUNCTION();
		if (!m_BufferAllocation) return;

		VulkanAllocator allocator("VulkanUniformBuffer");
		allocator.DestroyBuffer(m_UniformBuffer, m_BufferAllocation);
		m_UniformBuffer = nullptr;
		m_BufferAllocation = nullptr;

		m_LocalData.Release();
	}
	void VulkanUniformBuffer::Invalidate_RT()
	{
		HZR_PROFILE_FUNCTION();
		Release_RT();

		auto device = VulkanContext::GetLogicalDevice();

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = 0;
		allocInfo.memoryTypeIndex = 0;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		bufferInfo.size = m_LocalData.Size;

		VulkanAllocator allocator("VulkanUniformBuffer");
		m_BufferAllocation = allocator.AllocateBuffer(bufferInfo, VMA_MEMORY_USAGE_CPU_TO_GPU, m_UniformBuffer);
		VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_BUFFER, fmt::format("VkBuffer {0}", m_Name), m_UniformBuffer);

		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_LocalData.Size;
		m_DescriptorInfo.buffer = m_UniformBuffer;
	}
	void VulkanUniformBuffer::SetData_RT(void* data, uint32_t size, uint32_t offset)
	{
		HZR_PROFILE_FUNCTION();
		VulkanAllocator allocator("VulkanUniformBuffer");
		uint8_t* dstData = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(dstData, (uint8_t*)m_LocalData.Data, m_LocalData.Size);
		allocator.UnmapMemory(m_BufferAllocation);
	}
}
#endif