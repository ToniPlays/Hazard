#pragma once

#include <hzrpch.h>
#include "VulkanPipeline.h"
#include "VulkanContext.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanPipeline::VulkanPipeline(const PipelineSpecification& specs)
	{

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