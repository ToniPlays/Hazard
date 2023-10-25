#pragma once

#include "Core/Core.h"
#include "File.h"

#include "Core/ShaderCompiler.h"

namespace HazardRenderer::Vulkan
{

	class VulkanShaderCompiler
	{
	public:
		VulkanShaderCompiler() = default;
		~VulkanShaderCompiler();

		bool Compile(CompileInfo* compileInfo);

		double GetCompileTime() { return m_CompilationTime; }
		std::string GetErrorMessage() { return m_ErrorMessage; }
		Buffer GetCompiledBinary() { return m_ResultBinary; }

	private:
		Buffer m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
