#pragma once

#include "Core/Core.h"

#ifdef HZR_SHADER_COMPILER

#include "Core/CompileInfo.h"
#include "File.h"
#include "Buffer/Buffer.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLShaderCompiler
	{
	public:
		OpenGLShaderCompiler() = default;
		~OpenGLShaderCompiler();

		bool Compile(CompileInfo* compileInfo);
		bool Decompile(Buffer binary, std::string& result);

		double GetCompileTime() const { return m_CompilationTime; }
		std::string GetErrorMessage() const { return m_ErrorMessage; }
		Buffer GetCompiledBinary() const { return m_ResultBinary; }
	private:
		Buffer m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
#endif
