#pragma once

#include "Backend/Core/Pipeline/Buffers.h"
#ifdef HZR_INCLUDE_VULKAN
#include "MathCore.h"

#include <vulkan/vulkan.h>
#include "vk_mem_alloc.h"

namespace HazardRenderer::Vulkan
{
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(UniformBufferCreateInfo* createInfo);
		~VulkanUniformBuffer();

		void SetData(const void* data, size_t size) override;

		std::string& GetName() { return m_Name; }
		const uint32_t GetBinding() const override { return m_Binding; };
		const size_t GetSize() const override { return m_Size; };

		uint32_t GetUsageFlags() { return m_Usage; };

		//Vulkan specific
		void Release();
		void Release_RT();
		void Invalidate_RT();
		void SetData_RT(void* data, uint32_t size);

		uint32_t GetOffset() { return (uint32_t)Math::Max<int>(((int)m_CurrentBufferDataIndex - 1) * m_Size, 0); }
		VkBuffer GetVulkanBuffer() const { return m_UniformBuffer; }
		Buffer& GetWriteBuffer() { return m_LocalData; }

	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;

		Buffer m_LocalData;
		uint32_t m_CurrentBufferDataIndex = 0;
		uint32_t m_FrameIndex = 255;

		VkBuffer m_UniformBuffer = VK_NULL_HANDLE;
		VmaAllocation m_BufferAllocation = nullptr;
		VkDescriptorBufferInfo m_DescriptorInfo;
	};
}
#endif