
#include "VulkanShader.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"

#include "Backend/Core/Pipeline/ShaderFactory.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include "../VulkanShaderCompiler.h"

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
		Renderer::SubmitResourceFree([modules = m_ShaderModules]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
			for (auto& shader : modules)
				vkDestroyShaderModule(device, shader, nullptr);
			});
	}
	void VulkanShader::Reload()
	{
		if (ShaderFactory::HasCachedShader(ShaderStage::Vertex, m_FilePath))
		{
			__debugbreak();
			return;
		}

		std::cout << m_FilePath << std::endl;
		std::unordered_map<ShaderStage, std::string> sources = ShaderFactory::GetShaderSources(m_FilePath);

		//Compile Vulkan shader
		VulkanShaderCompiler compiler;
		std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries;

		for (auto& [stage, source] : sources)
		{
			double compilationTime = 0.0;
			std::vector<ShaderDefine> defines = { { "Vulkan" } };

			//Compile to Vulkan SPV
			CompileInfo compileInfoVulkan = {};
			compileInfoVulkan.Renderer = RenderAPI::Vulkan;
			compileInfoVulkan.Name = m_FilePath;
			compileInfoVulkan.Stage = stage;
			compileInfoVulkan.Source = source;
			compileInfoVulkan.Defines = defines;

			if (!compiler.Compile(&compileInfoVulkan))
			{
				std::cout << compiler.GetErrorMessage() << std::endl;
				continue;
			}
			compilationTime += compiler.GetCompileTime();
			binaries[stage] = compiler.GetCompiledBinary();
		}
		Reflect(binaries);
		Ref<VulkanShader> instance = this;
		Renderer::Submit([instance]() mutable {
			instance->CreateShaderModules();
			instance->CreateDescriptorSetLayouts();
			});
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
	void VulkanShader::Reload_RT(bool forceCompile)
	{

	}
	void VulkanShader::Reflect(const std::unordered_map<ShaderStage, std::vector<uint32_t>> binaries)
	{
		Timer timer;
		m_ShaderData.Stages.clear();

		VulkanShaderCompiler compiler;
		uint32_t uniformCount = 0;

		for (auto&& [stage, binary] : binaries) {


			ShaderStageData shaderStage = compiler.GetResources(binary);

			spirv_cross::Compiler compiler(binary);
			spirv_cross::ShaderResources resources = compiler.get_shader_resources();

			m_ShaderData.Stages[stage] = shaderStage;

			for (auto resource : resources.uniform_buffers)
			{
				auto& type = compiler.get_type(resource.base_type_id);

				ShaderUniformBufferDescription spec = {};
				spec.Name = resource.name;
				spec.Binding = compiler.get_decoration(resource.id, spv::Decoration::DecorationBinding);
				spec.MemberCount = type.member_types.size();
				spec.Size = compiler.get_declared_struct_size(type);
				spec.ShaderUsage |= (uint32_t)stage;

				auto it = m_ShaderData.UniformsDescriptions.find(spec.Binding);
				if (it != m_ShaderData.UniformsDescriptions.end())
				{
					auto& buffer = m_ShaderData.UniformsDescriptions[spec.Binding];
					buffer.ShaderUsage |= (uint32_t)stage;
					continue;
				}

				m_ShaderData.UniformsDescriptions[spec.Binding] = spec;
			}
		}

		for (auto& [binding, spec] : m_ShaderData.UniformsDescriptions)
		{
			UniformBufferCreateInfo bufferInfo = {};
			bufferInfo.Name = spec.Name;
			bufferInfo.Binding = spec.Binding;
			bufferInfo.Size = spec.Size;
			bufferInfo.Usage = spec.ShaderUsage;

			m_UniformBuffers[bufferInfo.Name] = UniformBuffer::Create(&bufferInfo);
		}
		m_ShaderCode = binaries;
		std::cout << "Reflection took: " << timer.ElapsedMillis() << "ms" << std::endl;
	}
	void VulkanShader::CreateShaderModules()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		uint32_t index = 0;

		m_ShaderModules.resize(m_ShaderCode.size());

		for (auto& [stage, code] : m_ShaderCode) {

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.codeSize = code.size() * sizeof(uint32_t);
			createInfo.pCode = code.data();

			VK_CHECK_RESULT(vkCreateShaderModule(device, &createInfo, nullptr, &m_ShaderModules[index]), "Failed to create VkShaderModule");

			auto& stageInfo = m_ShaderStageCreateInfos.emplace_back();
			stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			stageInfo.module = m_ShaderModules[index];
			stageInfo.pName = "main";
			stageInfo.flags = 0;
			stageInfo.stage = (VkShaderStageFlagBits)VkUtils::GetVulkanShaderStage(stage);

			index++;
		}
	}
	void VulkanShader::CreateDescriptorSetLayouts()
	{
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		//VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &m_DescriptorSetLayouts[0]), "Failed to create DescriptorSetLayout");
	}
}
#endif
