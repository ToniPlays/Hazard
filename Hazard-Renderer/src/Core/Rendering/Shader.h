#pragma once

#include "Core/Core.h"
#include "ShaderDataType.h"
#include "Core/RTCore/AccelerationStructure.h"

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

	/*
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
	*/

	class Shader : public RefCount
	{
	public:
		virtual ~Shader() = default;
		virtual void Reload() = 0;

		virtual std::unordered_map<uint32_t, Buffer> GetShaderCode() const = 0;
		virtual void SetShaderCode(const std::unordered_map<uint32_t, Buffer>& shaderCode) = 0;

		static Ref<Shader> Create(const std::unordered_map<uint32_t, std::string>& shaderModules);
	};
}
