#pragma once

#include "Core/Core.h"

#include "Core/Rendering/Shader.h"
#include "Core/RenderContextCreateInfo.h"
#include "Core/Rendering/Shader.h"

#include <spirv_cross/spirv_reflect.hpp>
#include <Core/Rendering/DescriptorSetLayout.h>

namespace Hazard
{
	namespace Utils
	{
		static HazardRenderer::ShaderStageFlags ShaderStageFromString(const std::string& type)
		{
			using namespace HazardRenderer;

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
			using namespace HazardRenderer;

			if (type & SHADER_STAGE_VERTEX_BIT)			return "Vertex";
			if (type & SHADER_STAGE_FRAGMENT_BIT)		return "Fragment";
			if (type & SHADER_STAGE_COMPUTE_BIT)		return "Compute";
			if (type & SHADER_STAGE_RAYGEN_BIT)			return "Raygen";
			if (type & SHADER_STAGE_MISS_BIT)			return "Miss";
			if (type & SHADER_STAGE_CLOSEST_HIT_BIT)	return "ClosestHit";
			if (type & SHADER_STAGE_ANY_HIT_BIT)		return "AnyHit";
			return "None";
		}
		static HazardRenderer::ShaderDataType ShaderDataTypeFromSPV(const spirv_cross::SPIRType& type)
		{
			using namespace HazardRenderer;
			using namespace spirv_cross;

			switch (type.basetype)
			{
				case SPIRType::Float:
				{
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
			using namespace HazardRenderer;

			std::string result;
			if (flags & SHADER_STAGE_VERTEX_BIT)	result += " Vertex";
			if (flags & SHADER_STAGE_FRAGMENT_BIT)	result += " Fragment";
			if (flags & SHADER_STAGE_COMPUTE_BIT)	result += " Compute";
			if (flags & SHADER_STAGE_GEOMETRY_BIT)	result += " Geometry";
			return result;
		}
	}

	struct ShaderCode
	{
		uint32_t Stage;
		uint64_t Length;
	};

	enum class ShaderPropertyType
	{
		String,
		Number,
		Bool,
		Object,
		Shader
	};

	struct ShaderCompilerSourceScope
	{
		std::string Scope;
		std::string Source;
		ShaderPropertyType Type;
	};

	struct ShaderProperty
	{
		std::string Name;
		std::string DisplayName;
		HazardRenderer::ShaderDataType Type;
		uint32_t Set;
		uint32_t Binding;
		uint32_t Length;
		uint32_t TypeFlags;
		uint32_t AccessFlags;
	};

	struct ShaderParseFileResult
	{
		std::string Name;
		std::string Language;
		std::string Version;
		std::string Type;
		std::unordered_map<std::string, std::string> PipelineState;
		std::vector<std::string> Shaders;
		std::vector<HazardRenderer::PushConstantRange> Constants;
		std::vector<HazardRenderer::DescriptorSetLayout> Layouts;
	};

	struct ShaderPropertyElement
	{
		std::string Name;
		std::string Prefix;
		std::string Type;
	};

	struct ShaderSourcePropertyBlock
	{
		std::string Name;
		std::string Type;
		std::string Source;

		std::vector<ShaderPropertyElement> Elements;
	};

	//TODO: Make this instanced
	class ShaderCompiler
	{
	public:
		//Only accepts GLSL code for now
		static std::string GetShaderFromSource(uint32_t type, const std::string& source, HazardRenderer::RenderAPI api);

		static std::unordered_map<uint32_t, std::string> GetShaderSources(const std::filesystem::path& path);
		static std::unordered_map<uint32_t, std::string> GetShaders(const std::string& source, const std::filesystem::path& relativePath);

		static ShaderParseFileResult ParseShaderFile(const std::filesystem::path& path);

	private:

		static std::unordered_map<std::string, ShaderCompilerSourceScope> ParseShaderObject(const std::string& source);
		static std::string GenerateShader(const ShaderCompilerSourceScope& scope, std::vector<ShaderProperty>& layouts, std::vector<ShaderProperty>& constants);

		static std::vector<ShaderProperty> GetLayoutFromProperties(const std::string& source);
		static std::vector<HazardRenderer::DescriptorSetLayout> GenerateDescriptorLayouts(const std::vector<ShaderProperty>& properties);

		static std::string VerifyEncapsulationWith(const std::string& value, char c);

		static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
		static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);

		static std::vector<ShaderSourcePropertyBlock> GetShaderPropertyBlocks(const std::string& shaderSource);
		static std::vector<ShaderPropertyElement> ParseElements(const std::string& source);
		static std::string GeneratePropertyBlockSource(const ShaderSourcePropertyBlock& block);

		static void ProcessUniformBlocks(std::string& source, std::vector<ShaderProperty>& properties, uint32_t stageFlags);
		static void ProcessPushConstantBlock(std::string& source, std::vector<ShaderProperty>& constants, uint32_t stageFlags);
		static std::vector<HazardRenderer::PushConstantRange> GeneratePushConstants(std::vector<ShaderProperty>& constants);
	};
}
