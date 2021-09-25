#pragma once

#include <vulkan/vulkan.h>

#include "Hazard/Rendering/Buffers/Buffers.h"

namespace Hazard::Rendering::OpenGL
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(VertexBufferCreateInfo* info);
		~OpenGLVertexBuffer();

		void Bind() override;
		void Unbind() override;
		void SetData(const void* data, uint32_t size) override;
		virtual BufferLayout GetLayout() { return m_Layout; }

	private:

		BufferUsage m_Usage;
		uint32_t m_ID;
		BufferLayout m_Layout;
		uint32_t m_Size;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(IndexBufferCreateInfo* info);
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
}