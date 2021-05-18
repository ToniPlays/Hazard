#pragma once

#include <hzrpch.h>
#include "ShaderCompiler.h"
#include "Hazard/File/File.h"
#include "Hazard/File/StringUtil.h"


namespace Hazard::Rendering 
{
	shaderc::CompileOptions options;

	std::vector<uint32_t> ShaderCompiler::CompileShader(const std::string& sourceFile, const std::string& dest, ShaderType type)
	{
		std::string source = File::ReadFile(sourceFile);
		StringUtil::RemoveAtEnd(source, '\0');
		shaderc_shader_kind kind;

		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);

		switch (type)
		{
		case ShaderType::VertexShader:		kind = shaderc_vertex_shader;	break;
		case ShaderType::FragmentShader:	kind = shaderc_fragment_shader; break;
		case ShaderType::GeometryShader:	kind = shaderc_geometry_shader; break;
		case ShaderType::ComputeShader:		kind = shaderc_compute_shader;	break;
		}

		source = Preprocess(source, kind);
		source = GLSLToAssembly(source, sourceFile.c_str(), kind);
		HZR_CORE_INFO(source);

		std::vector<uint32_t> binary;
		binary = CompileToSPIRV(source, sourceFile.c_str(), kind);
		return binary;
	}
	std::string ShaderCompiler::Preprocess(const std::string& source, shaderc_shader_kind kind)
	{
		shaderc::Compiler compiler;
		

		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source, kind, "shader_src", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_CORE_ERROR("Failed to preprocess shader file: " + result.GetErrorMessage());
			return source;
		}
		HZR_CORE_INFO("Shader file preprocessed");
		return { result.begin(), result.end() };
	}
	std::string ShaderCompiler::GLSLToAssembly(const std::string& source, const char* sourceFile, shaderc_shader_kind kind)
	{
		shaderc::Compiler compiler;

		shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, kind, sourceFile, options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_CORE_ERROR("Failed to create shader assembly file: " + result.GetErrorMessage());
			return source;
		}
		return { result.begin(), result.end() };
	}
	std::vector<uint32_t> ShaderCompiler::CompileToSPIRV(const std::string& source, const char* sourceFile, shaderc_shader_kind kind)
	{

		shaderc::Compiler compiler;

		std::vector<uint32_t> binary;
		shaderc::SpvCompilationResult module =
			compiler.CompileGlslToSpv(source, kind, sourceFile, options);

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			std::cerr << module.GetErrorMessage();
			return std::vector<uint32_t>();
		}
		return { binary.begin(), binary.end() };
	}
}