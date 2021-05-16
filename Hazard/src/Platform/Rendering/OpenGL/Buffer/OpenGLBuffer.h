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

		virtual VertexBufferInfo GetInfo() const override { return m_Info; }

	private:
		uint32_t m_BufferID = 0;
		VertexBufferInfo m_Info;
	};

	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(const IndexBufferCreateInfo& info);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(uint32_t* indices, uint32_t count);

		virtual IndexBufferInfo GetInfo() const override { return m_Info; }
	private:
		uint32_t m_BufferID = 0;
		IndexBufferInfo m_Info;
	};
}