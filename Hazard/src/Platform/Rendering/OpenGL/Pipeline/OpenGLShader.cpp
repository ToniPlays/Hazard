#pragma once
#include <hzrpch.h>
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Hazard/File/File.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/Pipeline/ShaderCompiler.h"

namespace Hazard::Rendering::OpenGL {

	static GLenum ShaderTypeFromType(ShaderType type) {
		if (type == ShaderType::VertexShader) {
			return GL_VERTEX_SHADER;
		}
		else if (type == ShaderType::GeometryShader) {
			return GL_GEOMETRY_SHADER;
		}
		else if (type == ShaderType::FragmentShader) {
			return GL_FRAGMENT_SHADER;
		}
		return -1;
	}

	OpenGLShader::OpenGLShader(const std::string& name, uint32_t stageCount, PipelineShaderStage* stages)
	{
		Vault::Add(name, (Shader*)this);
		Compile(stageCount, stages);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ProgramID);
		RenderUtils::RemoveAsset((Shader*)this);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_ProgramID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	GLint OpenGLShader::GetLocation(const std::string& name)
	{
		if (m_Locations.count(name) > 0) {
			return m_Locations[name];
		}

		GLint location = glGetUniformLocation(m_ProgramID, name.c_str());
		m_Locations[name] = location;

		return location;
	}
	GLint OpenGLShader::GetBlockLocation(const std::string& name)
	{
		if (m_Locations.count(name) > 0) {
			return m_Locations[name];
		}
		GLint location = glGetUniformBlockIndex(m_ProgramID, name.c_str());
		m_Locations[name] = location;
		return location;
	}
	//Uniforms 
	void OpenGLShader::SetUniformInt(const std::string& name, int value)
	{
		glUniform1i(GetLocation(name), value);
	}

	void OpenGLShader::SetUniformIntArray(const std::string& name, int* value, uint32_t size)
	{
		glUniform1iv(GetLocation(name), size, value);
	}

	void OpenGLShader::SetUniformFloat(const std::string& name, float value)
	{
		glUniform1f(GetLocation(name), value);
	}

	void OpenGLShader::SetUniformVec2(const std::string& name, glm::vec2 value)
	{
		glUniform2f(GetLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniformVec3(const std::string& name, glm::vec3 value)
	{
		glUniform3f(GetLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniformMat4(const std::string& name, glm::mat4 value)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}
	void OpenGLShader::SetUniformColor(const std::string& name, Color color) {
		SetUniformVec3(name, { color.r, color.g, color.b });
	}

	void OpenGLShader::SetUniformBool(const std::string& name, bool value)
	{
		SetUniformBool(name, value ? 1 : 0);
	}

	void OpenGLShader::Compile(uint32_t stageCount, PipelineShaderStage* stages)
	{
		m_ProgramID = glCreateProgram();

		int glShaderIDIndex = 0;
		std::vector<uint32_t> shaderID(stageCount);

		for (int i = 0; i < stageCount; i++)
		{

			PipelineShaderStage stage = stages[i];
			HZR_CORE_WARN("Shader: {0}", stage.shaderFileName);

			GLuint shaderType = ShaderTypeFromType(stage.stage);
			GLuint shader = glCreateShader(shaderType);
			shaderID.push_back(shader);

			std::string path = RenderUtils::GetShaderPath(stage.fileType);

			std::string source = File::ReadFile(path + "/" + stage.shaderFileName);
			const GLchar* shaderSource = source.c_str();

			glShaderSource(shader, 1, &shaderSource, 0);
			glCompileShader(shader);
			

			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (compiled == GL_FALSE) {

				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
				std::vector<GLchar> buffer(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &buffer[0]);

				glDeleteShader(shader);
				HZR_CORE_ERROR(buffer.data());
				return;
			}
			glAttachShader(m_ProgramID, shader);
		}
		glLinkProgram(m_ProgramID);

		GLint isLinked = 0;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE) {

			GLint maxLength = 0;
			glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ProgramID, maxLength, &maxLength, &infoLog[0]);
			glDeleteProgram(m_ProgramID);
			HZR_CORE_ERROR(infoLog.data());

			for (auto id : shaderID) {
				glDeleteShader(id);
			}

			return;
		}
		for (auto id : shaderID) {
			glDeleteShader(id);
		}
	}
}