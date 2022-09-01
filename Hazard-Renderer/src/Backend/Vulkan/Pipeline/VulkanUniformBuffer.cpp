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
		m_Name = createInfo->Name;
		m_Binding = createInfo->Binding;
		m_Size = Math::Max<uint32_t>(256, createInfo->Size);
		m_Usage = createInfo->Usage;
		m_LocalData.Allocate(m_Size * 64);
		std::cout << fmt::format("Create UniformBuffer: {0}, at {1}, {2}", m_Name, createInfo->Set, createInfo->Binding) << std::endl;

		Ref<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	VulkanUniformBuffer::~VulkanUniformBuffer()
	{
		Release();
	}
	void VulkanUniformBuffer::SetData(const void* data, size_t size)
	{
		uint32_t frameIndex = VulkanContext::GetInstance()->GetSwapchain().As<VulkanSwapchain>()->GetCurrentBufferIndex();
		if (m_FrameIndex != frameIndex)
		{
			m_CurrentBufferDataIndex = 0;
			m_FrameIndex = frameIndex;
		}
		uint32_t offset = m_CurrentBufferDataIndex * m_Size;
		m_LocalData.Write(data, size, offset);
		m_CurrentBufferDataIndex++;

		Ref<VulkanUniformBuffer> instance = this;
		Renderer::Submit([instance, size, offset]() mutable {
			instance->SetData_RT(instance->m_LocalData.Data, size, offset);
			});
	}
	void VulkanUniformBuffer::Release()
	{
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

		if (!m_BufferAllocation) return;

		VulkanAllocator allocator("VulkanUniformBuffer");
		allocator.DestroyBuffer(m_UniformBuffer, m_BufferAllocation);
		m_UniformBuffer = nullptr;
		m_BufferAllocation = nullptr;

		m_LocalData.Release();
	}
	void VulkanUniformBuffer::Invalidate_RT()
	{
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
		VkUtils::SetDebugUtilsObjectName(device->GetVulkanDevice(), VK_OBJECT_TYPE_BUFFER, m_Name, m_UniformBuffer);

		m_DescriptorInfo.offset = 0;
		m_DescriptorInfo.range = m_LocalData.Size;
		m_DescriptorInfo.buffer = m_UniformBuffer;
	}
	void VulkanUniformBuffer::SetData_RT(void* data, uint32_t size, uint32_t offset)
	{
		VulkanAllocator allocator("VulkanUniformBuffer");
		uint8_t* dstData = allocator.MapMemory<uint8_t>(m_BufferAllocation);
		memcpy(dstData + offset, (uint8_t*)m_LocalData.Data + offset, size);
		allocator.UnmapMemory(m_BufferAllocation);
	}
}
#endif