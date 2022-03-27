#pragma once

#include "Backend/Core/Core.h"

namespace HazardRenderer
{
	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
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
		case ShaderDataType::Bool:     return (uint32_t)1;
        case ShaderDataType::None:     return (uint32_t)0;
		}
		return 0;
	}
	static int ComponentCount(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:    return 1;
		case ShaderDataType::Float2:   return 2;
		case ShaderDataType::Float3:   return 3;
		case ShaderDataType::Float4:   return 3;
		case ShaderDataType::Mat3:     return 3;
		case ShaderDataType::Mat4:     return 4;
		case ShaderDataType::Int:      return 1;
		case ShaderDataType::Int2:     return 2;
		case ShaderDataType::Int3:     return 3;
		case ShaderDataType::Int4:     return 4;
		case ShaderDataType::Bool:     return 1;
        case ShaderDataType::None:     return (uint32_t)0;
		}
		return 0;
	}
	static std::string ShaderDataTypeToString(ShaderDataType type) {
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
		case ShaderDataType::Bool:     return "Bool";
        case ShaderDataType::None:     return "None";
		}
		return "";
	}
}
