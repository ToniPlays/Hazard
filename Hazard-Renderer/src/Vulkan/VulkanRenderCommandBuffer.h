#pragma once
#include "Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Core/RenderCommandBuffer.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{

	class VulkanRenderCommandBuffer : public RenderCommandBuffer {
	public:
		VulkanRenderCommandBuffer(uint32_t size = 0, const std::string& name = "");
		VulkanRenderCommandBuffer(const std::string& name, bool swapchain);
		~VulkanRenderCommandBuffer();

		uint32_t GetFrameIndex() { return m_FrameIndex; };
		
		bool IsRecording() { return m_State == State::Record; };
		void Begin();
		void End();
		void Submit();

		VkCommandBuffer GetBuffer(uint32_t index) { return m_CommandBuffers[index]; }
		VkFence GetFence(uint32_t index) { return m_WaitFences[index]; }

	private:
		void SetState(State state) { m_State = state; }
	private:
		std::string m_DebugName;
		VkCommandPool m_CommandPool = nullptr;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		std::vector<VkFence> m_WaitFences;

		uint32_t m_FrameIndex = 0;

		bool m_OwnedBySwapchain = false;

		State m_State = State::Waiting;
	};
}
#endif