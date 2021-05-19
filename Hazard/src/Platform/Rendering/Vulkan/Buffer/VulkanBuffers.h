#pragma once

#include "Hazard/Rendering/Buffers/Buffer.h"

namespace Hazard::Rendering::Vulkan {

	class VulkanVertexBuffer : public VertexBuffer {
	public:
		VulkanVertexBuffer(const VertexBufferCreateInfo& info);
		~VulkanVertexBuffer();

		void Bind() const;
		void Unbind() const;
		void SetData(const void* data, uint32_t size);
		
		VertexBufferInfo GetInfo() const override { return m_Info; }

	private:
		VertexBufferInfo m_Info;
	};
	class VulkanIndexBuffer : public IndexBuffer {
	public:
		VulkanIndexBuffer(const IndexBufferCreateInfo& info);
		~VulkanIndexBuffer();

		void Bind() const;
		void Unbind() const;

		IndexBufferInfo GetInfo() const override { return m_Info; }

	private:
		void SetData(uint32_t * indices, uint32_t count);

		IndexBufferInfo m_Info;
	};
	class VulkanUniformBuffer : public UniformBuffer {
	public:
		VulkanUniformBuffer(const UniformBufferCreateInfo& info);
		~VulkanUniformBuffer();


		void SetData(const void* data, uint32_t size, uint32_t offset = 0);
	};
}