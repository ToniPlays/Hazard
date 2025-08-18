#pragma once

#include "Core/Core.h"

#include "Core/Rendering/Shader.h"
#include "ShaderParser/ShaderParser.h"
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

			std::string t = type;
			StringUtil::ToLower(t);

			if (t == "vertex")		return SHADER_STAGE_VERTEX_BIT;
			if (t == "fragment")		return SHADER_STAGE_FRAGMENT_BIT;
			if (t == "pixel")		return SHADER_STAGE_FRAGMENT_BIT;
			if (t == "compute")		return SHADER_STAGE_COMPUTE_BIT;
			if (t == "raygen")		return SHADER_STAGE_RAYGEN_BIT;
			if (t == "miss")			return SHADER_STAGE_MISS_BIT;
			if (t == "closesthit")	return SHADER_STAGE_CLOSEST_HIT_BIT;
			if (t == "anyhit")		return SHADER_STAGE_ANY_HIT_BIT;
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

	//TODO: Make this instanced
	class ShaderCompiler
	{
	public:
		ShaderCompiler(const std::filesystem::path& path) : m_Path(std::filesystem::weakly_canonical(path)) {}
		//Only accepts GLSL code for now
		std::string GetShaderFromSource(uint32_t type, const std::string& source, HazardRenderer::RenderAPI api);
		std::unordered_map<uint32_t, std::string> SplitSource(const std::string& source);

		std::string EmitGLSL(std::shared_ptr<Shading::ATLNode> root);

	private:
		void EmitProperties(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node, uint32_t stageFlags);
		void EmitPushConstants(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node);
		void EmitScope(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node);
		void EmitIncludes(std::ostringstream& out, std::shared_ptr<Shading::ATLNode> node);

		std::string TransformGLSL(const std::string& source);
		std::string PostProcessUniformStructs(const std::string& source);

	private:
		std::filesystem::path m_Path;
	};
}
