#pragma once


#include "VKDevice.h"
#include "VKWindowSurface.h"
#include "VKSwapChain.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan 
{
	struct VKInstanceData 
	{
		GLFWwindow* window;
		VkInstance Instance;
		VKWindowSurface* Surface;
		VKDevice* Device;
		VKSwapChain* SwapChain;
	};

	class VKInstance {
	public:

		VKInstance(GLFWwindow* window, bool enableDebugging);
		~VKInstance();

		static VKInstance& GetInstance() { return *m_VulkanInstance; }
		static VKInstanceData GetData() { return GetInstance().data; }

	private:
		static VKInstance* m_VulkanInstance;
		VKInstanceData data;
	};
}