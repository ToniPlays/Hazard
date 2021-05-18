#pragma once

#include "Shader.h"
#include <shaderc/shaderc.hpp>

namespace Hazard::Rendering 
{
	class ShaderCompiler {
	public:
		static std::vector<uint32_t> CompileShader(const std::string& sourceFile, const std::string& dest, ShaderType type);

	private:
		static std::string Preprocess(const std::string& source, shaderc_shader_kind kind);
		static std::string GLSLToAssembly(const std::string& source, const char* sourceFile, shaderc_shader_kind kind);
		static std::vector<uint32_t> CompileToSPIRV();
	};
}