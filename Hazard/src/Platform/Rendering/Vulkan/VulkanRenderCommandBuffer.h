#pragma once

#include "Hazard/Rendering/RenderCommandBuffer.h"
#include <vulkan/vulkan.h>

namespace Hazard::Rendering::Vulkan 
{
	class VulkanRenderCommandBuffer : public RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(uint32_t size = 0, const std::string& name = "");
		VulkanRenderCommandBuffer(const std::string& name, bool swapchain);
		~VulkanRenderCommandBuffer();
		virtual RenderStats& GetStats() { return m_Stats; };

		void Begin();
		void End();
		void Submit();
		VkCommandBuffer GetBuffer(uint32_t index) { return m_CommandBuffers[index]; }

	private:
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;

		RenderStats m_Stats;

		bool m_OwnedBySwapchain = false;
	};
}