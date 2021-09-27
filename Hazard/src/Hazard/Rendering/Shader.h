#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Rendering/Buffers/ShaderDataType.h"

#include <spirv_cross/spirv_reflect.hpp>

namespace Hazard::Rendering
{
	enum ShaderType { Unknown = 0, Vertex, Fragment, Compute, Geometry };

	namespace Utils 
	{
		static ShaderType ShaderTypeFromString(const std::string& type) {
			if (type == "Vertex")		return ShaderType::Vertex;
			if (type == "Fragment")		return ShaderType::Fragment;
			if (type == "Pixel")		return ShaderType::Fragment;
			if (type == "Compute")		return ShaderType::Compute;
			return ShaderType::Unknown;
		}
		static std::string ShaderTypeToString(ShaderType type) {
			if (type == ShaderType::Vertex)			return "Vertex";
			if (type == ShaderType::Fragment)		return "Fragment";
			if (type == ShaderType::Compute)		return "Compute";
			return "Unknown";
		}
		static ShaderDataType ShaderTypeFromSPV(const spirv_cross::SPIRType& type) {
			using namespace spirv_cross;
			switch (type.basetype)
			{
			case SPIRType::Float: {
				switch (type.vecsize)
				{
				case 1: return ShaderDataType::Float;
				case 2: return ShaderDataType::Float2;
				case 3: return ShaderDataType::Float3;
				case 4: return ShaderDataType::Float4;
				}
			case SPIRType::Int: 
				switch (type.vecsize)
				{
				case 1: return ShaderDataType::Int;
				case 2: return ShaderDataType::Int2;
				case 3: return ShaderDataType::Int3;
				case 4: return ShaderDataType::Int4;
				}
			}
			}
		}
	}

	struct ShaderStageInput 
	{
		std::string Name;
		uint32_t Location;
		ShaderDataType Type;
		uint32_t Size;
	};
	struct ShaderStageOutput
	{
		std::string Name;
		uint32_t Location;
		ShaderDataType Type;
		uint32_t Size;
	};
	struct ShaderUniform 
	{			
		std::string Name;
	};

	struct ShaderStageData {
		std::vector<ShaderStageInput> Inputs;
		std::vector<ShaderStageOutput> Outputs;
		std::vector<ShaderUniform> Uniforms;

	};

	class Shader {
	public:

		virtual ~Shader() = default;
		virtual void Reload() = 0;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual std::unordered_map<ShaderType, ShaderStageData> GetShaderData() = 0;


		static Shader* Create(const std::string& path);
		static std::unordered_map<ShaderType, std::string> PreProcess(const std::string& source);
	};
}
