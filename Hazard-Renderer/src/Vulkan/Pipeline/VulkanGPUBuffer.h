#pragma once

#include "Core/Rendering//GPUBuffer.h"

#ifdef HZR_INCLUDE_VULKAN
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan 
{
	class VulkanGPUBuffer : public GPUBuffer
	{
	public:
		VulkanGPUBuffer(BufferCreateInfo* createInfo);
		~VulkanGPUBuffer();

		const std::string& GetDebugName() const override { return m_DebugName; };
		const uint64_t GetSize() const { return m_Size; };

		Buffer ReadData(const BufferCopyRegion& copyRegion) override;
		void SetData(const BufferCopyRegion& copyRegion);
		const uint32_t GetUsageFlags() const { return m_UsageFlags; };

		//Vulkan specific
		VkBuffer GetVulkanBuffer() const { return m_VulkanBuffer; }
		VmaAllocation GetVMAAllocation() const { return m_BufferAllocation; }

	private:

		std::string m_DebugName;
		uint32_t m_Size;
		uint32_t m_UsageFlags = 0;

		VkBuffer m_VulkanBuffer;
		VmaAllocation m_BufferAllocation = nullptr;
	};
}
#endif
