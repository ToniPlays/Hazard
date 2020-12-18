#pragma once
#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Buffer/Buffers.h"

namespace Hazard {

	class HAZARD_API OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual const BufferLayout& GetLayout() const override { return bufferLayout; }
		virtual void SetLayout(const BufferLayout& layout) override { bufferLayout = layout; }

	private:
		uint32_t BufferID = 0;
		BufferLayout bufferLayout;
	};

	class HAZARD_API OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer();
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void Unbind() const;
		virtual void SetData(uint32_t* indices, uint32_t count);
		virtual uint32_t GetID() const override { return BufferID; }
		virtual uint32_t GetCount() const { return count; }
	private:
		uint32_t BufferID = 0;
		uint32_t count = 0;
	};
}