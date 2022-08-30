#pragma once

#include "Backend/Core/Core.h"

#ifdef HZR_INCLUDE_OPENGL
#include "File.h"
#include "Backend/Core/Pipeline/ShaderFactory.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLShaderCompiler
	{
	public:
		OpenGLShaderCompiler() = default;
		~OpenGLShaderCompiler() = default;

		bool Compile(CompileInfo* compileInfo);
		bool Decompile(const std::vector<uint32_t> binary, std::string& result);

		ShaderData GetShaderResources(const std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries);

		double GetCompileTime() { return m_CompilationTime; }
		std::string GetErrorMessage() { return m_ErrorMessage; }
		std::vector<uint32_t> GetCompiledBinary() { return m_ResultBinary; }
	public:

		static void PrintReflectionData(const ShaderData& data);

	private:
		std::vector<uint32_t> m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
#endif