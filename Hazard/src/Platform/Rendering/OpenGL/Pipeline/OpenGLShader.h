#pragma once

#include "Hazard/Core/Core.h"
#include <glad/glad.h>

#include "Hazard/Rendering/Pipeline/Shader.h"
#include <unordered_map>
#include "glm/glm.hpp"

namespace Hazard::Rendering::OpenGL {

	class OpenGLShader : public Shader {
	public:
		OpenGLShader(const std::string& name, uint32_t stageCount, PipelineShaderStage* stages);
		~OpenGLShader();

		void Bind() const;
		void Unbind() const;

		void SetUniformInt(const std::string& name, int value);
		void SetUniformIntArray(const std::string& name, int* value, uint32_t size);
		void SetUniformFloat(const std::string& name, float value);
		void SetUniformVec2(const std::string& name, glm::vec2 value);
		void SetUniformVec3(const std::string& name, glm::vec3  value);
		void SetUniformMat4(const std::string& name, glm::mat4 value);
		void SetUniformColor(const std::string& name, Color color);
		void SetUniformBool(const std::string& name, bool value);

	private:
		GLint GetLocation(const std::string& name);
		GLint GetBlockLocation(const std::string& name);
		void Compile(uint32_t stageCount, PipelineShaderStage* stages);

	private:
		std::string m_Name;
		GLuint m_ProgramID = 0;
		std::unordered_map <std::string, GLint> m_Locations;

	};
}