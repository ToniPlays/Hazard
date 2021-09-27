#pragma once

#include <hzrpch.h>
#include "OpenGLShader.h"
#include "Hazard/Rendering/RenderEngine.h"

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
			HZR_CORE_ASSERT(false);
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
			HZR_CORE_ASSERT(false);
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
			HZR_CORE_ASSERT(false);
			return "";
		}

		static std::string GLShaderStageCachedVulkanExtension(ShaderType type) {
			switch (type)
			{
			case Vertex:	return ".cached_vulkan.vert";
			case Fragment:	return ".cached_vulkan.frag";
			}
			HZR_CORE_ASSERT(false);
			return "";
		}
		static std::string GLShaderStageCachedOpenGLExtension(GLenum type) {
			switch (type)
			{
			case GL_VERTEX_SHADER:		return ".cached_opengl.vert";
			case GL_FRAGMENT_SHADER:	return ".cached_opengl.frag";
			}
			HZR_CORE_ASSERT(false);
			return "";
		}
		static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
			switch (stage)
			{
			case GL_VERTEX_SHADER:		return shaderc_glsl_vertex_shader;
			case GL_FRAGMENT_SHADER:	return shaderc_glsl_fragment_shader;
			}
			HZR_ASSERT(false);
			return (shaderc_shader_kind)0;
		}
	}

	OpenGLShader::OpenGLShader(const std::string& filePath) : m_FilePath(filePath)
	{
		std::string result = File::ReadFile(filePath);
		auto shaderSources = PreProcess(result);
		{
			Timer timer;
			CompileOrGetVulkanBinaries(shaderSources);
			CompileOrGetOpenGLBinaries();
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
			CompileOrGetVulkanBinaries(sources);
			CompileOrGetOpenGLBinaries();
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
	void OpenGLShader::Reflect(GLenum stage, std::vector<uint32_t> data)
	{
		m_ShaderStageData.clear();

		for (auto&& [stage, binary] : m_OpenGLSPIRV) {
			ShaderStageData shaderStage;

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			for (auto& resource : resources.stage_inputs) {

				auto spvType = compiler.get_type(resource.base_type_id);
				ShaderStageInput input;
				input.Name = resource.name;
				input.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
				input.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
				input.Size = ShaderDataTypeSize(input.Type);

				shaderStage.Inputs.push_back(input);
			}
			for (auto& resource : resources.stage_outputs) {

				auto spvType = compiler.get_type(resource.base_type_id);
				ShaderStageOutput output;
				output.Name = resource.name;
				output.Location = compiler.get_decoration(resource.id, spv::Decoration::DecorationLocation);
				output.Type = Rendering::Utils::ShaderTypeFromSPV(spvType);
				output.Size = ShaderDataTypeSize(output.Type);

				shaderStage.Outputs.push_back(output);
			}

			m_ShaderStageData[GLUtils::ShaderTypeFromGLType(stage)] = shaderStage;
		}

	}
	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		m_ID = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);

		if(false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {
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
			shaderData[glType] = std::vector<uint32_t>(module.begin(), module.cend());
			File::WriteBinaryFile(cachedFilePath, shaderData[glType]);

		}
		for (auto&& [stage, data] : shaderData) {
			Reflect(stage, data);
		}
	}
	void OpenGLShader::CompileOrGetOpenGLBinaries()
	{
		auto& shaderData = m_OpenGLSPIRV;

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		if(false)
			options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV) {
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + GLUtils::GLShaderStageCachedOpenGLExtension(stage));

			if (!File::ReadBinaryFileUint32(cachedFilePath, shaderData[stage]))
			{
				spirv_cross::CompilerGLSL glslCompiler(spirv);
				m_OpenGLSourceCode[stage] = glslCompiler.compile();

				auto& source = m_OpenGLSourceCode[stage];

				shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, GLUtils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					HZR_CORE_ERROR("{0} : {1}", GLUtils::GLTypeToString(stage), module.GetErrorMessage());
					HZR_CORE_ASSERT(false);
				}
				shaderData[stage] = std::vector<uint32_t>(module.begin(), module.cend());
				File::WriteBinaryFile(cachedFilePath, shaderData[stage]);
			}
		}
	}
	void OpenGLShader::CreateProgram()
	{
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