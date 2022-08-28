#pragma once

#include "Backend/Core/Core.h"
#include "ShaderDataType.h"
#include "../Texture/Image2D.h"

#include <spirv_cross/spirv_reflect.hpp>
#include <unordered_map>
#include <vector>

namespace HazardRenderer
{
	class UniformBuffer;

	enum class ShaderType : uint32_t 
	{
		None = BIT(0),
		Vertex = BIT(1),
		Fragment = BIT(2),
		Compute = BIT(3),
		Geometry = BIT(4)
	};

	struct ShaderStageInput
	{
		std::string Name;
		uint32_t Binding;
		uint32_t Location;
		ShaderDataType Type;
		uint32_t Size;
		uint32_t Offset;
	};
	struct ShaderStageOutput
	{
		std::string Name;
		uint32_t Location;
		ShaderDataType Type;
		uint32_t Size;
	};
	struct ShaderUniformBufferDescription
	{
		std::string Name;
		uint32_t Binding;
		uint32_t Size;
		uint32_t MemberCount;
		uint32_t ShaderUsage = 0;
	};
	struct ShaderSampledImage {
		std::string Name;
		uint32_t Binding;
		uint32_t Dimension;
		uint32_t ArraySize;
	};

	struct ShaderStageData
	{
		std::unordered_map<uint32_t, ShaderStageInput> Inputs;
		std::unordered_map<uint32_t, ShaderStageOutput> Outputs;
		std::unordered_map<uint32_t, ShaderSampledImage> SampledImages;
		uint32_t Stride = 0;
	};

	struct ShaderData {
		std::unordered_map<ShaderType, ShaderStageData> Stages;
		std::unordered_map<uint32_t, ShaderUniformBufferDescription> UniformsDescriptions;
	};
	namespace Utils
	{
		static ShaderType ShaderTypeFromString(const std::string& type) {
			if (type == "Vertex")		return ShaderType::Vertex;
			if (type == "Fragment")		return ShaderType::Fragment;
			if (type == "Pixel")		return ShaderType::Fragment;
			if (type == "Compute")		return ShaderType::Compute;
			return ShaderType::None;
		}
		static std::string ShaderTypeToString(const ShaderType& type) {
			if ((uint32_t)type & (uint32_t)ShaderType::Vertex)			return "Vertex";
			if ((uint32_t)type & (uint32_t)ShaderType::Fragment)		return "Fragment";
			if ((uint32_t)type & (uint32_t)ShaderType::Compute)		return "Compute";
			return "Unknown";
		}
		static ShaderDataType ShaderTypeFromSPV(const spirv_cross::SPIRType& type) {
			using namespace spirv_cross;
			switch (type.basetype)
			{
			case SPIRType::Float: {
				switch (type.vecsize)
				{
				case 1:     return ShaderDataType::Float;
				case 2:     return ShaderDataType::Float2;
				case 3:     return ShaderDataType::Float3;
				case 4:     return ShaderDataType::Float4;
                default:    return ShaderDataType::None;
				}
            }
			case SPIRType::Int:
				switch (type.vecsize)
				{
				case 1:     return ShaderDataType::Int;
				case 2:     return ShaderDataType::Int2;
				case 3:     return ShaderDataType::Int3;
				case 4:     return ShaderDataType::Int4;
                default:    return ShaderDataType::None;
				}
            default:
                return ShaderDataType::None;
			}
		}
		static std::string UsageFlagsToString(const uint32_t& flags) 
		{
			std::string result;

			if (flags & (uint32_t)ShaderType::Vertex)	result += "Vertex";
			if (flags & (uint32_t)ShaderType::Fragment) result += " Fragment";
			if (flags & (uint32_t)ShaderType::Compute)	result += " Compute";
			if (flags & (uint32_t)ShaderType::Geometry) result += " Geometry";
			return result;

		}
	}

	class Shader : public RefCount {
	public:
		virtual ~Shader() = default;
		virtual void Reload() = 0;
		//virtual void Bind(Ref<RenderCommandBuffer> cmdBufer) = 0;
		//virtual void Bind_RT(Ref<RenderCommandBuffer> cmdBufer) = 0;
		//virtual void Unbind() = 0;
		virtual bool SetUniformBuffer(const std::string& name, void* data, uint32_t size) = 0;
		virtual UniformBuffer& GetUniform(const std::string& name) = 0;

		virtual void Set(const std::string& name, uint32_t index, uint32_t value) = 0;
		virtual void Set(const std::string& name, uint32_t index, Ref<Image2D> value) = 0;

		virtual const ShaderData& GetShaderData() = 0;

		static Ref<Shader> Create(const std::string& path);
	};
}
