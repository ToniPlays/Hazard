#pragma once

#include "Backend/Core/Core.h"
#include "ShaderDataType.h"
#include "Backend/Core/RTCore/AccelerationStructure.h"

#include <spirv_cross/spirv_reflect.hpp>
#include <unordered_map>
#include <vector>

namespace HazardRenderer
{
	class UniformBuffer;

	enum class ShaderStage : uint32_t
	{
		None = 0,
		Vertex = BIT(0),
		Fragment = BIT(1),
		Compute = BIT(2),
		Geometry = BIT(3),
		Raygen = BIT(4),
		Miss = BIT(5),
		ClosestHit = BIT(6),
		AnyHit = BIT(7)
	};
	enum ShaderResourceFlags : uint32_t
	{
		ShaderResourceFlags_ReadOnly = BIT(0),
		ShaderResourceFlags_WriteOnly = BIT(1),
		ShaderResourceFlags_ReadAndWrite = BIT(2)
	};

	struct ShaderStageCode
	{
		ShaderStage Stage;
		uint32_t Size;
		Buffer ShaderCode;
	};
    struct ShaderMemberType
    {
        std::string Name;
        ShaderDataType Type = ShaderDataType::None;
        uint32_t Offset = 0;
    };

	struct ShaderStageInput
	{
        ShaderMemberType Type;
		uint32_t Location = 0;
	};
	struct ShaderStageAccelerationStructure
	{
		std::string Name;
		uint32_t Location = 0;
		uint32_t UsageFlags = 0;
	};
	struct ShaderStorageBuffer
	{
		std::string Name;
		uint32_t Location;
		uint32_t UsageFlags;
	};
    //Do we need this?
	/*struct ShaderStageOutput
	{
		std::string Name;
        ShaderDataType Type = ShaderDataType::None;
		uint32_t Location = 0;
		uint32_t Size = 0;
	};*/
	struct ShaderUniformBufferDescription
	{
		std::string Name;
		uint32_t Binding = 0;
		size_t Size = 0;
		uint32_t UsageFlags = 0;
		uint32_t DescritorSet = UINT32_MAX;
        
        std::vector<ShaderMemberType> Members;
	};
	struct ShaderImageSampler 
	{
		std::string Name;
		uint32_t Binding = 0;
		uint32_t Dimension = 0;
		uint32_t ArraySize = 0;
		uint32_t DescritorSet = UINT32_MAX;
		uint32_t Flags = 0;
	};
	struct ShaderStorageImage
	{
		std::string Name;
		uint32_t Binding = 0;
		uint32_t Dimension = 0;
		uint32_t ArraySize = 0;
		uint32_t DescritorSet = UINT32_MAX;
		uint32_t Flags = 0;
	};
    struct ShaderPushConstantRange
    {
        std::string Name;
        uint32_t DescriptorSet = 0;
        uint32_t Binding = 0;
        size_t Size = 0;
        uint32_t UsageFlags = 0;
        
        std::vector<ShaderMemberType> Members;
    };

	struct ShaderStageData
	{
		std::unordered_map<uint32_t, ShaderStageInput> Inputs;
		//std::unordered_map<uint32_t, ShaderStageOutput> Outputs;
		uint32_t Stride = 0;
	};

	struct ShaderData 
	{
		std::unordered_map<ShaderStage, ShaderStageData> Stages;
		//Set Binding, buffer
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderUniformBufferDescription>> UniformsDescriptions;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderStageAccelerationStructure>> AccelerationStructures;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderStorageBuffer>> StorageBuffers;
		//Set binding sampler
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderImageSampler>> ImageSamplers;
		std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderStorageImage>> StorageImages;
        std::unordered_map<uint32_t, std::unordered_map<uint32_t, ShaderPushConstantRange>> PushConstants;
	};
	namespace Utils
	{
		static ShaderStage ShaderStageFromString(const std::string& type) 
		{
			if (type == "Vertex")		return ShaderStage::Vertex;
			if (type == "Fragment")		return ShaderStage::Fragment;
			if (type == "Pixel")		return ShaderStage::Fragment;
			if (type == "Compute")		return ShaderStage::Compute;
			if (type == "Raygen")		return ShaderStage::Raygen;
			if (type == "Miss")			return ShaderStage::Miss;
			if (type == "ClosestHit")	return ShaderStage::ClosestHit;
			if (type == "AnyHit")		return ShaderStage::AnyHit;
			HZR_ASSERT(false, "Undefined shader stage");
			return ShaderStage::None;
		}
		static std::string ShaderStageToString(const uint32_t& type) 
		{
			if (type & (uint32_t)ShaderStage::Vertex)		return "Vertex";
			if (type & (uint32_t)ShaderStage::Fragment)		return "Fragment";
			if (type & (uint32_t)ShaderStage::Compute)		return "Compute";
			if (type & (uint32_t)ShaderStage::Raygen)		return "Raygen";
			if (type & (uint32_t)ShaderStage::Miss)			return "Miss";
			if (type & (uint32_t)ShaderStage::ClosestHit)	return "ClosestHit";
			if (type & (uint32_t)ShaderStage::AnyHit)		return "AnyHit";
			HZR_ASSERT(false, "Undefined shader stage");
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
            case SPIRType::UInt:
                    switch (type.vecsize)
                    {
                    case 1:     return ShaderDataType::UInt;
                    case 2:     return ShaderDataType::UInt2;
                    case 3:     return ShaderDataType::UInt3;
                    case 4:     return ShaderDataType::UInt4;
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

	class CubemapTexture;
	class Image2D;

	class Shader : public RefCount
	{
	public:
		virtual ~Shader() = default;
		virtual void Reload() = 0;

		virtual void Set(const std::string& name, uint32_t index, Ref<Image2D> image) = 0;
		virtual void Set(const std::string& name, uint32_t index, Ref<CubemapTexture> cubemap) = 0;
		virtual void Set(const std::string& name, uint32_t index, Ref<AccelerationStructure> accelerationStructure) = 0;
		virtual void Set(const std::string& name, uint32_t index, Ref<BufferBase> buffer) = 0;
        virtual void Set(const std::string& name, Buffer buffer) = 0;
        
        virtual void Set(uint32_t index, Ref<VertexBuffer> buffer, size_t offset = 0) = 0;

		virtual const ShaderData& GetShaderData() = 0;
		virtual std::unordered_map<ShaderStage, Buffer> GetShaderCode() const = 0;

		static Ref<Shader> Create(const std::vector<ShaderStageCode>& shaderCode);
	};
}