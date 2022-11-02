#include "VulkanPipeline.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"
#include "../VulkanContext.h"
#include "VulkanShader.h"
#include "../VulkanFramebuffer.h"
#include "../VkUtils.h"
#include "spdlog/fmt/fmt.h"

namespace HazardRenderer::Vulkan
{
	VulkanPipeline::VulkanPipeline(PipelineSpecification* specs) : m_Specs(*specs)
	{
		HZR_PROFILE_FUNCTION();

		std::vector<ShaderStageCode> code(specs->ShaderCodeCount);
		for (uint32_t i = 0; i < specs->ShaderCodeCount; i++)
			code[i] = specs->pShaderCode[i];

		m_Shader = Shader::Create(code).As<VulkanShader>();
		if(specs->pBufferLayout)
			m_Layout = BufferLayout::Copy(*specs->pBufferLayout);

		if (!IsCompatibleWith(m_Shader))
		{
			std::cout << m_Specs.DebugName << " is not compatible with shader" << std::endl;
			return;
		}

		Invalidate();
	}
	VulkanPipeline::~VulkanPipeline()
	{
		HZR_PROFILE_FUNCTION();
		Renderer::SubmitResourceFree([pipeline = m_Pipeline, cache = m_PipelineCache, layout = m_PipelineLayout]() mutable {
			const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

			vkDestroyPipeline(device, pipeline, nullptr);
			vkDestroyPipelineLayout(device, layout, nullptr);
			vkDestroyPipelineCache(device, cache, nullptr);
			});
	}
	void VulkanPipeline::SetRenderPass(Ref<RenderPass> renderPass)
	{
		HZR_PROFILE_FUNCTION();
		if (m_Specs.pTargetRenderPass == renderPass) return;

		m_Specs.pTargetRenderPass = renderPass;
		Invalidate();
	}
	bool VulkanPipeline::IsCompatibleWith(Ref<Shader> shader) const
	{
		auto& data = shader->GetShaderData().Stages;
		uint32_t stride = m_Layout.GetStride();
		if (data.find(ShaderStage::Vertex) == data.end() && stride == 0)
			return true;

		return data.at(ShaderStage::Vertex).Stride == stride;
	}
	void VulkanPipeline::Invalidate()
	{
		HZR_PROFILE_FUNCTION();
		if (m_Specs.pTargetRenderPass == nullptr) return;

		Ref<VulkanPipeline> instance = this;
		Renderer::SubmitResourceCreate([instance]() mutable {
			instance->Invalidate_RT();
			});
	}
	VkPipelineBindPoint VulkanPipeline::GetBindingPoint() const
	{
		return m_Specs.Usage == PipelineUsage::GraphicsBit ? VK_PIPELINE_BIND_POINT_GRAPHICS : VK_PIPELINE_BIND_POINT_COMPUTE;
	}

	void VulkanPipeline::Invalidate_RT()
	{
		if (m_Specs.Usage == PipelineUsage::GraphicsBit)
			InvalidateGraphicsPipeline();
		else
			InvalidateComputePipeline();
	}

	void VulkanPipeline::InvalidateGraphicsPipeline()
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(m_Specs.Usage == PipelineUsage::GraphicsBit, "Pipeline is not a graphics pipeline");
		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();
		auto& fb = m_Specs.pTargetRenderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>();
		HZR_ASSERT(m_Shader, "No shader");

		auto& setLayouts = m_Shader->GetAllDescriptorSetLayouts();
		const auto& pushConstantRanges = m_Shader->GetPushConstantRanges();

		std::vector<VkPushConstantRange> vulkanPushConstantRanges(pushConstantRanges.size());
		for (uint32_t i = 0; i < pushConstantRanges.size(); i++)
		{
			const auto& range = pushConstantRanges[i];
			auto& vkRange = vulkanPushConstantRanges[i];

			vkRange.stageFlags = range.Stages;
			vkRange.offset = range.Offset;
			vkRange.size = range.Size;
		}
		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.setLayoutCount = (uint32_t)setLayouts.size();
		layoutInfo.pSetLayouts = setLayouts.data();
		layoutInfo.pushConstantRangeCount = (uint32_t)vulkanPushConstantRanges.size();
		layoutInfo.pPushConstantRanges = vulkanPushConstantRanges.data();

		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &layoutInfo, nullptr, &m_PipelineLayout), "Failed to create pipeline layout");

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = fb->GetRenderPass();

		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VkUtils::GetVulkanTopology(m_Specs.DrawType);

		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.polygonMode = VkUtils::GetVulkanPolygonMode(m_Specs.DrawType);
		rasterizationState.cullMode = VkUtils::GetVulkanCullMode(m_Specs.CullMode);
		rasterizationState.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationState.depthClampEnable = VK_FALSE;
		rasterizationState.rasterizerDiscardEnable = VK_FALSE;
		rasterizationState.depthBiasEnable = VK_FALSE;
		rasterizationState.lineWidth = m_Specs.LineWidth;

		size_t colorAttachmentCount = fb->GetSpecification().SwapChainTarget ? 1 : fb->GetColorAttachmentCount();
		std::vector<VkPipelineColorBlendAttachmentState> blendStates(colorAttachmentCount);
		if (fb->GetSpecification().SwapChainTarget)
		{
			blendStates[0].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
			blendStates[0].blendEnable = VK_TRUE;
			blendStates[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendStates[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blendStates[0].colorBlendOp = VK_BLEND_OP_ADD;
			blendStates[0].alphaBlendOp = VK_BLEND_OP_ADD;
			blendStates[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blendStates[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		}
		else
		{
			for (size_t i = 0; i < colorAttachmentCount; i++)
			{
				blendStates[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

				const auto& spec = fb->GetSpecification().Attachments[i];

				blendStates[i].blendEnable = spec.Blend ? VK_TRUE : VK_FALSE;
				blendStates[i].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				blendStates[i].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
				blendStates[i].colorBlendOp = VK_BLEND_OP_ADD;
				blendStates[i].alphaBlendOp = VK_BLEND_OP_ADD;
				blendStates[i].srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
				blendStates[i].dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			}
		}
		VkPipelineColorBlendStateCreateInfo blendStateInfo = {};
		blendStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		blendStateInfo.attachmentCount = (uint32_t)blendStates.size();
		blendStateInfo.pAttachments = blendStates.data();

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.scissorCount = 1;

		std::vector<VkDynamicState> dynamicStates;
		dynamicStates.push_back(VK_DYNAMIC_STATE_VIEWPORT);
		dynamicStates.push_back(VK_DYNAMIC_STATE_SCISSOR);

		if (m_Specs.DrawType == DrawType::Line)
			dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = (uint32_t)dynamicStates.size();
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineDepthStencilStateCreateInfo depthStencilCreateInfo = {};
		depthStencilCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilCreateInfo.depthTestEnable = m_Specs.DepthTest ? VK_TRUE : VK_FALSE;
		depthStencilCreateInfo.depthWriteEnable = m_Specs.DepthWrite ? VK_TRUE : VK_FALSE;
		depthStencilCreateInfo.depthCompareOp = VkUtils::GetVulkanCompareOp(m_Specs.DepthOperator);
		depthStencilCreateInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
		depthStencilCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
		depthStencilCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
		depthStencilCreateInfo.stencilTestEnable = VK_FALSE;
		depthStencilCreateInfo.front = depthStencilCreateInfo.back;

		VkPipelineMultisampleStateCreateInfo multisample = {};
		multisample.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisample.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisample.pSampleMask = nullptr;

		std::vector<VkVertexInputBindingDescription> vertexInputBinding;
		VkVertexInputBindingDescription& vertexDesc = vertexInputBinding.emplace_back();
		vertexDesc.binding = 0;
		vertexDesc.stride = m_Layout.GetBufferStride(PerVertex);
		vertexDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		if (m_Layout.GetBufferStride(PerInstance))
		{
			VkVertexInputBindingDescription& instanceDesc = vertexInputBinding.emplace_back();
			instanceDesc.binding = 1;
			instanceDesc.stride = m_Layout.GetBufferStride(PerInstance);
			instanceDesc.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		}

		std::vector<VkVertexInputAttributeDescription> inputAttrib(m_Layout.GetElementCount());
		uint32_t location = 0;
		for (auto& element : m_Layout.GetElements())
		{
			if (element.ElementDivisor != PerVertex) continue;
			inputAttrib[location].binding = PerVertex;
			inputAttrib[location].location = location;
			inputAttrib[location].format = VkUtils::ShaderDataTypeToVulkanType(element.Type);
			inputAttrib[location].offset = element.Offset;
			location++;
		}
		for (auto& element : m_Layout.GetElements())
		{
			if (element.ElementDivisor != PerInstance) continue;
			inputAttrib[location].binding = PerInstance;
			inputAttrib[location].location = location;
			inputAttrib[location].format = VkUtils::ShaderDataTypeToVulkanType(element.Type);
			inputAttrib[location].offset = element.Offset;
			location++;
		}

		VkPipelineVertexInputStateCreateInfo inputStateInfo = {};
		inputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		inputStateInfo.vertexBindingDescriptionCount = (uint32_t)vertexInputBinding.size();
		inputStateInfo.pVertexBindingDescriptions = vertexInputBinding.data();
		inputStateInfo.vertexAttributeDescriptionCount = (uint32_t)inputAttrib.size();
		inputStateInfo.pVertexAttributeDescriptions = inputAttrib.data();

		const auto& shaderStages = m_Shader->GetPipelineShaderStageCreateInfos();

		pipelineInfo.stageCount = (uint32_t)shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();

		pipelineInfo.pVertexInputState = &inputStateInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pRasterizationState = &rasterizationState;
		pipelineInfo.pColorBlendState = &blendStateInfo;
		pipelineInfo.pMultisampleState = &multisample;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pDepthStencilState = &depthStencilCreateInfo;
		pipelineInfo.renderPass = fb->GetRenderPass();
		pipelineInfo.pDynamicState = &dynamicState;

		VkPipelineCacheCreateInfo pipelineCacheInfo = {};
		pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr, &m_PipelineCache), "Failed to create VK");

		VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, m_PipelineCache, 1, &pipelineInfo, nullptr, &m_Pipeline), "Failed to create VkPipeline");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, fmt::format("VkPipeline {0}", m_Specs.DebugName), m_Pipeline);
	}
	void VulkanPipeline::InvalidateComputePipeline()
	{
		HZR_PROFILE_FUNCTION();
		HZR_ASSERT(m_Specs.Usage == PipelineUsage::ComputeBit, "Pipeline is not a compute pipeline");

		const auto device = VulkanContext::GetLogicalDevice()->GetVulkanDevice();

		auto& setLayouts = m_Shader->GetAllDescriptorSetLayouts();

		VkPipelineLayoutCreateInfo pipelineLayout = {};
		pipelineLayout.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayout.setLayoutCount = setLayouts.size();
		pipelineLayout.pSetLayouts = setLayouts.data();

		VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayout, nullptr, &m_PipelineLayout), "Failed to create Pipeline layout");

		VkPipelineShaderStageCreateInfo computeShaderInfo = m_Shader->GetPipelineShaderStageCreateInfos()[0];

		VkComputePipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.stage = computeShaderInfo;

		VkPipelineCacheCreateInfo pipelineCacheInfo = {};
		pipelineCacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheInfo, nullptr, &m_PipelineCache), "Failed to create VK");

		VK_CHECK_RESULT(vkCreateComputePipelines(device, m_PipelineCache, 1, &pipelineInfo, nullptr, &m_Pipeline), "Failed to create VkPipeline");
		VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_PIPELINE, fmt::format("VkPipeline {0}", m_Specs.DebugName), m_Pipeline);
	}
}

#endif