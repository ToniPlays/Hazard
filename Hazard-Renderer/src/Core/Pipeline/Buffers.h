#pragma once

#include "BufferLayout.h"
#include "Buffer.h"
#include "Core/Core.h"
#include "Core/RenderCommandBuffer.h"


namespace HazardRenderer
{
	enum BufferUsage : int32_t
	{
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
		std::string DebugName;
		uint32_t Size = 0;
		BufferUsage Usage = BufferUsage::StaticDraw;
		BufferLayout* Layout = nullptr;
		void* Data = nullptr;
		bool IsShared = true;
	};
	struct IndexBufferCreateInfo 
	{
		std::string DebugName;
		uint32_t Size;
		BufferUsage Usage;
		uint32_t* Data = nullptr;
		bool IsShared = true;
	};
	struct UniformBufferCreateInfo 
	{
		std::string Name;
		uint32_t Size;
		uint32_t Binding;
		uint32_t Usage;
		bool IsShared = true;
	};

	class VertexBuffer : public RefCount
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual void Bind(Ref<RenderCommandBuffer> cmdBuffer) = 0;
		virtual void Unbind(Ref<RenderCommandBuffer> cmdBuffer) = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual uint32_t GetSize() = 0;
		virtual std::string& GetDebugName() = 0;

		virtual const BufferLayout& GetLayout() const = 0;


		static Ref<VertexBuffer> Create(VertexBufferCreateInfo* createInfo);
	};

	class IndexBuffer : public RefCount
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind(Ref<RenderCommandBuffer> cmdBuffer) = 0;
		virtual void Unbind(Ref<RenderCommandBuffer> cmdBuffer) = 0;

		virtual void SetData(uint32_t* data, uint32_t size) = 0;
		virtual uint32_t GetCount() = 0;
		virtual std::string& GetDebugName() = 0;

		static Ref<IndexBuffer> Create(IndexBufferCreateInfo* createInfo);
	};
	class UniformBuffer : public RefCount
	{
	public:
		virtual ~UniformBuffer() = default;
		virtual void Bind(Ref<RenderCommandBuffer> cmdBuffer) = 0;
		virtual void Unbind() = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;
		
		virtual std::string& GetName() = 0;
		virtual uint32_t GetUsageFlags() = 0;
		virtual const uint32_t GetBinding() const = 0;
		virtual const uint32_t GetSize() const = 0;

		static Ref<UniformBuffer> Create(UniformBufferCreateInfo* createInfo);
	};
}