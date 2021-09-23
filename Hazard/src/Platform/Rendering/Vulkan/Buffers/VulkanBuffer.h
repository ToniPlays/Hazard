#pragma once

#include <vulkan/vulkan.h>
#include "CommandBuffer.h"

#include "Hazard/Rendering/Buffers/Buffers.h"

namespace Hazard::Rendering::Vulkan
{
	class VulkanVertexBuffer : public VertexBuffer 
	{
	public:
		VulkanVertexBuffer(uint32_t size, VertexBufferUsage usage);
		VulkanVertexBuffer(void* data, uint32_t size, VertexBufferUsage usage);
		~VulkanVertexBuffer();

		void Bind() override;
	private:
		void* m_LocalData = nullptr;
		uint32_t m_Size;
		VkBuffer m_Buffer;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(uint32_t size);
		VulkanIndexBuffer(void* data, uint32_t size);
		~VulkanIndexBuffer();
		
		void Bind() override {};
		void Bind(CommandBuffer* buffer);

		uint32_t GetCount() { return m_Size; }

	private:

		void* m_LocalData = nullptr;
		uint32_t m_Size;
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
	};
}