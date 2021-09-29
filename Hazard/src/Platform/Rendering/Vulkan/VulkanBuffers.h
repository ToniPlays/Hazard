#pragma once

#include <vulkan/vulkan.h>

#include "Hazard/Rendering/Buffers/Buffers.h"

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

	private:
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		BufferUsage m_Usage;
		uint32_t m_Size;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(const IndexBufferCreateInfo& createInfo);
		~VulkanIndexBuffer();

		void Bind() override;
		void Unbind() override;

		uint32_t GetCount() override { return m_Size; }

	private:
		void SetData(uint32_t* data, uint32_t size);

		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;

		BufferUsage m_Usage;
		uint32_t m_Size;

	};
	class VulkanUniformBuffer : public UniformBuffer
	{
	public:
		VulkanUniformBuffer(const UniformBufferCreateInfo& createInfo);
		~VulkanUniformBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data) override;

	private:

		BufferUsage m_Usage;
		uint32_t m_Size;
		uint32_t m_Binding;

	};
}