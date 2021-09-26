#pragma once
#include "Hazard/Core/Core.h"
#include "BufferLayout.h"
#include "ShaderDataType.h"

namespace Hazard::Rendering 
{
	enum BufferUsage {
		StreamDraw = 0,
		StreamRead = 1,
		StreamCopy = 2,
		StaticDraw = 4,
		StaticRead = 5,
		StaticCopy = 6,
		DynamicDraw = 8,
		DynamicRead = 9,
		DynamicCopy = 10
	};


	struct VertexBufferCreateInfo 
	{
		uint32_t Size;
		void* Data = nullptr;
		BufferUsage Usage;
		BufferLayout* Layout;
	};
	struct IndexBufferCreateInfo 
	{
		uint32_t Size;
		uint32_t* Data = nullptr;
		BufferUsage Usage;
	};
	struct UniformBufferCreateInfo 
	{

	};


	class Buffer {
	public:
		static void* Copy(const void* data, size_t size);
	};

	class VertexBuffer 
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual BufferLayout GetLayout() = 0;

		static VertexBuffer* Create(const VertexBufferCreateInfo& createInfo);
	};

	class IndexBuffer 
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(uint32_t* data, uint32_t size) = 0;
		virtual uint32_t GetCount() = 0;

		static IndexBuffer* Create(const IndexBufferCreateInfo& createInfo);
	};
	class UniformBuffer 
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		static UniformBuffer* Create(const UniformBufferCreateInfo& createInfo);
	};
}