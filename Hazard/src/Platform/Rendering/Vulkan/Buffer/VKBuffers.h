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
		
		VertexBufferInfo GetInfo() const override { return m_Info; }

	private:
		VertexBufferInfo m_Info;
	};
	class VKIndexBuffer : public IndexBuffer {
	public:
		VKIndexBuffer(const IndexBufferCreateInfo& info);
		~VKIndexBuffer();

		void Bind() const;
		void Unbind() const;

		IndexBufferInfo GetInfo() const override { return m_Info; }

	private:
		void SetData(uint32_t * indices, uint32_t count);

		IndexBufferInfo m_Info;
	};
}