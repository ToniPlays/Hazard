#pragma once

#include <hzrpch.h>
#include "Hazard/Rendering/Buffer/Buffers.h"

namespace Hazard {


	class HAZARD_API VertexArray {
	public:
		virtual ~VertexArray() = default;
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void EnableAll(uint32_t index = 0) const = 0;
		virtual uint32_t GetID() const = 0;

		virtual void AddBuffer(VertexBuffer* vertexBuffer) = 0;
		virtual IndexBuffer* GetIndexBuffer() const = 0;

		virtual std::vector<VertexBuffer*> GetBuffers() = 0;
		virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;
	};
}