#include "OpenGLShaderCompiler.h"
#ifdef HZR_INCLUDE_OPENGL

#include "MathCore.h"
#include "Profiling/Timer.h"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::OpenGL
{
	static shaderc_shader_kind ShaderStageToShaderC(const ShaderStage& type) {
		switch (type)
		{
		case ShaderStage::Vertex:	return shaderc_glsl_vertex_shader;
		case ShaderStage::Fragment:	return shaderc_glsl_fragment_shader;
		case ShaderStage::Compute:	return shaderc_glsl_compute_shader;
		case ShaderStage::Geometry:	return shaderc_glsl_geometry_shader;
		case ShaderStage::None:      return (shaderc_shader_kind)0;
		}
		return (shaderc_shader_kind)0;
	}

	OpenGLShaderCompiler::~OpenGLShaderCompiler()
	{
		m_ResultBinary.Release();
	}

	bool OpenGLShaderCompiler::Compile(CompileInfo* compileInfo)
	{
		HZR_PROFILE_FUNCTION();
		Timer timer;

		m_ResultBinary.Release();
		m_ErrorMessage.clear();

		shaderc::CompileOptions options;

		switch (compileInfo->Renderer)
		{
		case RenderAPI::Vulkan: options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2); break;
		case RenderAPI::OpenGL: options.SetTargetEnvironment(shaderc_target_env_opengl, 450); break;
		}

		options.SetOptimizationLevel((shaderc_optimization_level)compileInfo->Optimization);
		options.SetGenerateDebugInfo();

		for (uint32_t i = 0; i < compileInfo->DefineCount; i++) 
		{
			ShaderDefine define = compileInfo->pDefines[i];
			if (!define.Value.empty())
				options.AddMacroDefinition(define.Name, define.Value);
			else options.AddMacroDefinition(define.Name);
		}
        
		shaderc::Compiler compiler;
		shaderc::CompilationResult result = compiler.CompileGlslToSpv(compileInfo->Source,
			ShaderStageToShaderC(compileInfo->Stage), compileInfo->Name.c_str(), options);

		bool succeeded = result.GetCompilationStatus() == shaderc_compilation_status_success;
		if (succeeded)
			m_ResultBinary = Buffer::Copy(result.begin(), (result.end() - result.begin()) * sizeof(uint32_t));
		else
			m_ErrorMessage = result.GetErrorMessage();

		m_CompilationTime = timer.ElapsedMillis();
		return succeeded;
	}
	bool OpenGLShaderCompiler::Decompile(Buffer binary, std::string& result)
	{
		HZR_PROFILE_FUNCTION();
		m_ErrorMessage.clear();
		spirv_cross::CompilerGLSL compiler((uint32_t*)binary.Data, binary.Size / sizeof(uint32_t));
		result = compiler.compile();
		return !result.empty();
	}
}
#endif
