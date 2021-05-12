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

		std::string& GetFile() override { return m_File; }

		void SetUniformInt(const std::string& name, int value) override;
		void SetUniformIntArray(const std::string& name, int* value, uint32_t size) override;
		void SetUniformFloat(const std::string& name, float value) override;
		void SetUniformVec2(const std::string& name, glm::vec2 value) override;
		void SetUniformVec3(const std::string& name, glm::vec3  value) override;
		void SetUniformMat4(const std::string& name, glm::mat4 value) override;
		void SetUniformColor(const std::string& name, Color color) override;
		void SetUniformBool(const std::string& name, bool value) override;

		uint32_t GetProgram() const { return m_ProgramID; }

	private:
		GLint GetLocation(const std::string& name);
		std::unordered_map<GLenum, std::string> Process(std::string source);
		void Compile(const std::unordered_map<GLenum, std::string>& sources);

	private:
		std::string m_File;
		GLuint m_ProgramID = 0;
		std::unordered_map <std::string, GLint> m_Locations;

	};
}