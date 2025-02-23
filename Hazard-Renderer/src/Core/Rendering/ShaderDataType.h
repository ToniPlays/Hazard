#pragma once

#include "Core/Core.h"

#include "DescriptorSetLayout.h"

namespace HazardRenderer
{
	enum class ShaderDataType
	{
		None = 0,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		Int,
		Int2,
		Int3,
		Int4,
		UInt,
		UInt2,
		UInt3,
		UInt4,
		Bool,
		Other,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return (uint32_t)4;
			case ShaderDataType::Float2:   return (uint32_t)4 * 2;
			case ShaderDataType::Float3:   return (uint32_t)4 * 3;
			case ShaderDataType::Float4:   return (uint32_t)4 * 4;
			case ShaderDataType::Mat3:     return (uint32_t)4 * 3 * 3;
			case ShaderDataType::Mat4:     return (uint32_t)4 * 4 * 3;
			case ShaderDataType::Int:      return (uint32_t)4;
			case ShaderDataType::Int2:     return (uint32_t)4 * 2;
			case ShaderDataType::Int3:     return (uint32_t)4 * 3;
			case ShaderDataType::Int4:     return (uint32_t)4 * 4;
			case ShaderDataType::UInt:     return (uint32_t)4;
			case ShaderDataType::UInt2:    return (uint32_t)4 * 2;
			case ShaderDataType::UInt3:    return (uint32_t)4 * 4;
			case ShaderDataType::UInt4:    return (uint32_t)4 * 3;
			case ShaderDataType::Bool:     return (uint32_t)1;
		}
		return 0;
	}

	static int ComponentCount(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 1;
			case ShaderDataType::Float2:   return 2;
			case ShaderDataType::Float3:   return 3;
			case ShaderDataType::Float4:   return 4;
			case ShaderDataType::Mat3:     return 3;
			case ShaderDataType::Mat4:     return 4;
			case ShaderDataType::Int:      return 1;
			case ShaderDataType::Int2:     return 2;
			case ShaderDataType::Int3:     return 3;
			case ShaderDataType::Int4:     return 4;
			case ShaderDataType::UInt:     return 1;
			case ShaderDataType::UInt2:    return 2;
			case ShaderDataType::UInt3:    return 3;
			case ShaderDataType::UInt4:    return 4;
			case ShaderDataType::Bool:     return 1;
		}
		return 0;
	}

	static int ShaderDataTypeLocationSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Mat3:	return 3;
			case ShaderDataType::Mat4:	return 4;
			default:					return 1;
		}
	}

	static std::string ShaderDataTypeToString(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return "Float";
			case ShaderDataType::Float2:   return "Float2";
			case ShaderDataType::Float3:   return "Float3";
			case ShaderDataType::Float4:   return "Float4";
			case ShaderDataType::Mat3:     return "Mat3";
			case ShaderDataType::Mat4:     return "Mat4";
			case ShaderDataType::Int:      return "Int";
			case ShaderDataType::Int2:     return "Int2";
			case ShaderDataType::Int3:     return "Int3";
			case ShaderDataType::Int4:     return "Int4";
			case ShaderDataType::UInt:     return "UInt";
			case ShaderDataType::UInt2:    return "UInt2";
			case ShaderDataType::UInt3:    return "UInt3";
			case ShaderDataType::UInt4:    return "UInt4";
			case ShaderDataType::Bool:     return "Bool";
			case ShaderDataType::Other:     return "Other";
		}
		return "None";
	}
	static ShaderDataType ShaderDataTypeFromString(const std::string& type)
	{
		std::string val = type;
		for (auto& c : val)
			c = tolower(c);

		if (val == "float") return ShaderDataType::Float;
		if (val == "float2") return ShaderDataType::Float2;
		if (val == "float3") return ShaderDataType::Float3;
		if (val == "float4") return ShaderDataType::Float4;
		if (val == "mat3") return ShaderDataType::Mat3;
		if (val == "mat4") return ShaderDataType::Mat4;
		if (val == "int") return ShaderDataType::Int;
		if (val == "int2") return ShaderDataType::Int2;
		if (val == "int3") return ShaderDataType::Int3;
		if (val == "int4") return ShaderDataType::Int4;
		if (val == "uint") return ShaderDataType::UInt;
		if (val == "uint2") return ShaderDataType::UInt2;
		if (val == "uint3") return ShaderDataType::UInt3;
		if (val == "uint4") return ShaderDataType::UInt4;
		if (val == "bool") return ShaderDataType::Bool;
		if (val == "none") return ShaderDataType::None;
		return ShaderDataType::Other;
	}

	static std::string ShaderDataTypeToGLSLString(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return "float";
			case ShaderDataType::Float2:   return "vec2";
			case ShaderDataType::Float3:   return "vec3";
			case ShaderDataType::Float4:   return "vec4";
			case ShaderDataType::Mat3:     return "mat3";
			case ShaderDataType::Mat4:     return "mat4";
			case ShaderDataType::Int:      return "unt";
			case ShaderDataType::Int2:     return "int2";
			case ShaderDataType::Int3:     return "int3";
			case ShaderDataType::Int4:     return "int4";
			case ShaderDataType::UInt:     return "uint";
			case ShaderDataType::UInt2:    return "uint2";
			case ShaderDataType::UInt3:    return "uint3";
			case ShaderDataType::UInt4:    return "uint4";
			case ShaderDataType::Bool:     return "bool";
			case ShaderDataType::Other:    return "";
		}
		return "None";
	}


	static std::string GetShaderDescriptorType(uint32_t type)
	{
		if (type & DESCRIPTOR_TYPE_SAMPLER_2D) return "sampler2D";
		if (type & DESCRIPTOR_TYPE_SAMPLER_CUBE) return "samplerCube";
		if (type & DESCRIPTOR_TYPE_UNIFORM_BUFFER) return "uniformBuffer";
		if (type & DESCRIPTOR_TYPE_STORAGE_IMAGE) return "imageCube";

		return "";
	}

	static uint32_t GetShaderDescriptorType(const std::string& type)
	{
		if (type == "Sampler2D")				return DESCRIPTOR_TYPE_SAMPLER_2D;
		if (type == "SamplerCube")				return DESCRIPTOR_TYPE_SAMPLER_CUBE;
		if (type == "ImageCube")				return DESCRIPTOR_TYPE_STORAGE_IMAGE;
		if (type == "Buffer")					return DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		if (type == "StorageBuffer")			return DESCRIPTOR_TYPE_STORAGE_BUFFER;
		if (type == "AccelerationStructure")	return DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE;

		return 0;
	}
}
