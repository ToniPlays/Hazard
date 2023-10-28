#pragma once

#include "Core/Core.h"
#ifdef HZR_DESKTOP

#include "Core/Rendering/Shader.h"
#include "Core/RenderContextCreateInfo.h"
#include "Core/Rendering/Shader.h"

#include <spirv_cross/spirv_reflect.hpp>

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

	class ShaderCompiler
	{
	public:
		//Only accepts GLSL code for now
		static std::string GetShaderFromSource(uint32_t type, const std::string& source, HazardRenderer::RenderAPI api);
		//static uint64_t GetBinaryLength(const std::vector<ShaderStageCode>& binaries);
		static std::unordered_map<uint32_t, std::string> GetShaderSources(const std::filesystem::path& path);

	private:
		static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
		static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);
	};
}
#endif
