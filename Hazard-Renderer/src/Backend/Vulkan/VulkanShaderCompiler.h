#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN
#include "File.h"
#include "Backend/Core/Pipeline/ShaderFactory.h"

namespace HazardRenderer::Vulkan
{
	class VulkanShaderCompiler
	{
	public:
		VulkanShaderCompiler() = default;
		~VulkanShaderCompiler() = default;

		bool Compile(CompileInfo* compileInfo);
		ShaderStageData GetResources(std::vector<uint32_t> binary);

		double GetCompileTime() { return m_CompilationTime; }
		std::string GetErrorMessage() { return m_ErrorMessage; }
		std::vector<uint32_t> GetCompiledBinary() { return m_ResultBinary; }

	private:
		std::vector<uint32_t> m_ResultBinary;
		std::string m_ErrorMessage;
		double m_CompilationTime = 0.0;
	};
}
#endif