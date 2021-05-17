#pragma once

namespace Hazard::Rendering::Vulkan {

	class VulkanCommandBuffer {
	public:
		VulkanCommandBuffer();
		~VulkanCommandBuffer();

		void Bind();
		void Unbind();
	};
}