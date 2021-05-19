#pragma once

#include <hzrpch.h>
#include "ShaderCompiler.h"
#include "Hazard/File/File.h"
#include "Hazard/File/StringUtil.h"

#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace Hazard::Rendering 
{
	shaderc::CompileOptions options;

	std::vector<uint32_t> ShaderCompiler::CompileShader(const std::string& sourceFile, const std::string& dest, ShaderType type)
	{
		std::string source = File::ReadFile(sourceFile);
		source = StringUtil::RemoveAtEnd(source, '\0');

		shaderc_shader_kind kind;
		//options.SetOptimizationLevel(shaderc_optimization_level_performance);
		options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
		//options.SetSourceLanguage(shaderc_source_language_glsl);

		switch (type)
		{
		case ShaderType::VertexShader:		kind = shaderc_vertex_shader;	break;
		case ShaderType::FragmentShader:	kind = shaderc_fragment_shader; break;
		case ShaderType::GeometryShader:	kind = shaderc_geometry_shader; break;
		case ShaderType::ComputeShader:		kind = shaderc_compute_shader;	break;
		}

		source = Preprocess(source, kind);

		std::vector<uint32_t> binary;
		binary = CompileToSPIRV(source, sourceFile.c_str(), kind);

		File::WriteBinaryFile(dest, binary);

		HZR_CORE_INFO("Shader compiled");

		return binary;
	}
	std::string ShaderCompiler::Preprocess(const std::string& source, shaderc_shader_kind kind)
	{
		shaderc::Compiler compiler;
		shaderc::PreprocessedSourceCompilationResult result = compiler.PreprocessGlsl(source, kind, "shader_src", options);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_THROW("Failed to preprocess shader file: " + result.GetErrorMessage());
			return source;
		}
		HZR_CORE_INFO("Shader file preprocessed");
		return { result.begin(), result.end() };
	}
	std::vector<uint32_t> ShaderCompiler::CompileToSPIRV(const std::string& source, const char* sourceFile, shaderc_shader_kind kind)
	{
		shaderc::Compiler compiler;

		shaderc::SpvCompilationResult module =
			compiler.CompileGlslToSpv(source, kind, "shader_src");

		if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
			HZR_THROW(module.GetErrorMessage());
		}
		return { module.begin(), module.end() };
	}
}