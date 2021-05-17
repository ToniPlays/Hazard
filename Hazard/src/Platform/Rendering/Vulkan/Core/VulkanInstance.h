#pragma once


#include "VulkanDevice.h"
#include "VulkanWindowSurface.h"
#include "VulkanSwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan 
{
	struct VKInstanceData 
	{
		GLFWwindow* window;
		VkInstance Instance;
		VulkanWindowSurface* Surface;
		VulkanDevice* Device;
		VulkanSwapChain* SwapChain;
	};

	class VulkanInstance {
	public:

		VulkanInstance(GLFWwindow* window, bool enableDebugging);
		~VulkanInstance();

		static VulkanInstance& GetInstance() { return *m_VulkanInstance; }
		static VKInstanceData GetData() { return GetInstance().data; }

	private:
		static VulkanInstance* m_VulkanInstance;
		VKInstanceData data;
	};
}