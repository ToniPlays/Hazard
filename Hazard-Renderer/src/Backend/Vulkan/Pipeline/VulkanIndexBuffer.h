#pragma once

#include "Backend/Core/Pipeline/Buffers.h"
#ifdef HZR_INCLUDE_VULKAN
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan 
{
	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(IndexBufferCreateInfo* createInfo);
		~VulkanIndexBuffer();

		size_t GetCount() const override { return m_Size / sizeof(uint32_t); }
		std::string& GetDebugName() { return m_DebugName; };
		const size_t GetSize() const { return m_Size; };

		//Vulkan specific

		VkBuffer GetVulkanBuffer() const { return m_IndexBuffer; }

	private:
		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);

		std::string m_DebugName;
		uint32_t m_Size;

		Buffer m_LocalData;
		VkBuffer m_IndexBuffer;
		VmaAllocation m_BufferAllocation;
	};
}
#endif