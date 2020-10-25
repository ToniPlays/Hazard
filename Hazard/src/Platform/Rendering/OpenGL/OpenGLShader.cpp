#pragma once
#include <hzrpch.h>
#include "OpenGLShader.h"
#include "glm/gtc/type_ptr.hpp"
#include "Hazard/Modules/Renderer/RenderEngine.h"


namespace Hazard {

	static GLenum ShaderTypeFromString(const std::string& type) {
		if (type == "Vertex") {
			return GL_VERTEX_SHADER;
		}
		else if (type == "Geometry") {
			return GL_GEOMETRY_SHADER;
		}
		else if (type == "Fragment" || type == "Pixel") {
			return GL_FRAGMENT_SHADER;
		}
		return -1;
	}

	OpenGLShader::OpenGLShader(std::string path) {
		std::string file = File::ReadFile(path);
		Compile(Process(file));
		
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(program);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(program);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}
	GLint OpenGLShader::GetLocation(const std::string& name)
	{
		if (locations.count(name) > 0) {
			return locations[name];
		}

		GLint location = glGetUniformLocation(program, name.c_str());
		locations[name] = location;
		return location;
	}
	//Uniforms 
	void OpenGLShader::SetUniform(const std::string& name, int value)
	{
		glUniform1i(GetLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, int* value, uint32_t size)
	{
		glUniform1iv(GetLocation(name), size, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, float value)
	{
		glUniform1f(GetLocation(name), value);
	}

	void OpenGLShader::SetUniform(const std::string& name, Vector2<float> value)
	{
		glUniform2f(GetLocation(name), value.x, value.y);
	}

	void OpenGLShader::SetUniform(const std::string& name, Vector3<float> value)
	{
		glUniform3f(GetLocation(name), value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform(const std::string& name, Matrix4 value)
	{
		//glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, value.GetAll());
	}
	void OpenGLShader::SetUniform(const std::string& name, Color color) {
		SetUniform(name, { color.r, color.g, color.b });
	}

	void OpenGLShader::SetUniform(const std::string& name, bool value)
	{
		SetUniform(name, value ? 1 : 0);
	}


	void OpenGLShader::SetUniform(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
	}


	std::unordered_map<GLenum, std::string> OpenGLShader::Process(std::string source)
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
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& sources)
	{
		program = glCreateProgram();
		std::vector<GLenum> shaderId(sources.size());
		int glShaderIDIndex = 0;

		for (auto& kv : sources) {
			GLenum type = kv.first;
			const GLchar* source = kv.second.c_str();
			if (type == -1) 
				continue;

			GLuint shader = glCreateShader(type);
			glShaderSource(shader, 1, &source, 0);

			glCompileShader(shader);
			GLint compiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

			if (compiled == GL_FALSE) {
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);
				HZR_CORE_ERROR(infoLog.data());
				return;
			}
			glAttachShader(program, shader);
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);

		if (isLinked == GL_FALSE) {

			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			HZR_CORE_ERROR(infoLog.data());

			for (auto id : shaderId)
				glDeleteShader(id);

			return;
		}
		for (auto id : shaderId)
		{
			//glDetachShader(program, id);
			//glDeleteShader(id);
		}
	}
}