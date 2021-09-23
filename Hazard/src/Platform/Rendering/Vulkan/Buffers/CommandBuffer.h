#pragma once

#include "vulkan/vulkan.h"
#include "../Device/VulkanDevice.h"

namespace Hazard::Rendering::Vulkan
{
	class CommandBuffer {
	public:
		CommandBuffer(VkDevice device, VkCommandPool pool);
		~CommandBuffer();

		void Begin();
		void End();

		VkCommandBuffer* GetBuffer() { return &m_CommandBuffer; }

		static CommandBuffer* Create();
		static void Free(CommandBuffer* buffer);

	private:
		VkCommandBuffer m_CommandBuffer;
	};
}