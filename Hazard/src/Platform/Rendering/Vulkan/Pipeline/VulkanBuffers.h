#pragma once

#include <vulkan/vulkan.h>

#include "Hazard/Rendering/Pipeline/Buffers.h"
#include "vk_mem_alloc.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(const VertexBufferCreateInfo& createInfo);
		~VulkanVertexBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data, uint32_t size) override;
		void RT_SetData(const void* data, uint32_t size);

		VkBuffer GetVulkanBuffer() const {
			return m_Buffer;
		}

	private:
		uint32_t m_Size = 0;
		Buffer m_LocalData;

		VkBuffer m_Buffer;
		BufferUsage m_Usage;
		VmaAllocation m_Allocation;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(const IndexBufferCreateInfo& createInfo);
		~VulkanIndexBuffer();

		void Bind() override;
		void Unbind() override;

		uint32_t GetCount() override { return m_Size; }

		VkBuffer GetVulkanBuffer() const {
			return m_Buffer;
		}

	private:
		void SetData(uint32_t* data, uint32_t size);

		uint32_t m_Size = 0;
		Buffer m_LocalData;

		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;

	};
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(const UniformBufferCreateInfo& createInfo);
		~VulkanUniformBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data) override;
		void RT_SetData(const void* data);

		virtual uint32_t GetUsageFlags() { return m_Usage; };

		uint32_t GetSize() { return m_Size; }
		uint32_t GetBinding() { return m_Binding; }
		VkBuffer GetBuffer() { return m_Buffer; }

	private:
		void Release();
		void RT_Invalidate();

	private:
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;

		VkBuffer m_Buffer;
		
		VmaAllocation m_Allocation = nullptr;
		uint8_t* m_LocalData = nullptr;
	};
}