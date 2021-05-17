#pragma once
#include <hzrpch.h>
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Hazard/File/File.h"
#include "Hazard/Rendering/RenderUtils.h"
#include "Hazard/Rendering/Shader/ShaderCompiler.h"

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

	OpenGLShader::OpenGLShader(ShaderCreateInfo info) 
	{
		m_Info.shaderName = info.shaderName;
		Compile(info.stages);
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
	/*std::unordered_map<GLenum, std::string> OpenGLShader::Process(std::string source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos) {

			size_t eol = source.find_first_of("\r\n", pos);
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] =
				source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}*/

	void OpenGLShader::Compile(std::vector<ShaderStage> stages)
	{
		m_ProgramID = glCreateProgram();

		int glShaderIDIndex = 0;
		std::vector<uint32_t> shaderID(stages.size());

		for (auto stage : stages)
		{
			GLuint shaderType = ShaderTypeFromType(stage.type);
			GLuint shader = glCreateShader(shaderType);
			shaderID.push_back(shader);

			if (stage.fileType == ShaderFileType::Source) {

				std::string source = File::ReadFile(stage.filename);
				const GLchar* shaderSource = source.c_str();

				glShaderSource(shader, 1, &shaderSource, 0);
				glCompileShader(shader);
			}
			else 
			{
				if (stage.forceCompile)
				{
					if (!ShaderCompiler::CompileShader(stage.filename, stage.filename + ".spv")) {
						HZR_CORE_FATAL("Failed to compile shader binary");
					}
				}
				std::vector<char> binary = File::ReadBinaryFile(stage.filename + ".spv");
				glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, binary.data(), binary.size());
				glSpecializeShader(shader, "main", 0, 0, 0);
			}

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