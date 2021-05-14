#pragma once

#include "Hazard/Rendering/Buffers/Buffer.h"

namespace Hazard::Rendering::Vulkan {

	class VKVertexBuffer : public VertexBuffer {
	public:
		VKVertexBuffer(uint32_t size);
		VKVertexBuffer(float* vertices, uint32_t size);

		virtual ~VKVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual const BufferLayout& GetLayout() const override { return m_BufferLayout;	}
		virtual void SetLayout(const BufferLayout& layout) { m_BufferLayout = layout; }

	private:
		BufferLayout m_BufferLayout;
	};
	class VKIndexBuffer : public IndexBuffer {
	public:
		VKIndexBuffer();
		virtual ~VKIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
		virtual void SetData(uint32_t* data, uint32_t size) override;
		virtual uint32_t GetID() const override { return m_BufferID; }
		virtual uint32_t GetCount() const override { return m_Count; }

	private:
		uint32_t m_BufferID = 0;
		uint32_t m_Count = 0;
	};
}