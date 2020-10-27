#pragma once

#include "Hazard/Core/Core.h"
#include "Hazard/Utils/Maths/Vector/Matrix4.h"
#include "Hazard/Utils/Maths/Vector/Vector2.h"
#include "Hazard/Utils/Maths/Vector/Vector3.h"
#include "Hazard/Utils/Maths/Color.h"

#include "glm/glm.hpp"

namespace Hazard {
	class HAZARD_API Shader {

	public:
		Shader(std::string f) : file(f) {};
		virtual ~Shader() {};
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetProgram() const = 0;

	public:
		virtual void SetUniform(const std::string& name, int value) = 0;
		virtual void SetUniform(const std::string& name, int* value, uint32_t size) = 0;
		virtual void SetUniform(const std::string& name, float value) = 0;
		virtual void SetUniform(const std::string& name, Vector2<float> value) = 0;
		virtual void SetUniform(const std::string& name, Vector3<float>  value) = 0;
		virtual void SetUniform(const std::string& name, Matrix4 value) = 0;
		virtual void SetUniform(const std::string& name, Color color) = 0;
		virtual void SetUniform(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetUniform(const std::string& name, bool value) = 0;
	
		std::string file;
	};
}