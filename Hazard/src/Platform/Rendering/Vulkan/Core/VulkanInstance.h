#pragma once


#include "VulkanWindowSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapChain.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptorPool.h"
#include "../Buffers/VulkanCommandPool.h"
#include "../Buffers/VulkanBuffer.h"

#include "vulkan/vulkan.h"

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
		VulkanRenderPass* RenderPass;
		VulkanDescriptorPool* DescriptorPool;
		VkPipelineCache Cache;

		VulkanCommandPool* CommandPool;
		VulkanBuffer* Buffer;
	};

	class VulkanInstance {
	public:

		VulkanInstance(GLFWwindow* window, bool enableDebugging);
		~VulkanInstance();

		static VulkanInstance& GetInstance() { return *m_VulkanInstance; }
		static VKInstanceData& GetData() { return m_VulkanInstance->data; }
	private:
		static VulkanInstance* m_VulkanInstance;

		VKInstanceData data;
	};
}