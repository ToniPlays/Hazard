#pragma once

#include <hzrpch.h>
#include "Hazard/Core/Core.h"
#include "Hazard/Math/Color.h"

#include "glm/glm.hpp"

namespace Hazard::Rendering {

	class HAZARD_API Shader {

	public:
		Shader() {};
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetProgram() const = 0;

	public:
		virtual void SetUniform(const std::string& name, int value) = 0;
		virtual void SetUniform(const std::string& name, int* value, uint32_t size) = 0;
		virtual void SetUniform(const std::string& name, float value) = 0;
		virtual void SetUniform(const std::string& name, glm::vec2 value) = 0;
		virtual void SetUniform(const std::string& name, glm::vec3  value) = 0;
		virtual void SetUniform(const std::string& name, glm::mat4 value) = 0;
		virtual void SetUniform(const std::string& name, Color color) = 0;
		virtual void SetUniform(const std::string& name, bool value) = 0;
	};
}