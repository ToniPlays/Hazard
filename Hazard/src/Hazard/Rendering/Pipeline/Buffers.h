#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Core/Buffer.h"
#include "ShaderDataType.h"
#include "../RenderCommandBuffer.h"

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
		size_t Size = 0;
		BufferUsage Usage = BufferUsage::StaticDraw;
		void* Data = nullptr;
	};
	struct IndexBufferCreateInfo 
	{
		size_t Size;
		BufferUsage Usage;
		uint32_t* Data = nullptr;
	};
	struct UniformBufferCreateInfo 
	{
		std::string Name;
		size_t Size;
		uint32_t Binding;
		uint32_t Usage;
	};

	class VertexBuffer : public RefCount
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual uint32_t GetSize() = 0;

		static Ref<VertexBuffer> Create(VertexBufferCreateInfo* createInfo);
	};

	class IndexBuffer : public RefCount
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		virtual void SetData(uint32_t* data, uint32_t size) = 0;
		virtual uint32_t GetCount() = 0;

		static Ref<IndexBuffer> Create(IndexBufferCreateInfo* createInfo);
	};
	class UniformBuffer : public RefCount
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data) = 0;
		virtual uint32_t GetUsageFlags() = 0;
		virtual const uint32_t GetBinding() const = 0;
		virtual const uint32_t GetSize() const = 0;

		static Ref<UniformBuffer> Create(UniformBufferCreateInfo* createInfo);
	};
}