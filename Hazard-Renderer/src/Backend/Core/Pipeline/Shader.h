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

	enum class ShaderStage : uint32_t 
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
		uint32_t Location = 0;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
	};
	struct ShaderStageOutput
	{
		std::string Name;
		uint32_t Location = 0;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
	};
	struct ShaderUniformBufferDescription
	{
		std::string Name;
		uint32_t Binding = 0;
		uint32_t Size = 0;
		uint32_t MemberCount = 0;
		uint32_t UsageFlags = 0;
		uint32_t DescritorSet = UINT32_MAX;
	};
	struct ShaderImageSampler 
	{
		std::string Name;
		uint32_t Binding = 0;
		uint32_t Dimension = 0;
		uint32_t ArraySize = 0;
		uint32_t DescritorSet = UINT32_MAX;
	};

	struct ShaderStageData
	{
		std::unordered_map<uint32_t, ShaderStageInput> Inputs;
		std::unordered_map<uint32_t, ShaderStageOutput> Outputs;
		uint32_t Stride = 0;
	};

	struct ShaderData 
	{
		std::unordered_map<ShaderStage, ShaderStageData> Stages;
		//Set Binding, buffer
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderUniformBufferDescription>> UniformsDescriptions;
		//Set binding sampler
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderImageSampler>> ImageSamplers;
	};
	namespace Utils
	{
		static ShaderStage ShaderStageFromString(const std::string& type) 
		{
			if (type == "Vertex")		return ShaderStage::Vertex;
			if (type == "Fragment")		return ShaderStage::Fragment;
			if (type == "Pixel")		return ShaderStage::Fragment;
			if (type == "Compute")		return ShaderStage::Compute;
			return ShaderStage::None;
		}
		static std::string ShaderStageToString(const uint32_t& type) {
			if (type & (uint32_t)ShaderStage::Vertex)		return "Vertex";
			if (type & (uint32_t)ShaderStage::Fragment)		return "Fragment";
			if (type & (uint32_t)ShaderStage::Compute)		return "Compute";
			return "Unknown";
		}
		static ShaderDataType ShaderDataTypeFromSPV(const spirv_cross::SPIRType& type) 
		{
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
			if (flags & (uint32_t)ShaderStage::Vertex)		result += "Vertex";
			if (flags & (uint32_t)ShaderStage::Fragment)	result += " Fragment";
			if (flags & (uint32_t)ShaderStage::Compute)		result += " Compute";
			if (flags & (uint32_t)ShaderStage::Geometry)	result += " Geometry";
			return result;
		}
	}

	class Shader : public RefCount {
	public:
		virtual ~Shader() = default;
		virtual void Reload() = 0;
		virtual bool SetUniformBuffer(uint32_t set, uint32_t binding, void* data, uint32_t size) = 0;

		virtual void Set(uint32_t set, uint32_t binding, Ref<Image2D> image) = 0;
		virtual void Set(uint32_t set, uint32_t binding, Ref<UniformBuffer> uniformBuffer) = 0;

		virtual const ShaderData& GetShaderData() = 0;

		static Ref<Shader> Create(const std::string& path);
	};
}
