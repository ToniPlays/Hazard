#pragma once

#include "Hazard/Rendering/Pipeline.h"
#include <vulkan/vulkan.h>
#include "VulkanShader.h"

namespace Hazard::Rendering::Vulkan 
{
	class VulkanPipeline : public Pipeline 
	{
	public:
		VulkanPipeline(const PipelineSpecification& specs);
		virtual ~VulkanPipeline();

		virtual PipelineSpecification GetSpecifications() { return m_Specs; }
		virtual const PipelineSpecification GetSpecifications() const { return m_Specs; }

		virtual void Invalidate() override;
		virtual void Bind() override;

	private:
		PipelineSpecification m_Specs;
		VkPipeline m_Pipeline;

		VulkanShader* m_Shader;
	};
}