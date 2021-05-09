#pragma once

#include "Hazard/Core/Core.h"
#include <glad/glad.h>

#include "Hazard/Rendering/Shader/Shader.h"
#include <unordered_map>
#include "glm/glm.hpp"

namespace Hazard::Rendering::OpenGL {
	class  OpenGLShader : public Shader {
	public:
		OpenGLShader(std::string path);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		const char* GetFile() const override { return file.c_str(); }

		void SetUniformInt(const std::string& name, int value) override;
		void SetUniformIntArray(const std::string& name, int* value, uint32_t size) override;
		void SetUniformFloat(const std::string& name, float value) override;
		void SetUniformVec2(const std::string& name, glm::vec2 value) override;
		void SetUniformVec3(const std::string& name, glm::vec3  value) override;
		void SetUniformMat4(const std::string& name, glm::mat4 value) override;
		void SetUniformColor(const std::string& name, Color color) override;
		void SetUniformBool(const std::string& name, bool value) override;

		uint32_t GetProgram() const { return program; }

	private:

		std::string file;

		GLint GetLocation(const std::string& name);
		std::unordered_map<GLenum, std::string> Process(std::string source);
		GLuint program = 0;
		std::unordered_map <std::string, GLint> locations;

		void Compile(const std::unordered_map<GLenum, std::string>& sources);
	};
}