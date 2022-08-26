#pragma once

#include "Backend/Core/Core.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/PhysicalDevice.h"
#include <vector>
#include <vulkan/vulkan.h>

namespace HazardRenderer::Vulkan
{
	struct QueueIndices {
		VkQueue Queue;
		uint32_t Index;
	};

	class SwapChain;
	class CommandBuffer;

	class VulkanDevice : public PhysicalDevice {
	public:
		
		VulkanDevice() = default;
		VulkanDevice(VkInstance instance, VkSurfaceKHR surface, uint32_t imagesInFlight);
		~VulkanDevice();

		std::string GetDeviceName() override { return m_DeviceName; };
		const PhysicalDeviceLimits& GetDeviceLimits() const override { return m_Limits; }

	private:

		std::string m_DeviceName;
		PhysicalDeviceLimits m_Limits;
	};
}
#endif
