#pragma once

#include <hzrpch.h>
#include "OpenGLShader.h"
#include "Hazard/Rendering/RenderEngine.h"

#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazard::Rendering::OpenGL
{
	namespace GLUtils {

		static GLenum ShaderTypeToGLType(ShaderType type) {
			switch (type)
			{
			case Vertex:	return GL_VERTEX_SHADER;
			case Fragment:	return GL_FRAGMENT_SHADER;
			case Compute:	return GL_COMPUTE_SHADER;
			case Geometry:	return GL_GEOMETRY_SHADER;
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown ShaderType {0}", Utils::ShaderTypeToString(type));
			return 0;
		}
		static ShaderType ShaderTypeFromGLType(GLenum type) {
			switch (type)
			{
			case GL_VERTEX_SHADER:		return Vertex;
			case GL_FRAGMENT_SHADER:	return Fragment;
			case GL_COMPUTE_SHADER:		return Compute;
			case GL_GEOMETRY_SHADER:	return Geometry;
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown conversion from glType to ShaderType");
			return ShaderType::Unknown;
		}
		static std::string GLTypeToString(GLenum type) {
			switch (type)
			{
			case GL_VERTEX_SHADER:		return "Vertex";
			case GL_FRAGMENT_SHADER:	return "Fragment";
			case GL_COMPUTE_SHADER:		return "Compute";
			case GL_GEOMETRY_SHADER:	return "Geometry";
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unknown conversion from GLenum to string");
			return "";
		}

		static std::string GLShaderStageCachedVulkanExtension(ShaderType type) {
			switch (type)
			{
			case Vertex:	return ".cached_vulkan.vert";
			case Fragment:	return ".cached_vulkan.frag";
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown Vulkan cache filename for {0}", Utils::ShaderTypeToString(type));
			return "";
		}
		static std::string GLShaderStageCachedOpenGLExtension(GLenum type) {
			switch (type)
			{
			case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown OpenGL cache filename for {0}", GLTypeToString(type));
			return "";
		}
		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage)
			{
			case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
			}
			HZR_CORE_ASSERT(false, "[OpenGLShader]: Unkown OpenGL type to ShaderC type");
			return (shaderc_shader_kind)0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_PROFILE_FUNCTION();
		std::string result = File::ReadFile(filePath);
		auto shaderSources = PreProcess(result);
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
	void OpenGLShader::Set(const std::string name, uint32_t index, uint32_t value)
	{
		std::stringstream ss;
		ss << name << "[" << index << "]";

		GLuint location = glGetUniformLocation(m_ID, ss.str().c_str());
		glUniform1i(location, value);
	}
	void OpenGLShader::Set(const std::string name, uint32_t index, Ref<Texture2D>& value)
	{
		Set(name, index, value->GetID());
	}
	void OpenGLShader::Reflect()
	{
		HZR_PROFILE_FUNCTION();
		HZR_CORE_TRACE("Reflecting shader {0}", m_FilePath);
		m_ShaderData.Stages.clear();

		uint32_t uniformCount = 0;

		for (auto&& [stage, binary] : m_OpenGLSPIRV) {

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			ShaderStageData shaderStage = ProcessShaderStage(compiler, resources);
			m_ShaderData.Stages[GLUtils::ShaderTypeFromGLType(stage)] = shaderStage;
			
			for (auto& resource : resources.uniform_buffers) 
			{
				bool found = false;
				for (auto& uniform : m_ShaderData.UniformsDescriptions) {
					if (uniform.Name == resource.name) {
						uniform.ShaderUsage |= GLUtils::ShaderTypeFromGLType(stage);
						found = true;
						break;
					}
				}
				if (found) continue;

				auto& type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription desc = {};
				desc.Name = resource.name;
				desc.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				desc.MemberCount = type.member_types.size();
				desc.Size = compiler.get_declared_struct_size(type);
				desc.ShaderUsage |= GLUtils::ShaderTypeFromGLType(stage);

				m_ShaderData.UniformsDescriptions.push_back(desc);
			}
		}

		for (auto& uniformBuffer : m_ShaderData.UniformsDescriptions) 
		{
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = uniformBuffer.Name;
			bufferInfo.Binding = uniformBuffer.Binding;
			bufferInfo.Size = uniformBuffer.Size;

			m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
		}

		Utils::PrintReflectResults(m_FilePath, m_ShaderData);
	}
	bool OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		HZR_PROFILE_FUNCTION();
		bool wasCompiled = false;
		m_ID = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		if (false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {
			HZR_PROFILE_FUNCTION("Shader stage");
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + GLUtils::GLShaderStageCachedVulkanExtension(stage));

			GLenum glType = GLUtils::ShaderTypeToGLType(stage);

			if (File::Exists(cachedFilePath)) {
				if (!File::IsNewerThan(shaderFilePath, cachedFilePath)) {
					File::ReadBinaryFileUint32(cachedFilePath, shaderData[glType]);
					continue;
				}
			}

			HZR_CORE_INFO("[VulkanShader]: Reload, source modified");
			shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, GLUtils::GLShaderStageToShaderC(GLUtils::ShaderTypeToGLType(stage)), m_FilePath.c_str(), options);

			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				HZR_CORE_ERROR("Error {0} : {1}, using cached shader binary", Rendering::Utils::ShaderTypeToString(stage), module.GetErrorMessage());
				File::ReadBinaryFileUint32(cachedFilePath, shaderData[glType]);
				continue;
			}
			wasCompiled = true;
			shaderData[glType] = std::vector<uint32_t>(module.begin(), module.cend());
			File::WriteBinaryFile(cachedFilePath, shaderData[glType]);
		}
		return wasCompiled;
	}
	void OpenGLShader::CompileOrGetOpenGLBinaries(bool forceCompile)
	{
		HZR_PROFILE_FUNCTION();
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		if (false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV) {
			HZR_PROFILE_FUNCTION("Shader stage");
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + GLUtils::GLShaderStageCachedOpenGLExtension(stage));

			if (File::Exists(cachedFilePath)) {
				if (!File::IsNewerThan(shaderFilePath, cachedFilePath) && !forceCompile) {
					File::ReadBinaryFileUint32(cachedFilePath, shaderData[stage]);
					continue;
				}
			}

			spirv_cross::CompilerGLSL glslCompiler(spirv);
			m_OpenGLSourceCode[stage] = glslCompiler.compile();

			auto& source = m_OpenGLSourceCode[stage];

			shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, GLUtils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
			if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
				HZR_CORE_ERROR("{0} : {1}", GLUtils::GLTypeToString(stage), module.GetErrorMessage());
			}
			shaderData[stage] = std::vector<uint32_t>(module.begin(), module.cend());
			File::WriteBinaryFile(cachedFilePath, shaderData[stage]);
		}
		Reflect();
	}
	void OpenGLShader::CreateProgram()
	{
		HZR_PROFILE_FUNCTION();
		GLuint program = glCreateProgram();

		std::vector<GLuint> shaderIDs;
		for (auto& [stage, spirv] : m_OpenGLSPIRV) {
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