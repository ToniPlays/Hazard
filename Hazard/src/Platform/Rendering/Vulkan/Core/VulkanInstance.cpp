#pragma once

#include <hzrpch.h>
#include "VulkanInstance.h"
#include "../VKUtils.h"
#include "VulkanValidationLayer.h"

namespace Hazard::Rendering::Vulkan 
{
	VulkanInstance* VulkanInstance::m_VulkanInstance = nullptr;

	VulkanInstance::VulkanInstance(GLFWwindow* window, bool enableDebugging)
	{
		auto extensions = VKUtils::GetRequiredExtensions(enableDebugging);

		VkApplicationInfo appInfo = {};

		appInfo.sType						= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName			= "Hazard Application";
		appInfo.pEngineName					= "Hazard";
		appInfo.applicationVersion			= VK_MAKE_VERSION(1, 0, 0);
		appInfo.engineVersion				= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion					= VK_API_VERSION_1_0;
		appInfo.pNext						= NULL;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo			= &appInfo;
		createInfo.enabledExtensionCount	= static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames	= extensions.data();

		if (VulkanValidationLayer::IsValidationSupported()) {
			VulkanValidationLayer::InitValidationLayers(createInfo, enableDebugging);
		}
		
		if (vkCreateInstance(&createInfo, nullptr, &data.Instance) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan instance");
		}

		if(enableDebugging)
			VulkanValidationLayer::SetupDebugger(data.Instance);
		
		HZR_CORE_INFO("Created Vulkan instance");
		
		data.window = window;
		m_VulkanInstance = this;

		data.Surface = new VulkanWindowSurface();
		data.Device = new VulkanDevice();
		data.SwapChain = new VulkanSwapChain(data.Device);
		data.RenderPass = new VulkanRenderPass(data.Device, data.SwapChain);
		data.DescriptorPool = new VulkanDescriptorPool(data.Device, data.SwapChain);
		data.SwapChain->CreateFrameBuffers(data.Device, data.RenderPass);

		data.CommandPool = new VulkanCommandPool(data.Device);
		data.Buffer = new VulkanBuffer(data.Device, data.CommandPool);

		VkPipelineCacheCreateInfo cacheInfo = {};
		cacheInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

		if (vkCreatePipelineCache(data.Device->GetDevice(), &cacheInfo, nullptr, &data.Cache) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan PipelineCache");
		}

	}
	VulkanInstance::~VulkanInstance()
	{
		delete data.SwapChain;
		delete data.Device;
		delete data.Surface;

		vkDestroyInstance(data.Instance, nullptr);
	}
}