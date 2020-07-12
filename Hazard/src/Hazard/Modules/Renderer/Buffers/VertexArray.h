#pragma once

#include <hzrpch.h>
#include "Hazard/Modules/Renderer/Buffers/Buffer.h"

namespace Hazard {
	class HAZARD_API VertexArray {
	public:

		virtual void Bind() const = 0;
		virtual void BindAll() const = 0;
		virtual void Unbind() const = 0;
		virtual IndexBuffer* GetIndexBuffer() const = 0;

		virtual void SetLayout(std::initializer_list<VertexBuffer*> buffers) = 0;
		virtual void AddBuffer(VertexBuffer* buffers) = 0;
		virtual VertexBuffer* GetBuffer(std::string name) = 0;
		virtual void SetIndexBuffer(IndexBuffer* indexBuffer) = 0;
	};
}