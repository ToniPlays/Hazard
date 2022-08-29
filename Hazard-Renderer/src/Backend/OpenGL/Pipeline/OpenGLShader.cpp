
#include "OpenGLShader.h"
#ifdef HZR_INCLUDE_OPENGL

#include "../OpenGLUtils.h"
#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"
#include "Backend/OpenGL/OpenGLShaderCompiler.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::OpenGL
{
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_ASSERT(!filePath.empty(), "Shader path cannot be empty");
		Reload();
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}
	void OpenGLShader::Reload()
	{
		if (ShaderFactory::HasCachedShader(ShaderStage::Vertex, m_FilePath))
		{
			__debugbreak();
			return;
		}

		std::cout << m_FilePath << std::endl;
		std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(m_FilePath);

		//Compile OpenGL shader
		OpenGLShaderCompiler compiler;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries;
		for (auto& [stage, source] : sources)
		{
			double compilationTime = 0.0;
			std::vector<ShaderDefine> defines = { { "Vulkan" } };

			//Compile to Vulkan SPV
			CompileInfo compileInfoVulkan = {};
			compileInfoVulkan.Renderer = RenderAPI::Vulkan;
			compileInfoVulkan.Name = m_FilePath;
			compileInfoVulkan.Stage = stage;
			compileInfoVulkan.Source = source;
			compileInfoVulkan.Defines = defines;

			if (!compiler.Compile(&compileInfoVulkan))
			{
				std::cout << compiler.GetErrorMessage() << std::endl;
				continue;
			}
			compilationTime += compiler.GetCompileTime();
			//Get OpenGL shader source

			std::string glSource;
			if (!compiler.Decompile(compiler.GetCompiledBinary(), glSource))
			{
				std::cout << "Decompilation failed" << std::endl;
				continue;
			}

			std::vector<ShaderDefine> glDefines = { { "OpenGL" } };
			//Compile to OpenGL SPV
			CompileInfo compileInfoOpenGL = {};
			compileInfoOpenGL.Renderer = RenderAPI::OpenGL;
			compileInfoOpenGL.Name = m_FilePath;
			compileInfoOpenGL.Stage = stage;
			compileInfoOpenGL.Source = glSource;
			compileInfoOpenGL.Defines = glDefines;

			compilationTime += compiler.GetCompileTime();
			binaries[stage] = compiler.GetCompiledBinary();
			std::cout << "Compilation took: " << compilationTime << "ms" << std::endl;
		}
		Reflect(binaries);

		Ref<OpenGLShader> instance = this;
		Renderer::Submit([instance, binaries]() mutable {
			instance->CreateProgram(binaries);
			});
	}
	bool OpenGLShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		if (!uniformBuffer) return false;
		//HZR_ASSERT(uniformBuffer, "[OpenGLShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
		return true;
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, uint32_t value)
	{
		std::stringstream ss;
		ss << name << "[" << index << "]";

		GLuint location = glGetUniformLocation(m_ID, ss.str().c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, Ref<Image2D> value)
	{
		Set(name, index, index);
	}
	void OpenGLShader::Reflect(std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		Timer timer;
		m_ShaderData.Stages.clear();

		uint32_t uniformCount = 0;

		OpenGLShaderCompiler compiler;
		for (auto&& [stage, binary] : binaries) {


			ShaderStageData shaderStage = compiler.GetResources(binary);

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			m_ShaderData.Stages[stage] = shaderStage;

			for (auto resource : resources.uniform_buffers)
			{
				auto& type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription spec = {};
				spec.Name = resource.name;
				spec.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				spec.MemberCount = type.member_types.size();
				spec.Size = compiler.get_declared_struct_size(type);
				spec.ShaderUsage |= (uint32_t)stage;

				auto it = m_ShaderData.UniformsDescriptions.find(spec.Binding);
				if (it != m_ShaderData.UniformsDescriptions.end())
				{
					auto& buffer = m_ShaderData.UniformsDescriptions[spec.Binding];
					buffer.ShaderUsage |= (uint32_t)stage;
					continue;
				}

				m_ShaderData.UniformsDescriptions[spec.Binding] = spec;
			}
		}

		for (auto& [binding, spec] : m_ShaderData.UniformsDescriptions)
		{
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = spec.Name;
			bufferInfo.Binding = spec.Binding;
			bufferInfo.Size = spec.Size;
			bufferInfo.Usage = spec.ShaderUsage;

			m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
		}
		std::cout << "Reflection took: " << timer.ElapsedMillis() << "ms" << std::endl;
	}
	void OpenGLShader::Reload_RT(std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		CreateProgram(binaries);
	}
	static void CheckShader(GLuint id, GLuint type)
	{
		GLint ret = 0;
		//Check if something is wrong with the shader
		switch (type) {
		case(GL_COMPILE_STATUS):
			glGetShaderiv(id, type, &ret);
			if (ret == false) {
				int infologLength = 0;
				glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				std::vector<GLchar> infoLog(infologLength);
				GLsizei charsWritten = 0;

				glGetShaderInfoLog(id, infologLength, &charsWritten, infoLog.data());
				Window::SendDebugMessage({ Severity::Error, "Shader compilation failed", std::string(infoLog.data()) });
			}
			break;
		case(GL_LINK_STATUS):
			glGetProgramiv(id, type, &ret);
			if (ret == false) {
				int infologLength = 0;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infologLength);
				std::vector<GLchar> infoLog(infologLength);
				GLsizei charsWritten = 0;

				glGetProgramInfoLog(id, infologLength, &charsWritten, infoLog.data());
				Window::SendDebugMessage({ Severity::Error, "Shader linking failed", std::string(infoLog.data()) });
			}
			break;
		default:
			break;
		};
	}

	void OpenGLShader::CreateProgram(const std::unordered_map<ShaderStage, std::vector<uint32_t>>& binary)
	{
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : binary)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(OpenGLUtils::ShaderStageToGLType(stage)));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
			CheckShader(program, shaderID);
		}
		glLinkProgram(program);

		CheckShader(program, GL_LINK_STATUS);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (linked == GL_FALSE)
		{
			GLint maxLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);

			std::vector<GLchar> infoLog(maxLen);
			glGetShaderInfoLog(program, maxLen, &maxLen, infoLog.data());
			Window::SendDebugMessage({ Severity::Error, "Shader linking failed", std::string(infoLog.data()) });
			glDeleteProgram(program);

			for (auto id : shaderIDs)
				glDeleteShader(id);
		}

		for (auto id : shaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_ID = program;
	}
}
#endif
