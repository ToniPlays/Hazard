
#include "OpenGLShader.h"
#ifdef HZR_INCLUDE_OPENGL

#include "../OpenGLUtils.h"
#include "Backend/Core/Pipeline/ShaderFactory.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::OpenGL
{
	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		ASSERT(!filePath.empty(), "Shader path cannot be empty");
		auto shaderSources = ShaderFactory::GetShaderSources(m_FilePath);
		{
			bool wasCompiled = CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries(wasCompiled);
			CreateProgram();
		}

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath)
	{
		std::unordered_map<ShaderType, std::string> sources;

		sources[ShaderType::Vertex] = vertexPath;
		sources[ShaderType::Fragment] = fragmenPath;
		{
			bool wasCompiled = CompileOrGetVulkanBinaries(sources);
			CompileOrGetOpenGLBinaries(wasCompiled);
			CreateProgram();
		}
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_ID);
	}
	void OpenGLShader::Reload()
	{

	}
	void OpenGLShader::Bind(Ref<RenderCommandBuffer> cmdBuffer)
	{
		glUseProgram(m_ID);
	}
	void OpenGLShader::Unbind()
	{
		glUseProgram(0);
	}
	void OpenGLShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		ASSERT(uniformBuffer, "[OpenGLShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
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
		m_ShaderData.Stages.clear();

		uint32_t uniformCount = 0;

		for (auto&& [stage, binary] : m_OpenGLSPIRV) {

			ShaderStageData shaderStage = ShaderFactory::GetShaderResources(binary);

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			m_ShaderData.Stages[OpenGLUtils::ShaderTypeFromGLType(stage)] = shaderStage;

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

		for (auto& [binding, spec] : m_ShaderData.UniformsDescriptions) {
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = spec.Name;
			bufferInfo.Binding = spec.Binding;
			bufferInfo.Size = spec.Size;
			bufferInfo.Usage = spec.ShaderUsage;

			m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
		}

		Utils::PrintReflectResults(m_FilePath, m_ShaderData);
	}
	bool OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		bool wasCompiled = false;
		m_ID = glCreateProgram();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {

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
				Window::SendDebugMessage({ Severity::Error, compileInfo.Error });
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
		auto& shaderData = m_OpenGLSPIRV;

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV)
		{
			GLenum glStage = OpenGLUtils::ShaderTypeToGLType(stage);

			if (!forceCompile) {

				const auto& binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::OpenGL);

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

			ASSERT(compileInfo.Succeeded(), "Failed compilation");

			shaderData[glStage] = compileInfo.Binary;
		}
		Reflect();
	}
	void OpenGLShader::CreateProgram()
	{
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
			ASSERT(false, "[OpenGLShader]: Shader linking failed {0}", infoLog.data());
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
#endif
