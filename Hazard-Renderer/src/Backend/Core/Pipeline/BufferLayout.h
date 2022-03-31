#pragma once

#include "Backend/Core/Core.h"
#include "Shader.h"
#include "ShaderDataType.h"

namespace HazardRenderer {


	enum Divisor : uint32_t {
		PerVertex = 0,
		PerInstance = 1
	};

	struct BufferElement
	{
		std::string Name;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t ElementDivisor = 0;
		size_t Offset;
		bool Normalized;

		BufferElement() = default;

		BufferElement(const std::string& name, ShaderDataType type, bool normalized = false, Divisor divisor = PerVertex)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized), ElementDivisor(divisor)
		{
		}

		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
			case ShaderDataType::Int:     return 1;
			case ShaderDataType::Int2:    return 2;
			case ShaderDataType::Int3:    return 3;
			case ShaderDataType::Int4:    return 4;
			case ShaderDataType::Bool:    return 1;
                default:
					HZR_ASSERT(false, "Unknown ShaderDataType");
                    break;
			}

			HZR_ASSERT(false, "Unknown ShaderDataType!");
			return 0;
		}
	};

	class BufferLayout
	{
	public:
		BufferLayout() {}

		BufferLayout(std::initializer_list<BufferElement> elements)
			: m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}
		uint32_t GetBufferStride(uint32_t bufferIndex = 0) {
			uint32_t result = 0;
			for (auto& element : m_Elements)
			{
				if(element.ElementDivisor == bufferIndex)
					result += element.Size;
			}
			return result;
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		size_t GetElementCount() { return m_Elements.size(); }
		

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
