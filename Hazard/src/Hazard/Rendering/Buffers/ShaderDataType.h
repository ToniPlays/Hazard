#pragma once

#include "Hazard/Core/Core.h"

namespace Hazard::Rendering
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
		}
		return 0;
	}
}