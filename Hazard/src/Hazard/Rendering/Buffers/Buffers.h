#pragma once
#include "Hazard/Core/Core.h"

namespace Hazard::Rendering 
{
	enum VertexBufferUsage { StaticDraw, DynamiDraw, };

	class Buffer {
	public:
		static void* Copy(void* data, uint32_t size);
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
	};

	class IndexBuffer 
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
	};
}