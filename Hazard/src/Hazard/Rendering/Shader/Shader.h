#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"

#include "glm/glm.hpp"

namespace Hazard::Rendering {

	enum ShaderType { VertexShader = 0, FragmentShader = 1, GeometryShader = 2, ComputeShader = 3 };

	struct ShaderStage {
		ShaderType type;
		std::string filename;

		ShaderStage() = default;
		ShaderStage(ShaderType type, const std::string& filename) {
			this->type = type;
			this->filename = filename;
		}
	};

	struct ShaderCreateInfo {
		std::string shaderName;
		std::vector<ShaderStage> stages;
	};
	struct ShaderInfo {
		std::string shaderName;
	};
	
	class Shader : public RefCount {

	public:
		Shader() {};
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual ShaderInfo GetInfo() const = 0;

	public:
		virtual void SetUniformInt(const std::string& name, int value) = 0;
		virtual void SetUniformIntArray(const std::string& name, int* value, uint32_t size) = 0;
		virtual void SetUniformFloat(const std::string& name, float value) = 0;
		virtual void SetUniformVec2(const std::string& name, glm::vec2 value) = 0;
		virtual void SetUniformVec3(const std::string& name, glm::vec3  value) = 0;
		virtual void SetUniformMat4(const std::string& name, glm::mat4 value) = 0;
		virtual void SetUniformColor(const std::string& name, Color color) = 0;
		virtual void SetUniformBool(const std::string& name, bool value) = 0;
	};
}