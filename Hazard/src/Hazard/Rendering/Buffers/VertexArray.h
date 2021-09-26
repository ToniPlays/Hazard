#pragma once
#include "Buffers.h"

namespace Hazard::Rendering 
{
	struct VertexArrayCreateInfo {
		VertexBufferCreateInfo* VertexBuffer;
		IndexBufferCreateInfo* IndexBuffer;
	};

	class VertexArray 
	{
	public:
		~VertexArray() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void AddBuffer(VertexBuffer* buffer) = 0;
		virtual IndexBuffer& GetIndexBuffer() = 0;
		virtual void SetIndexBuffer(IndexBuffer* buffer) = 0;

		virtual VertexBuffer& GetVertexBuffer() = 0;

		static VertexArray* Create(const VertexArrayCreateInfo& info);
	};
}