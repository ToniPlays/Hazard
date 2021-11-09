#pragma once

#include "Hazard/Rendering/Pipeline/Buffers.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::OpenGL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferCreateInfo* createInfo);
		~OpenGLVertexBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data, uint32_t size) override;

	private:

		BufferUsage m_Usage;
		uint32_t m_ID;
		uint32_t m_Size;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(IndexBufferCreateInfo* createInfo);
		~OpenGLIndexBuffer();

		void Bind() override;
		void Unbind() override;

		uint32_t GetCount() override { return m_Size; }

	private:
		void SetData(uint32_t* data, uint32_t size);

		BufferUsage m_Usage;

		uint32_t m_ID;
		uint32_t m_Size;

	};
	class OpenGLUniformBuffer : public UniformBuffer
	{
	public:
		OpenGLUniformBuffer(UniformBufferCreateInfo* createInfo);
		~OpenGLUniformBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data) override;
		const uint32_t GetBinding() const override { return m_Binding; };
		const uint32_t GetSize() const override { return m_Size; };

		uint32_t GetUsageFlags() { return m_Usage; };

	private:
		uint32_t m_ID;
		uint32_t m_Size;
		uint32_t m_Binding;
		uint32_t m_Usage;
	};
}