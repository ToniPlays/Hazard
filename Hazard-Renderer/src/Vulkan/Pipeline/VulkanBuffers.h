#pragma once

#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/Pipeline/Buffers.h"

#include "Buffer.h"

#include "vk_mem_alloc.h"
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	class VulkanVertexBuffer : public VertexBuffer
	{
	public:
		VulkanVertexBuffer(VertexBufferCreateInfo* info);
		~VulkanVertexBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void SetData(const void* data, uint32_t size) override;
		void RT_SetData(const void* data, uint32_t size);
		uint32_t GetSize() override { return m_Size; }
		std::string& GetDebugName() { return m_DebugName; }
		const BufferLayout& GetLayout() const { return m_Layout; }

		VkBuffer GetVulkanBuffer() const {
			return m_Buffer;
		}

	private:
		std::string m_DebugName;

		uint32_t m_Size = 0;
		HazardUtility::Buffer m_LocalData;

		VkBuffer m_Buffer;
		BufferUsage m_Usage;
		VmaAllocation m_Allocation;
		BufferLayout m_Layout;
	};

	class VulkanIndexBuffer : public IndexBuffer
	{
	public:
		VulkanIndexBuffer(IndexBufferCreateInfo* createInfo);
		~VulkanIndexBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind(Ref<RenderCommandBuffer> cmdBuffer) override;
		std::string& GetDebugName() { return m_DebugName; }

		uint32_t GetCount() override { return m_Size; }

		VkBuffer GetVulkanBuffer() const { return m_Buffer;	}

	private:
		void SetData(uint32_t* data, uint32_t size);

		std::string m_DebugName;
		uint32_t m_Size = 0;
		HazardUtility::Buffer m_LocalData;

		VkBuffer m_Buffer;
		VmaAllocation m_Allocation;

	};
	class VulkanUniformBuffer : public UniformBuffer
	{
		friend class VulkanShader;
	public:
		VulkanUniformBuffer(UniformBufferCreateInfo* createInfo);
		~VulkanUniformBuffer();

		void Bind(Ref<RenderCommandBuffer> cmdBuffer) override;
		void Unbind() override;
		void SetData(const void* data, uint32_t size) override;
		void RT_SetData();

		std::string& GetName() { return m_Name; }
		const uint32_t GetBinding() const override { return m_Binding; };
		const uint32_t GetSize() const { return m_Size; };
		uint32_t GetUsageFlags() { return m_Usage; };
		VkBuffer GetBuffer() { return m_Buffer; }

	private:
		void Release();
		void RT_Invalidate();
		void SetCurrentReadPointer(uint32_t writes, uint32_t size);
		void ResizeBuffer(uint32_t newSize);

	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;
		
		uint32_t m_Writes = 0;
		uint32_t m_Offset = 0;

		uint32_t m_FrameIndex = -1;

		VkBuffer m_Buffer = nullptr;
		
		VmaAllocation m_Allocation = nullptr;
		HazardUtility::Buffer m_LocalData;
	};
}
#endif