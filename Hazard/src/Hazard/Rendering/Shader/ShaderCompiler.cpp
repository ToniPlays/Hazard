#pragma once

#include <hzrpch.h>
#include "ShaderCompiler.h"
#include "Hazard/File/File.h"


namespace Hazard::Rendering 
{

	shaderc::Compiler compiler;
	shaderc::CompileOptions options;

	std::vector<uint32_t> ShaderCompiler::CompileShader(const std::string& sourceFile, const std::string& dest, ShaderType type)
	{
		std::string source = File::ReadFile(sourceFile);

		shaderc_shader_kind kind;

		switch (type)
		{
		case ShaderType::VertexShader:		kind = shaderc_vertex_shader;	break;
		case ShaderType::FragmentShader:	kind = shaderc_fragment_shader; break;
		case ShaderType::GeometryShader:	kind = shaderc_geometry_shader; break;
		case ShaderType::ComputeShader:		kind = shaderc_compute_shader;	break;
		}


		source = Preprocess(source, kind);
		source = GLSLToAssembly(source, sourceFile.c_str(), kind);

		std::vector<uint32_t> binary = CompileToSPIRV();
		return binary;
	}
	std::string ShaderCompiler::Preprocess(const std::string& source, shaderc_shader_kind kind)
	{
		auto result = compiler.PreprocessGlsl(source, kind, "sourceFile.glsl", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_CORE_ERROR("Failed to preprocess shader file: " + result.GetErrorMessage());
			return source;
		}
		HZR_CORE_INFO("Shader file preprocessed");
		return { result.begin(), result.end() };
	}
	std::string ShaderCompiler::GLSLToAssembly(const std::string& source, const char* sourceFile, shaderc_shader_kind kind)
	{
		shaderc::AssemblyCompilationResult result = compiler.CompileGlslToSpvAssembly(source, kind, sourceFile, options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_CORE_ERROR("Failed to create shader assembly file: " + result.GetErrorMessage());
			return source;
		}

		return { result.begin(), result.end() };
	}
	std::vector<uint32_t> ShaderCompiler::CompileToSPIRV()
	{
		return std::vector<uint32_t>();
	}
}