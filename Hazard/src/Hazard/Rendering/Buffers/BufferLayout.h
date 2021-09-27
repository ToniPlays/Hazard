#pragma once

#include "Hazard/Core/Core.h"
#include "ShaderDataType.h"

namespace Hazard::Rendering 
{
	struct BufferElement {
		std::string Name;
		ShaderDataType Type;
		uint32_t Size = 0;
		size_t Offset = 0;
		bool Normalized;

		BufferElement() = default;
		BufferElement(const std::string& name, ShaderDataType type, bool normalize = false) : Name(name), Type(type), Normalized(normalize) {
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
			return 0;
		}
	};
	class BufferLayout 
	{
	public:
		BufferLayout() = default;
		BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() { return m_Stride; }
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
	private:

		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};
}