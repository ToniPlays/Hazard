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

	enum ShaderStageFlags : uint32_t
	{
		SHADER_STAGE_NONE = 0,
		SHADER_STAGE_VERTEX_BIT = BIT(0),
		SHADER_STAGE_FRAGMENT_BIT = BIT(1),
		SHADER_STAGE_COMPUTE_BIT = BIT(2),
		SHADER_STAGE_GEOMETRY_BIT = BIT(3),
		SHADER_STAGE_RAYGEN_BIT = BIT(4),
		SHADER_STAGE_MISS_BIT = BIT(5),
		SHADER_STAGE_CLOSEST_HIT_BIT = BIT(6),
		SHADER_STAGE_ANY_HIT_BIT = BIT(7)
	};
	enum ResourceAccessFlags : uint32_t
	{
		SHADER_ACCESS_READ = BIT(0),
		SHADER_ACCESS_WRITE = BIT(1),
	};

	struct ShaderStageCode
	{
		uint32_t Stage;
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
	struct ShaderUniformBufferDescription
	{
		std::string Name;
		uint32_t Binding = 0;
		uint64_t Size = 0;
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
        uint32_t Set = 0;
        uint32_t Binding = 0;
		uint64_t Size = 0;
        uint32_t UsageFlags = 0;
        
        std::vector<ShaderMemberType> Members;
    };

	struct ShaderStageData
	{
		std::unordered_map<uint32_t, ShaderStageInput> Inputs;
		uint32_t Stride = 0;
	};

	struct ShaderData 
	{
		std::unordered_map<uint32_t, ShaderStageData> Stages;
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
		static ShaderStageFlags ShaderStageFromString(const std::string& type) 
		{
			if (type == "Vertex")		return SHADER_STAGE_VERTEX_BIT;
			if (type == "Fragment")		return SHADER_STAGE_FRAGMENT_BIT;
			if (type == "Pixel")		return SHADER_STAGE_FRAGMENT_BIT;
			if (type == "Compute")		return SHADER_STAGE_COMPUTE_BIT;
			if (type == "Raygen")		return SHADER_STAGE_RAYGEN_BIT;
			if (type == "Miss")			return SHADER_STAGE_MISS_BIT;
			if (type == "ClosestHit")	return SHADER_STAGE_CLOSEST_HIT_BIT;
			if (type == "AnyHit")		return SHADER_STAGE_ANY_HIT_BIT;
			HZR_ASSERT(false, "Undefined shader stage");
			return SHADER_STAGE_NONE;
		}
		static std::string ShaderStageToString(const uint32_t& type) 
		{
			if (type & SHADER_STAGE_VERTEX_BIT)			return "Vertex";
			if (type & SHADER_STAGE_FRAGMENT_BIT)		return "Fragment";
			if (type & SHADER_STAGE_COMPUTE_BIT)		return "Compute";
			if (type & SHADER_STAGE_RAYGEN_BIT)			return "Raygen";
			if (type & SHADER_STAGE_MISS_BIT)			return "Miss";
			if (type & SHADER_STAGE_CLOSEST_HIT_BIT)	return "ClosestHit";
			if (type & SHADER_STAGE_ANY_HIT_BIT)		return "AnyHit";
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
			if (flags & SHADER_STAGE_VERTEX_BIT)	result += " Vertex";
			if (flags & SHADER_STAGE_FRAGMENT_BIT)	result += " Fragment";
			if (flags & SHADER_STAGE_COMPUTE_BIT)	result += " Compute";
			if (flags & SHADER_STAGE_GEOMETRY_BIT)	result += " Geometry";
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

		virtual const ShaderData& GetShaderData() const = 0;
		virtual std::unordered_map<uint32_t, Buffer> GetShaderCode() const = 0;

		static Ref<Shader> Create(const std::vector<ShaderStageCode>& shaderCode);
	};
}
