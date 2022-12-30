#pragma once

#include "BufferLayout.h"
#include "Backend/Core/Core.h"

namespace HazardRenderer
{
	class VertexBuffer;

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
		DynamicCopy = 10,
		TLAS = 11,
		BLAS = 12
	};

	enum class BufferType
	{
		Vertex,
		Index,
		Uniform,
		Storage
	};

	struct VertexBufferCreateInfo 
	{
		std::string DebugName;
		size_t Size = 0;
		BufferUsage Usage = BufferUsage::StaticDraw;

		//No layout means object cannot be used for rendering meshes, only instancing data
		BufferLayout* Layout = nullptr;
		void* Data = nullptr;
	};
	struct IndexBufferCreateInfo 
	{
		std::string DebugName;

		//Size is size in bytes
		size_t Size;
		BufferUsage Usage;
		uint32_t* Data = nullptr;
	};
	struct UniformBufferCreateInfo 
	{
		std::string Name;
		size_t Size;
		uint32_t Set;
		uint32_t Binding;
		uint32_t Usage;
	};

	struct BufferCopyRegion
	{
		const void* Data = nullptr;
		size_t Size = 0;
		size_t Offset = 0;
	};

	class BufferBase : public RefCount
	{
	public:
		virtual ~BufferBase() = default;

		virtual void SetData(const BufferCopyRegion& copyRegion) = 0;
		virtual BufferType GetType() const = 0;
	};

	class VertexBuffer : public BufferBase
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual size_t GetSize() const = 0;
		virtual std::string& GetDebugName() = 0;
		virtual const BufferLayout& GetLayout() const = 0;

		BufferType GetType() const override { return BufferType::Vertex; };

		static Ref<VertexBuffer> Create(VertexBufferCreateInfo* createInfo);
	};

	class IndexBuffer : public BufferBase
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual size_t GetCount() const = 0;
		virtual std::string& GetDebugName() = 0;
		BufferType GetType() const override { return BufferType::Index; };

		static Ref<IndexBuffer> Create(IndexBufferCreateInfo* createInfo);
	};
	class UniformBuffer : public BufferBase
	{
	public:
		virtual ~UniformBuffer() = default;
		//virtual void Unbind() = 0;
		
		virtual std::string& GetName() = 0;
		virtual uint32_t GetUsageFlags() = 0;
        virtual void AddUsageFlags(uint32_t flags) = 0;
		virtual const uint32_t GetBinding() const = 0;
		virtual const size_t GetSize() const = 0;

		BufferType GetType() const override { return BufferType::Uniform; };

		static Ref<UniformBuffer> Create(UniformBufferCreateInfo* createInfo);
	};
}
