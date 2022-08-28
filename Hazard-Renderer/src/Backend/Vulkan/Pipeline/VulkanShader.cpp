
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"

#include <sstream>
#include <glad/glad.h>
#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

namespace HazardRenderer::Vulkan
{
	VulkanShader::VulkanShader(const std::string& filePath) : m_FilePath(filePath)
	{
		HZR_ASSERT(!filePath.empty(), "Shader path cannot be empty");
		Reload();
	}
	VulkanShader::~VulkanShader()
	{

	}
	void VulkanShader::Reload()
	{

	}
	bool VulkanShader::SetUniformBuffer(const std::string& name, void* data, uint32_t size)
	{
		auto& uniformBuffer = m_UniformBuffers[name];
		if (!uniformBuffer) return false;
		//HZR_ASSERT(uniformBuffer, "[OpenGLShader]: UniformBuffer '{0}' does not exist", name);
		uniformBuffer->SetData(data, size);
		return true;
	}
	void VulkanShader::Set(const std::string& name, uint32_t index, uint32_t value)
	{

	}
	void VulkanShader::Set(const std::string& name, uint32_t index, Ref<Image2D> value)
	{

	}
	void VulkanShader::Reflect()
	{
		m_ShaderData.Stages.clear();

		uint32_t uniformCount = 0;
	}
	bool VulkanShader::CompileOrGetVulkanBinaries(const std::unordered_map<ShaderType, std::string>& sources)
	{
		bool wasCompiled = false;

		auto& shaderData = m_VulkanSPIRV;
		shaderData.clear();

		for (auto&& [stage, source] : sources) {

			auto binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::Vulkan);

			if (binaries.size() > 0) {
				shaderData[stage] = binaries;
				continue;
			}

			std::vector<ShaderDefine> defines = { { "RENDERER_VULKAN" } };

			CompileInfo compileInfo = {};
			compileInfo.Path = m_FilePath;
			compileInfo.Environment = RenderAPI::Vulkan;
			compileInfo.Optimization = Optimization::None;
			compileInfo.Source = source;
			compileInfo.Stage = stage;
			compileInfo.Defines = defines;

			ShaderFactory::Compile(&compileInfo);

			if (!compileInfo.Succeeded())
			{
				std::cout << compileInfo.Error << std::endl;
				Window::SendDebugMessage({ Severity::Error, compileInfo.Error, source });
				auto binaries = ShaderFactory::GetShaderBinaries(m_FilePath, stage, RenderAPI::OpenGL);
				shaderData[stage] = binaries;
				continue;
			}
			wasCompiled = true;
			shaderData[stage] = compileInfo.Binary;
		}
		return wasCompiled;
	}
}
#endif
