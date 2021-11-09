#pragma once

#include <hzrpch.h>
#include "VulkanPipeline.h"
#include "../VulkanContext.h"
#include "Hazard/Rendering/2D/Renderer2D.h"
#include "Hazard/Rendering/RenderCommand.h"

namespace Hazard::Rendering::Vulkan
{

	VulkanPipeline::VulkanPipeline(const PipelineSpecification& specs) : m_Specs(specs)
	{
		HZR_PROFILE_FUNCTION();
		auto device = VulkanContext::GetDevice()->GetDevice();
		m_Shader = Shader::Create(specs.ShaderPath).As<VulkanShader>();

		m_VertexBuffer = VertexBuffer::Create(specs.pVertexBuffer).As<VulkanVertexBuffer>();
		m_IndexBuffer = IndexBuffer::Create(specs.pIndexBuffer).As<VulkanIndexBuffer>();

		Invalidate();
	}
	VulkanPipeline::~VulkanPipeline()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();

		vkDestroyDescriptorSetLayout(device, m_UniformDescriptorLayout, nullptr);
		vkDestroyPipeline(device, m_Pipeline, nullptr);
		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
	}
	void VulkanPipeline::Invalidate()
	{
		auto device = VulkanContext::GetDevice()->GetDevice();
		std::vector<VkPipelineShaderStageCreateInfo> shaderStages = m_Shader->GetStageInfo();

		auto& shaderData = m_Shader->GetShaderData();

		VkVertexInputBindingDescription inputBindings = m_Shader->GetBindingDescriptions();
		std::vector<VkVertexInputAttributeDescription> inputAttribs = m_Shader->GetAttriDescriptions();

		if (m_Shader->CreateDescriptorLayout(&m_UniformDescriptorLayout) != VK_SUCCESS) {
			__debugbreak();
		}
		m_Shader->CreateDescriptorSet(&m_UniformDescriptorLayout);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 1;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pSetLayouts = &m_UniformDescriptorLayout;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan Pipeline Layout!");
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 1;
		vertexInputInfo.pVertexBindingDescriptions = &inputBindings;
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(inputAttribs.size());
		vertexInputInfo.pVertexAttributeDescriptions = inputAttribs.data();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkViewport viewport = {};
		viewport.width = (float)1280;
		viewport.height = -(float)720;
		viewport.x = 0;
		viewport.y = 720.0f;
		viewport.minDepth = (float)0.0f;
		viewport.maxDepth = (float)1.0f;

		VkRect2D scissor = {};
		scissor.offset = { 0, 0 };
		scissor.extent = { 1280, 720 };

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.depthBiasSlopeFactor = -1.0f;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;

		VkPipelineMultisampleStateCreateInfo multisampling = {};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

		VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;

		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;

		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f;
		colorBlending.blendConstants[1] = 0.0f;
		colorBlending.blendConstants[2] = 0.0f;
		colorBlending.blendConstants[3] = 0.0f;

		VkPipelineDepthStencilStateCreateInfo depthStencil = {};

		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencil.depthTestEnable = VK_TRUE;
		depthStencil.depthWriteEnable = VK_TRUE;
		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
		depthStencil.depthBoundsTestEnable = VK_FALSE;
		depthStencil.minDepthBounds = 0.0f;  // Optional
		depthStencil.maxDepthBounds = 1.0f;  // Optional
		depthStencil.stencilTestEnable = VK_FALSE;

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDepthStencilState = &depthStencil;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = VulkanContext::GetSwapchain()->GetRenderPass();
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan Graphics Pipeline!");
		}
	}
	void VulkanPipeline::Bind()
	{
		RenderCommand::Submit([=]() {
			auto cmdBuffer = VulkanContext::GetSwapchain()->GetCurrentDrawCommandBuffer();

			VkBuffer vertexBuffer = m_VertexBuffer->GetVulkanBuffer();
			VkDeviceSize offsets[1] = { 0 };

			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
			vkCmdBindVertexBuffers(cmdBuffer, 0, 1, &vertexBuffer, offsets);
			vkCmdBindIndexBuffer(cmdBuffer, m_IndexBuffer->GetVulkanBuffer(), 0, VK_INDEX_TYPE_UINT32);
			vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, m_Shader->GetDescriptorSet(), 0, nullptr);
		});
	}
	void VulkanPipeline::Draw(uint32_t size) 
	{
		auto buffer = VulkanContext::GetSwapchain()->GetCurrentDrawCommandBuffer();
		vkCmdDrawIndexed(buffer, 24, 1, 0, 0, 0);
	}
}