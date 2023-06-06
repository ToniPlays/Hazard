#pragma once

#include "BufferLayout.h"
#include "Backend/Core/Core.h"

namespace HazardRenderer
{
	class VertexBuffer;

	enum BufferUsageFlags : uint32_t
	{
		BUFFER_USAGE_TOP_LEVEL_ACCELERATION_STRUCTURE = BIT(0),
		BUFFER_USAGE_BOTTOM_LEVEL_ACCELERATION_STRUCTURE = BIT(1)
	};

	enum class BufferType
	{
		Vertex,
		Index,
		Uniform,
		Storage,
		Argument
	};
    
    struct BufferCreateInfo
    {
        std::string Name;
        size_t Size;
		uint32_t UsageFlags = 0;
		void* Data = nullptr;
    };

	struct VertexBufferCreateInfo : BufferCreateInfo
	{
		//No layout means object cannot be used for rendering meshes, only instancing data
		BufferLayout* Layout = nullptr;
	};
	struct IndexBufferCreateInfo : BufferCreateInfo
	{
		uint32_t* Data = nullptr;
	};
	struct ArgumentBufferCreateInfo : BufferCreateInfo
	{
		void* Data = nullptr;
	};
	struct UniformBufferCreateInfo : BufferCreateInfo
	{
		uint32_t Set;
		uint32_t Binding;
		uint32_t Usage;
	};

	struct BufferCopyRegion
	{
		size_t Size = 0;
		size_t Offset = 0;
		const void* Data = nullptr;
	};

	class BufferBase : public RefCount
	{
	public:
		virtual ~BufferBase() = default;

		virtual void SetData(const BufferCopyRegion& copyRegion) = 0;
		virtual const BufferType GetType() const = 0;
        virtual const size_t GetSize() const = 0;
	};

	class VertexBuffer : public BufferBase
	{
	public:
		virtual ~VertexBuffer() = default;
		virtual std::string& GetDebugName() = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		const BufferType GetType() const override { return BufferType::Vertex; };

		static Ref<VertexBuffer> Create(VertexBufferCreateInfo* createInfo);
	};

	class IndexBuffer : public BufferBase
	{
	public:
		virtual ~IndexBuffer() = default;

		virtual size_t GetCount() const = 0;
		virtual std::string& GetDebugName() = 0;
		const BufferType GetType() const override { return BufferType::Index; };

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

		const BufferType GetType() const override { return BufferType::Uniform; };

		static Ref<UniformBuffer> Create(UniformBufferCreateInfo* createInfo);
	};

    class ArgumentBuffer : public BufferBase
    {
    public:
        virtual ~ArgumentBuffer() = default;
        
        virtual const std::string& GetDebugName() = 0;
        const BufferType GetType() const override { return BufferType::Argument; };

        static Ref<ArgumentBuffer> Create(ArgumentBufferCreateInfo* createInfo);
    };
}
