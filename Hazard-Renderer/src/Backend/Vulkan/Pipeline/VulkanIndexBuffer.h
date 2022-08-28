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

		//void Bind(Ref<RenderCommandBuffer> cmdBuffer);
		//void Unbind(Ref<RenderCommandBuffer> cmdBuffer);

		size_t GetCount() override { return m_Size / sizeof(uint32_t); }
		std::string& GetDebugName() { return m_DebugName; };

	private:
		void SetData(uint32_t* data, size_t size) override;
		void SetData_RT(uint32_t* data, size_t size);

		std::string m_DebugName;
		BufferUsage m_Usage;
		uint32_t m_Size;

		Buffer m_LocalData;
		VkBuffer m_IndexBuffer;
		VmaAllocation m_BufferAllocation;
	};
}
#endif