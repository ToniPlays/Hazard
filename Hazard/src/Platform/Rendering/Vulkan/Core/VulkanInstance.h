#pragma once


#include "VulkanDevice.h"
#include "VulkanWindowSurface.h"
#include "VulkanSwapChain.h"
#include "VulkanDescriptorPool.h"

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
		VulkanDescriptorPool* DescriptorPool;
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