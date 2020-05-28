#pragma once

#include "Hazard/Core/Core.h"
#include <glad/glad.h>
#include "Hazard/Modules/Rendering/Shaders/Shader.h"

namespace Hazard {
	class HAZARD_API OpenGLShader : public Shader {
	public:
		OpenGLShader(std::string path);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;
		uint32_t GetProgram() const { return program; }

		void SetUniform(const std::string& name, int value);
		void SetUniform(const std::string& name, int* value, uint32_t size);
		void SetUniform(const std::string& name, float value);
		void SetUniform(const std::string& name, Vector2<float> value);
		void SetUniform(const std::string& name, Vector3<float>  value);
		void SetUniform(const std::string& name, Matrix4 value);
		void SetUniform(const std::string& name, bool value);

	private:
		GLint GetLocation(const std::string& name);

		std::unordered_map<GLenum, std::string> Process(std::string source);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);
		GLuint program = 0;
		std::unordered_map <std::string, GLint> locations;
	};
}