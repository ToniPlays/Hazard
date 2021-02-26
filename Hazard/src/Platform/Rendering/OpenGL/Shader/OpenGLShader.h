#pragma once

#include "Hazard/Core/Core.h"
#include <glad/glad.h>

#include "Hazard/Rendering/Shader/Shader.h"
#include <unordered_map>
#include "glm/glm.hpp"

namespace Hazard::Rendering {
	class HAZARD_API OpenGLShader : public Shader {
	public:
		OpenGLShader(std::string path);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, int* value, uint32_t size);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, glm::vec2 value);
		void SetUniform(const std::string& name, glm::vec3  value);
		void SetUniform(const std::string& name, glm::mat4 value);
		void SetUniform(const std::string& name, Color color);
		void SetUniform(const std::string& name, bool value);

		uint32_t GetProgram() const { return program; }

	private:
		GLint GetLocation(const std::string& name);
		std::unordered_map<GLenum, std::string> Process(std::string source);
		GLuint program = 0;
		std::unordered_map <std::string, GLint> locations;

		void Compile(const std::unordered_map<GLenum, std::string>& sources);
	};
}