#pragma once

#include <hzrpch.h>
#include "OpenGLShader.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "Hazard/Rendering/RenderEngine.h"

namespace Hazard::Rendering::OpenGL 
{

	namespace Utils 
	{
		static GLenum ShaderTypeFromString(const std::string& type) {
			if (type == "Vertex")		return GL_VERTEX_SHADER;
			if (type == "Fragment")		return GL_FRAGMENT_SHADER;
			if (type == "Pixel")		return GL_FRAGMENT_SHADER;
			return 0;
		}
		static std::string ShaderTypeToString(GLenum type) {
			if (type == GL_VERTEX_SHADER)		return "Vertex";
			if (type == GL_FRAGMENT_SHADER)		return "Fragment";
			return 0;
		}
		static std::string GLSahderStageCachedVulkanExtension(GLenum type) {
			switch (type)
			{
			case GL_VERTEX_SHADER:		return ".cached_vulkan.vert";
			case GL_FRAGMENT_SHADER:	return ".cached_vulkan.frag";
			}
			HZR_CORE_ASSERT(false);
			return "";
		}
		static std::string GLSahderStageCachedOpenGLExtension(GLenum type) {
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
			HZR_CORE_TRACE("Shader creation took {0} ms", timer.ElapsedMillis());
		}

	}
	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexPath, const std::string& fragmenPath)
	{

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
	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLen = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);

		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			HZR_CORE_ASSERT(eol != std::string::npos, "Syntax Error, check Shader");
			size_t begin = pos + typeTokenLen + 1;
			std::string type = source.substr(begin, eol - begin);

			HZR_CORE_ASSERT(Utils::ShaderTypeFromString(type), "Invalid shader type {0}", type);
			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			HZR_CORE_ASSERT(nextLinePos != std::string::npos, "Syntax Error, check Shader");

			pos = source.find(typeToken, nextLinePos);

			shaderSources[Utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
		}
		return shaderSources;

	}
	void OpenGLShader::Reflect(GLenum stage, std::vector<uint32_t> data)
	{

	}
	void OpenGLShader::CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& sources)
	{
		m_ID = glCreateProgram();

		shaderc::Compiler compiler;
		shaderc::CompileOptions options;
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);
		
		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();
		
		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + Utils::GLSahderStageCachedVulkanExtension(stage));
			
			std::ifstream in(cachedFilePath, std::ios::in | std::ios::binary);

			if (in.is_open()) {
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else 
			{
				shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					HZR_CORE_ERROR("{0} : {1}", Utils::ShaderTypeToString(stage), module.GetErrorMessage());
					HZR_CORE_ASSERT(false);
				}
				shaderData[stage] = std::vector<uint32_t>(module.begin(), module.cend());

				std::ofstream out(cachedFilePath, std::ios::out | std::ios::binary);

				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
			}
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
		options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
		options.SetOptimizationLevel(shaderc_optimization_level_performance);

		std::filesystem::path cache = RenderEngine::GetShaderCompilePath();

		shaderData.clear();
		m_OpenGLSourceCode.clear();

		for (auto&& [stage, spirv] : m_VulkanSPIRV) {
			std::filesystem::path shaderFilePath = m_FilePath;
			std::filesystem::path cachedFilePath = cache / (shaderFilePath.filename().string() + Utils::GLSahderStageCachedVulkanExtension(stage));

			std::ifstream in(cachedFilePath, std::ios::in | std::ios::binary);

			if (in.is_open()) 
			{
				in.seekg(0, std::ios::end);
				auto size = in.tellg();
				in.seekg(0, std::ios::beg);

				auto& data = shaderData[stage];
				data.resize(size / sizeof(uint32_t));
				in.read((char*)data.data(), size);
			}
			else 
			{
				//spirv_cross::CompilerGLSL glslCompiler(spirv);
				/*m_OpenGLSourceCode[stage] = glslCompiler.compile();

				auto& source = m_OpenGLSourceCode[stage];

				shaderc::CompilationResult module = compiler.CompileGlslToSpv(source, Utils::GLShaderStageToShaderC(stage), m_FilePath.c_str(), options);
				if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
					HZR_CORE_ERROR("{0} : {1}", Utils::ShaderTypeToString(stage), module.GetErrorMessage());
					HZR_CORE_ASSERT(false);
				}
				shaderData[stage] = std::vector<uint32_t>(module.begin(), module.cend());

				std::ofstream out(cachedFilePath, std::ios::out | std::ios::binary);

				if (out.is_open()) {
					auto& data = shaderData[stage];
					out.write((char*)data.data(), data.size() * sizeof(uint32_t));
					out.flush();
					out.close();
				}
				*/
			}
		}
	}
}