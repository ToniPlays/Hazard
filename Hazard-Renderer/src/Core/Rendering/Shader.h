#pragma once

#include "Core/Core.h"
#include "ShaderDataType.h"
#include "Core/RTCore/AccelerationStructure.h"

#include "Buffer/Buffer.h"

#include <unordered_map>
#include <vector>
#include <Utility/StringUtil.h>

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
		SHADER_STAGE_ANY_HIT_BIT = BIT(7),

		SHADER_STAGE_ALL_GRAPHICS = SHADER_STAGE_VERTEX_BIT | SHADER_STAGE_FRAGMENT_BIT,
	};

	enum ResourceAccessFlags : uint32_t
	{
		SHADER_ACCESS_READ = BIT(0),
		SHADER_ACCESS_WRITE = BIT(1),
	};

	static uint32_t ShaderStageFlagsFromString(const std::string& type)
	{
		std::string val = type;
		StringUtil::ToLower(val);

		if (val == "vertex")		return SHADER_STAGE_VERTEX_BIT;
		if (val == "fragment")		return SHADER_STAGE_FRAGMENT_BIT;
		if (val == "pixel")			return SHADER_STAGE_FRAGMENT_BIT;
		if (val == "compute")		return SHADER_STAGE_COMPUTE_BIT;
		if (val == "geometry")		return SHADER_STAGE_GEOMETRY_BIT;
		if (val == "raygen")		return SHADER_STAGE_RAYGEN_BIT;
		if (val == "miss")			return SHADER_STAGE_MISS_BIT;
		if (val == "closesthit")	return SHADER_STAGE_CLOSEST_HIT_BIT;
		if (val == "anyhit")		return SHADER_STAGE_ANY_HIT_BIT;

		return SHADER_STAGE_NONE;
	}

	static std::string ShaderStageFlagsToString(uint32_t flag)
	{
		if (flag & SHADER_STAGE_NONE) return "SHADER_STAGE_NONE";
		if (flag & SHADER_STAGE_VERTEX_BIT) return "SHADER_STAGE_VERTEX_BIT";
		if (flag & SHADER_STAGE_FRAGMENT_BIT) return "SHADER_STAGE_FRAGMENT_BIT";
		if (flag & SHADER_STAGE_COMPUTE_BIT) return "SHADER_STAGE_COMPUTE_BIT";
		if (flag & SHADER_STAGE_GEOMETRY_BIT) return "SHADER_STAGE_GEOMETRY_BIT";
		if (flag & SHADER_STAGE_RAYGEN_BIT) return "SHADER_STAGE_RAYGEN_BIT";
		if (flag & SHADER_STAGE_MISS_BIT) return "SHADER_STAGE_MISS_BIT";
		if (flag & SHADER_STAGE_CLOSEST_HIT_BIT) return "SHADER_STAGE_CLOSEST_HIT_BIT";
		if (flag & SHADER_STAGE_ANY_HIT_BIT) return "SHADER_STAGE_ANY_HIT_BIT";
		if (flag & SHADER_STAGE_ALL_GRAPHICS) return "SHADER_STAGE_ALL_GRAPHICS";

		return "UNKNOWN";
	}

	class Shader : public RefCount
	{
	public:
		virtual ~Shader() = default;
		virtual void Reload() = 0;

		virtual std::unordered_map<uint32_t, Buffer> GetShaderCode() const = 0;
		virtual void SetShaderCode(const std::unordered_map<uint32_t, Buffer>& shaderCode) = 0;

		static Ref<Shader> Create(const std::string& debugName, const std::unordered_map<uint32_t, std::string>& shaderModules);
	};
}
