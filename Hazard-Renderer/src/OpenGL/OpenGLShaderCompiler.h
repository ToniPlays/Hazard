#pragma once

#include "Core/Core.h"

#ifdef HZR_DESKTOP

#include "Core/ShaderCompiler.h"

#include "File.h"

namespace HazardRenderer::OpenGL
{
	class OpenGLShaderCompiler
	{
	public:
		OpenGLShaderCompiler() = default;
		~OpenGLShaderCompiler();

		bool Compile(CompileInfo* compileInfo);
		bool Decompile(Buffer binary, std::string& result);

		double GetCompileTime() { return m_CompilationTime; }
		std::string GetErrorMessage() { return m_ErrorMessage; }
		Buffer GetCompiledBinary() { return m_ResultBinary; }
	private:
		Buffer m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
#endif
