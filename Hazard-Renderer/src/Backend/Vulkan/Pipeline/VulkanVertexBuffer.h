#pragma once
#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Pipeline/Buffers.h"
#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VertexBufferCreateInfo* createInfo);
		~VulkanVertexBuffer();

		void SetData(const BufferCopyRegion& copyRegion) override;
		void SetData_RT(const BufferCopyRegion& copyRegion);
		size_t GetSize() const override { return m_Size; }
		std::string& GetDebugName() { return m_DebugName; };
		const BufferLayout& GetLayout() const override { return m_Layout; }

		//Vulkan specific
		VkBuffer GetVulkanBuffer() const { return m_VertexBuffer; }

	private:
		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_Size;
		BufferLayout m_Layout;
		Buffer m_LocalData;

		VkBuffer m_VertexBuffer;
		VmaAllocation m_BufferAllocation;
	};
}
#endif
