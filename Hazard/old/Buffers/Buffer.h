#pragma once

#include <hzrpch.h>
#include "glad/glad.h"
#include "ShaderDataType.h"

namespace Hazard::Rendering {

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false) : Name(name), Type(type), Offset(0), Normalized(normalized)
		{
			Size = ShaderDataTypeSize(type);
		}
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return (uint32_t)1;
			case ShaderDataType::Float2:  return (uint32_t)2;
			case ShaderDataType::Float3:  return (uint32_t)3;
			case ShaderDataType::Float4:  return (uint32_t)4;
			case ShaderDataType::Mat3:    return (uint32_t)3; // 3 * float3
			case ShaderDataType::Mat4:    return (uint32_t)4; // 4 * float4
			case ShaderDataType::Int:     return (uint32_t)1;
			case ShaderDataType::Int2:    return (uint32_t)2;
			case ShaderDataType::Int3:    return (uint32_t)3;
			case ShaderDataType::Int4:    return (uint32_t)4;
			case ShaderDataType::Bool:    return (uint32_t)1;
			}
			//HZR_CORE_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout {
	public:
		BufferLayout() = default;

		BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride()
		{
			size_t offset = 0;
			m_Stride = 0;

			for (auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	enum DataStream {
		StreamDraw  = 0, 
		StreamRead  = 1,
		StreamCopy  = 2,
		StaticDraw  = 4,
		StaticRead  = 5,
		StaticCopy  = 6,
		DynamicDraw = 8,
		DynamicRead = 9,
		DynamicCopy = 10
	};

	struct VertexBufferCreateInfo 
	{
		BufferLayout* layout = nullptr;
		void* data = nullptr;
		DataStream dataStream = DataStream::StaticDraw;
		uint32_t size = 0;
	};
	struct VertexBufferInfo 
	{
		DataStream dataStream;
		BufferLayout layout;
	};

	struct IndexBufferCreateInfo 
	{
		DataStream dataStream = DataStream::StaticDraw;
		uint32_t size = 0;
		uint32_t* data = nullptr;
	};

	struct IndexBufferInfo {
		DataStream dataStream;
		uint32_t count;
	};
	struct UniformBufferCreateInfo {
		uint32_t size;
		uint32_t binding;
	};

	class VertexBuffer {
		friend class VertexDataBuffer;
	public:
		virtual ~VertexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetData(const void* data, uint32_t size) = 0;

		virtual VertexBufferInfo GetInfo() const = 0;
	};

	class IndexBuffer {
		friend class VertexDataBuffer;
	public:
		virtual ~IndexBuffer() = default;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual IndexBufferInfo GetInfo() const = 0;

	private:
		virtual void SetData(uint32_t* indices, uint32_t count) = 0;
	};
	class UniformBuffer : public RefCount {
	public:
		virtual ~UniformBuffer() {};
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
	};
}