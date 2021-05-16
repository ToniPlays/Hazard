#pragma once

#include "Hazard/Rendering/Buffers/Buffer.h"

namespace Hazard::Rendering::Vulkan {

	class VKVertexBuffer : public VertexBuffer {
	public:
		VKVertexBuffer(const VertexBufferCreateInfo& info);
		~VKVertexBuffer();

		void Bind() const;
		void Unbind() const;
		void SetData(const void* data, uint32_t size);

		const BufferLayout& GetLayout() const { return m_Layout; };
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; };
	private:
		BufferLayout m_Layout;
	};
	class VKIndexBuffer : public IndexBuffer {
	public:
		VKIndexBuffer(const IndexBufferCreateInfo& info);
		~VKIndexBuffer();

		void Bind() const;
		void Unbind() const;
		uint32_t GetID() const;

		uint32_t GetCount() const;

	private:
		void SetData(uint32_t * indices, uint32_t count);
	};
}