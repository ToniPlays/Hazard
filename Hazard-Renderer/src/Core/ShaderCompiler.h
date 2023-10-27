#pragma once

#include "Core/Core.h"
#ifdef HZR_DESKTOP

#include "Core/Rendering/Shader.h"
#include "Core/RenderContextCreateInfo.h"

#include <spirv_cross/spirv_reflect.hpp>

namespace HazardRenderer
{
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
			std::string result;
			if (flags & SHADER_STAGE_VERTEX_BIT)	result += " Vertex";
			if (flags & SHADER_STAGE_FRAGMENT_BIT)	result += " Fragment";
			if (flags & SHADER_STAGE_COMPUTE_BIT)	result += " Compute";
			if (flags & SHADER_STAGE_GEOMETRY_BIT)	result += " Geometry";
			return result;
		}
	}

	enum class Optimization { None = 0, Memory, Performance };

	struct ShaderCode
	{
		uint32_t Stage;
		uint64_t Length;
	};

	struct ShaderDefine
	{
		std::string Name;
		std::string Value;

		ShaderDefine(std::string name) : Name(name), Value("") {};
		ShaderDefine(std::string name, std::string value) : Name(name), Value(value) {};
	};

	struct CompileInfo
	{
		RenderAPI Renderer;
		Optimization Optimization;
		uint32_t Stage;
		std::string Source;
		std::string Name;
		uint64_t DefineCount = 0;
		ShaderDefine* pDefines = nullptr;
	};

    
	class ShaderCompiler
	{
	public:
		//Only accepts GLSL code for now
		static std::string GetShaderFromSource(uint32_t type, const std::string& source, RenderAPI api);
		static uint64_t GetBinaryLength(const std::vector<ShaderStageCode>& binaries);
		static std::unordered_map<uint32_t, std::string> GetShaderSources(const std::filesystem::path& path);

	private:
		static bool PreprocessSource(const std::filesystem::path& path, std::string& shaderSource);
		static bool PreprocessIncludes(const std::filesystem::path& path, std::string& source);
	};
}
#endif
