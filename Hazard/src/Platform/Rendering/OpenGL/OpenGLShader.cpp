#pragma once

#include <hzrpch.h>
#include "OpenGLShader.h"
#include "Hazard/Rendering/RenderEngine.h"
#include "OpenGLUtils.h"

#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazard::Rendering::OpenGL
{
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_PROFILE_FUNCTION();

		auto shaderSources = ShaderFactory::GetShaderSources(m_FilePath);
		{
			Timer timer;
			bool wasCompiled = CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries(wasCompiled);
			CreateProgram();
			HZR_CORE_TRACE("Shader creation took {0} ms", timer.ElapsedMillis());
		}

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath)
	{
		std::unordered_map<ShaderType, std::string> sources;

		sources[ShaderType::Vertex] = vertexPath;
		sources[ShaderType::Fragment] = fragmenPath;
		{
			Timer timer;
			bool wasCompiled = CompileOrGetVulkanBinaries(sources);
			CompileOrGetOpenGLBinaries(wasCompiled);
			CreateProgram();
			HZR_CORE_TRACE("Shader creation took {0} ms", timer.ElapsedMillis());
		}
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}
	void OpenGLShader::Reload()
	{

	}
	void OpenGLShader::Bind()
	{
		glUseProgram(m_ID);
	}
	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetUniformBuffer(const std::string& name, void* data)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		HZR_CORE_ASSERT(uniformBuffer, "[OpenGLShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data);
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, uint32_t value)
	{
		std::stringstream ss;
		ss << name << "[" << index << "]";

		GLuint location = glGetUniformLocation(m_ID, ss.str().c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::Set(const std::string& name, uint32_t index, Ref<Texture2D>& value)
	{
		Set(name, index, index);
	}
	void OpenGLShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_TRACE("Reflecting shader {0}", m_FilePath);
		m_ShaderData.Stages.clear();

		uint32_t uniformCount = 0;

		for (auto&& [stage, binary] : m_OpenGLSPIRV) {

			ShaderStageData shaderStage = ShaderFactory::GetShaderResources(binary);

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			m_ShaderData.Stages[OpenGLUtils::ShaderTypeFromGLType(stage)] = shaderStage;

			for (auto resource : resources.uniform_buffers)
			{
				auto type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription desc = {};
				desc.Name = resource.name;
				desc.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				desc.MemberCount = type.member_types.size();
				desc.Size = compiler.get_declared_struct_size(type);
				desc.ShaderUsage |= (int)stage;

				auto it = m_UniformBuffers.find(desc.Name);
				if (it != m_UniformBuffers.end())
				{
					auto& buffer = m_UniformBuffers[desc.Name];
					HZR_CORE_ASSERT(desc.Binding == buffer->GetBinding(), "OpenGL UniformBuffer Binding missmatch for name: {0}", desc.Name);
					continue;
				}

				m_ShaderData.UniformsDescriptions[desc.Binding] = desc;

				UniformBufferCreateInfo bufferInfo = {};
				bufferInfo.Name = desc.Name;
				bufferInfo.Binding = desc.Binding;
				bufferInfo.Size = desc.Size;

				m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
			}
		}

		Utils::PrintReflectResults(m_FilePath, m_ShaderData);
	}
	bool OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		HZR_PROFILE_FUNCTION();
		bool wasCompiled = false;
		m_ID = glCreateProgram();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {
			HZR_PROFILE_FUNCTION("Shader stage");

			auto binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::Vulkan);

			if (binaries.size() > 0) {
				shaderData[stage] = binaries;
				continue;
			}

			std::vector<ShaderDefine> defines = { { "RENDERER_VULKAN" } };

			CompileInfo compileInfo = {};
			compileInfo.Path = m_FilePath;
			compileInfo.Environment = RenderAPI::Vulkan;
			compileInfo.Optimization = Optimization::None;
			compileInfo.Source = source;
			compileInfo.Stage = stage;
			compileInfo.Defines = defines;

			ShaderFactory::Compile(&compileInfo);

			if (!compileInfo.Succeeded()) {
				HZR_CORE_ERROR(compileInfo.Error);
				auto binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::OpenGL);
				shaderData[stage] = binaries;
				continue;
			}
			wasCompiled = true;
			shaderData[stage] = compileInfo.Binary;
		}
		return wasCompiled;
	}
	void OpenGLShader::CompileOrGetOpenGLBinaries(bool forceCompile)
	{
		HZR_PROFILE_FUNCTION();
		auto& shaderData = m_OpenGLSPIRV;

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			HZR_PROFILE_FUNCTION("Shader stage");
			GLenum glStage = OpenGLUtils::ShaderTypeToGLType(stage);

			if (!forceCompile) {

				auto& binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::OpenGL);

				if (binaries.size() > 0) {
					shaderData[glStage] = binaries;
					continue;
				}
			}

			std::vector<ShaderDefine> defines = { { "RENDERER_OPENGL" } };

			CompileInfo compileInfo = {};
			compileInfo.Environment = RenderAPI::OpenGL;
			compileInfo.Optimization = Optimization::None;
			compileInfo.Path = m_FilePath;
			compileInfo.Stage = stage;
			compileInfo.Binary = spirv;
			compileInfo.Defines = defines;

			ShaderFactory::Compile(&compileInfo);

			if (!compileInfo.Succeeded()) {
				HZR_CORE_ERROR(compileInfo.Error);
			}

			shaderData[glStage] = compileInfo.Binary;
		}
		Reflect();
	}
	void OpenGLShader::CreateProgram()
	{
		HZR_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : m_OpenGLSPIRV)
		{
			GLuint shaderID = shaderIDs.emplace_back(glCreateShader(stage));
			glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
			glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
			glAttachShader(program, shaderID);
		}
		glLinkProgram(program);

		GLint linked;
		glGetProgramiv(program, GL_LINK_STATUS, &linked);

		if (linked == GL_FALSE) {
			GLint maxLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLen);

			std::vector<GLchar> infoLog(maxLen);
			glGetShaderInfoLog(program, maxLen, &maxLen, infoLog.data());
			HZR_CORE_ERROR("[OpenGLShader]: Shader linking failed {0}", infoLog.data());
			glDeleteProgram(program);

			for (auto id : shaderIDs) {
				glDeleteShader(id);
			}
		}

		for (auto id : shaderIDs) {
			glDetachShader(program, id);
			glDeleteShader(id);
		}
		m_ID = program;
	}
}