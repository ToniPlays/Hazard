#pragma once

#include "Core/Core.h"
#ifdef HZR_SHADER_COMPILER

#include "File.h"
#include "Core/CompileInfo.h"
#include "Buffer/Buffer.h"

namespace HazardRenderer::Vulkan
{
	class VulkanShaderCompiler
	{
	public:
		VulkanShaderCompiler() = default;
		~VulkanShaderCompiler();

		bool Compile(CompileInfo* compileInfo);

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
