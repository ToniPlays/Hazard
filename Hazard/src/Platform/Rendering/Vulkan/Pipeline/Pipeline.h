#pragma once

#include "vulkan/vulkan.h"
#include "../Device/SwapChain.h"

namespace Hazard::Rendering::Vulkan
{
	class Pipeline {
	public:
		Pipeline(SwapChain* swapChain);
		~Pipeline();

		VkPipeline GetPipeline() { return m_Pipeline; }

	private:
		VkPipelineLayout m_Layout;
		VkPipeline m_Pipeline;
	};
}