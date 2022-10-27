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
		~OpenGLShaderCompiler();

		bool Compile(CompileInfo* compileInfo);
		bool Decompile(Buffer binary, std::string& result);

		ShaderData GetShaderResources(const std::unordered_map<ShaderStage, Buffer>& binaries);

		double GetCompileTime() { return m_CompilationTime; }
		std::string GetErrorMessage() { return m_ErrorMessage; }
		Buffer GetCompiledBinary() { return m_ResultBinary; }
	public:

		static void PrintReflectionData(const ShaderData& data);

	private:
		Buffer m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
#endif