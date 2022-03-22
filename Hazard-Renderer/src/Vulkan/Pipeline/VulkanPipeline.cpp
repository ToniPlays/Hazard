//#pragma once
//
//#include <hzrpch.h>
//#include "VulkanPipeline.h"
//#include "../VulkanContext.h"
//#include "../VulkanFrameBuffer.h"
//#include "../VKUtils.h"
//#include "Hazard/Rendering/2D/Renderer2D.h"
//#include "Hazard/Rendering/RenderCommand.h"
//
//namespace Hazard::Rendering::Vulkan
//{
//
//	VulkanPipeline::VulkanPipeline(PipelineSpecification* specs)
//	{
//		auto device = VulkanContext::GetDevice()->GetDevice();
//
//		if (m_Specs.ShaderPath != specs->ShaderPath)
//			m_Shader = Shader::Create(specs->ShaderPath).As<VulkanShader>();
//			
//		m_Specs = *specs;
//
//		Ref<VulkanPipeline> instance = this;
//		RenderContextCommand::SubmitResourceCreate([instance]() mutable {
//			HZR_CORE_WARN("Created pipeline");
//			instance->Invalidate();
//			});
//
//	}
//	VulkanPipeline::~VulkanPipeline()
//	{
//		RenderContextCommand::SubmitResourceFree([uboLayout = m_UniformDescriptorLayout, pipeline = m_Pipeline, layout = m_PipelineLayout]() {
//			auto device = VulkanContext::GetDevice()->GetDevice();
//			vkDestroyDescriptorSetLayout(device, uboLayout, nullptr);
//			vkDestroyPipeline(device, pipeline, nullptr);
//			vkDestroyPipelineLayout(device, layout, nullptr);
//			HZR_CORE_ERROR("Destroyed Vulkan pipeline");
//			});
//		
//	}
//	void VulkanPipeline::SetRenderPass(Ref<RenderPass> renderPass)
//	{
//		m_Specs.TargetRenderPass = renderPass;
//		Ref<VulkanPipeline> instance = this;
//
//		RenderContextCommand::Submit([instance]() mutable {
//			HZR_PROFILE_FUNCTION("VulkanPipeline::SetRenderPass() RT");
//			instance->Invalidate();
//			});
//		RenderContextCommand::SubmitResourceFree([pipeline = m_Pipeline, layout = m_PipelineLayout]() {
//			HZR_PROFILE_FUNCTION("VulkanPipeline::SetRenderPass() Release RT");
//
//			if (!pipeline) return;
//
//			auto device = VulkanContext::GetDevice()->GetDevice();
//			//vkDestroyPipeline(device, pipeline, nullptr);
//			//vkDestroyPipelineLayout(device, layout, nullptr);
//			});
//	}
//	void VulkanPipeline::Invalidate()
//	{
//		auto device = VulkanContext::GetDevice()->GetDevice();
//		std::vector<VkPipelineShaderStageCreateInfo> shaderStages = m_Shader->GetStageInfo();
//
//		auto& shaderData = m_Shader->GetShaderData();
//
//		VkVertexInputBindingDescription inputBindings = m_Shader->GetBindingDescriptions();
//		std::vector<VkVertexInputAttributeDescription> inputAttribs = m_Shader->GetAttriDescriptions();
//
//		m_UniformDescriptorLayout = m_Shader->GetDescriptorLayout();
//
//		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
//		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//		pipelineLayoutInfo.setLayoutCount = 1;
//		pipelineLayoutInfo.pushConstantRangeCount = 0;
//		pipelineLayoutInfo.pSetLayouts = &m_UniformDescriptorLayout;
//
//		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout) != VK_SUCCESS) {
//			HZR_THROW("Failed to create Vulkan Pipeline Layout!");
//		}
//
//		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
//		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
//
//		//If vertex shader has no inputs
//		if (shaderData.Stages.at(ShaderType::Vertex).Inputs.size() != 0) {
//			vertexInputInfo.vertexBindingDescriptionCount = 1;
//			vertexInputInfo.pVertexBindingDescriptions = &inputBindings;
//			vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(inputAttribs.size());
//			vertexInputInfo.pVertexAttributeDescriptions = inputAttribs.data();
//		}
//		else {
//			vertexInputInfo.vertexBindingDescriptionCount = 0;
//			vertexInputInfo.pVertexBindingDescriptions = nullptr;
//			vertexInputInfo.vertexAttributeDescriptionCount = 0;
//			vertexInputInfo.pVertexAttributeDescriptions = nullptr;
//		}
//
//		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
//		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
//		inputAssembly.topology = VKUtils::DrawTypeToVKTopology(m_Specs.DrawType);
//		inputAssembly.primitiveRestartEnable = VK_FALSE;
//
//		VkViewport viewport = {};
//		viewport.width = (float)1280;
//		viewport.height = -(float)720;
//		viewport.x = 0;
//		viewport.y = 720.0f;
//		viewport.minDepth = (float)0.0f;
//		viewport.maxDepth = (float)1.0f;
//
//		VkRect2D scissor = {};
//		scissor.offset = { 0, 0 };
//		scissor.extent = { 1280, 720 };
//
//		VkPipelineViewportStateCreateInfo viewportState = {};
//		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
//		viewportState.viewportCount = 1;
//		viewportState.pViewports = &viewport;
//		viewportState.scissorCount = 1;
//		viewportState.pScissors = &scissor;
//
//		VkDynamicState dynamicStates[] = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_LINE_WIDTH };
//
//		VkPipelineDynamicStateCreateInfo dynamicState = {};
//		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
//		dynamicState.dynamicStateCount = 3;
//		dynamicState.pDynamicStates = dynamicStates;
//
//		VkPipelineRasterizationStateCreateInfo rasterizer = {};
//		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
//		rasterizer.depthClampEnable = VK_FALSE;
//		rasterizer.depthBiasSlopeFactor = -1.0f;
//		rasterizer.rasterizerDiscardEnable = VK_FALSE;
//		rasterizer.polygonMode = VKUtils::DrawTypeToVKType(m_Specs.DrawType);
//		rasterizer.lineWidth = 1.0f;
//		rasterizer.cullMode = VKUtils::CullModeToVKMode(m_Specs.CullMode);
//		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
//		rasterizer.depthBiasEnable = VK_FALSE;
//
//		VkPipelineMultisampleStateCreateInfo multisampling = {};
//		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
//		multisampling.sampleShadingEnable = VK_FALSE;
//		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
//
//		uint32_t attachmentCount = m_Specs.TargetRenderPass->GetSpecs().TargetFrameBuffer->GetColorAttachmentCount();
//
//		std::vector<VkPipelineColorBlendAttachmentState> attachments(attachmentCount);
//		uint32_t index = 0;
//
//		for (auto& attachment : attachments) {
//
//			Ref<VulkanImage2D> image = m_Specs.TargetRenderPass->GetSpecs().TargetFrameBuffer->GetImage(index).As<VulkanImage2D>();
//
//			attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
//			attachment.blendEnable = image->GetFormat() == ImageFormat::RGB ? VK_FALSE : VK_TRUE;
//
//			attachment.colorBlendOp = VK_BLEND_OP_ADD;
//			attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//			attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//
//			attachment.alphaBlendOp = VK_BLEND_OP_ADD;
//			attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
//			attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
//			index++;
//		}
//
//		VkPipelineColorBlendStateCreateInfo colorBlending = {};
//		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
//		colorBlending.logicOpEnable = VK_FALSE;
//		colorBlending.logicOp = VK_LOGIC_OP_COPY;
//		colorBlending.attachmentCount = attachments.size();
//		colorBlending.pAttachments = attachments.data();
//		colorBlending.blendConstants[0] = 0.0f;
//		colorBlending.blendConstants[1] = 0.0f;
//		colorBlending.blendConstants[2] = 0.0f;
//		colorBlending.blendConstants[3] = 0.0f;
//
//		VkPipelineDepthStencilStateCreateInfo depthStencil = {};
//		depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
//		depthStencil.depthTestEnable = m_Specs.DepthTest ? VK_TRUE : VK_FALSE;
//		depthStencil.depthWriteEnable = VK_TRUE;
//		depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
//		depthStencil.depthBoundsTestEnable = VK_FALSE;
//		depthStencil.stencilTestEnable = VK_FALSE;
//
//		VkGraphicsPipelineCreateInfo pipelineInfo = {};
//		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
//		pipelineInfo.stageCount = (uint32_t)shaderStages.size();
//		pipelineInfo.pStages = shaderStages.data();
//		pipelineInfo.pViewportState = &viewportState;
//		pipelineInfo.pVertexInputState = &vertexInputInfo;
//		pipelineInfo.pInputAssemblyState = &inputAssembly;
//		pipelineInfo.pRasterizationState = &rasterizer;
//		pipelineInfo.pMultisampleState = &multisampling;
//		pipelineInfo.pColorBlendState = &colorBlending;
//		pipelineInfo.pDepthStencilState = &depthStencil;
//		pipelineInfo.pDynamicState = &dynamicState;
//		pipelineInfo.layout = m_PipelineLayout;
//		pipelineInfo.renderPass = m_Specs.TargetRenderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>()->GetRenderPass();
//		pipelineInfo.subpass = 0;
//		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
//
//		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_Pipeline) != VK_SUCCESS) {
//			HZR_THROW("Failed to create Vulkan Graphics Pipeline!");
//		}
//	}
//	void VulkanPipeline::Bind(Ref<RenderCommandBuffer> commandBuffer)
//	{
//		HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanPipeline::Bind() RT");
//		HZR_CORE_ASSERT(commandBuffer->IsRecording(), "CommandBuffer not in recording state");
//
//		auto& offsets = m_Shader->GetDynamicOffsets();
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		auto cmdBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//
//		m_Shader->Bind(commandBuffer);
//		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_Pipeline);
//		vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_PipelineLayout, 0, 1, m_Shader->GetDescriptorSet(), offsets.size(), offsets.data());
//
//	}
//	void VulkanPipeline::Draw(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
//	{
//		HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanPipeline::Draw() RT");
//		HZR_CORE_ASSERT(commandBuffer->IsRecording(), "CommandBuffer not in recording state");
//
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		auto cmdBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//		vkCmdDrawIndexed(cmdBuffer, count, 1, 0, 0, 0);
//	}
//	void VulkanPipeline::DrawArrays(Ref<RenderCommandBuffer> commandBuffer, uint32_t count)
//	{
//		HZR_PROFILE_FUNCTION("Hazard::Rendering::VulkanPipeline::DrawArrays() RT");
//		HZR_CORE_ASSERT(commandBuffer->IsRecording(), "CommandBuffer not in recording state");
//
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		auto cmdBuffer = commandBuffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//		vkCmdDraw(cmdBuffer, count, 1, 0, 0);
//	}
//	void VulkanPipeline::Destroy()
//	{
//		HZR_PROFILE_FUNCTION();
//		if (!m_Pipeline) return;
//
//		auto device = VulkanContext::GetDevice()->GetDevice();
//		vkDestroyPipeline(device, m_Pipeline, nullptr);
//		vkDestroyPipelineLayout(device, m_PipelineLayout, nullptr);
//	}
//}