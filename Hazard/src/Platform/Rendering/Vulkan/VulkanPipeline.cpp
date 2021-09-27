#pragma once

#include <hzrpch.h>
#include "VulkanPipeline.h"
#include "VulkanContext.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanPipeline::VulkanPipeline(const PipelineSpecification& specs) : m_Specs(specs)
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		m_Shader = (VulkanShader*)Shader::Create(specs.ShaderPath);
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

		m_Shader->GetStageInfo(shaderStages);
		
		std::unordered_map<ShaderType, ShaderStageData> data = m_Shader->GetShaderData();

		VkPipelineLayoutCreateInfo pipelineLayout = {};
		pipelineLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayout.setLayoutCount = 1;
		pipelineLayout.pSetLayouts = 0;

		VkDescriptorBufferInfo info = {};

		VkGraphicsPipelineCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		createInfo.stageCount = shaderStages.size();
		createInfo.pStages = shaderStages.data();




		//vkCreateGraphicsPipelines(device, VulkanContext::GetPipelineCache(), 1, &createInfo, nullptr, &m_Pipeline);

		m_Shader->DestroyModules();
	}
	VulkanPipeline::~VulkanPipeline()
	{

	}
	void VulkanPipeline::Invalidate()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();

		VkGraphicsPipelineCreateInfo pipelineCreateInfo;
		pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		vkCreateGraphicsPipelines(device, VulkanContext::GetPipelineCache(), 1, &pipelineCreateInfo, nullptr, &m_Pipeline);
	}
	void VulkanPipeline::Bind()
	{

	}
}