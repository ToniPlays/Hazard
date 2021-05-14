#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Buffers/Buffer.h"

namespace Hazard::Rendering::OpenGL {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(const VertexBufferCreateInfo& info);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_BufferLayout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_BufferLayout = layout; }

	private:
		uint32_t m_BufferID = 0;
		BufferLayout m_BufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const IndexBufferCreateInfo& info);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(uint32_t* indices, uint32_t count);
		virtual uint32_t GetID() const override { return m_BufferID; }

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		uint32_t m_BufferID = 0;
		uint32_t m_Count = 0;
	};
}