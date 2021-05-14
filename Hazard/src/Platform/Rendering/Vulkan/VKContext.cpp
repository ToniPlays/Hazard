#pragma once

#include <hzrpch.h>
#include "VKContext.h"
#include "VKUtils.h"
#include "VKValidationLayers.h"

#include <set>

namespace Hazard::Rendering {

	namespace Vulkan
	{
		ErrorCallback VKContext::s_Callback;

		VKContext::VKContext(WindowProps* props)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		VKContext::~VKContext()
		{
			vkDestroySurfaceKHR(m_VulkanData.instance, m_VulkanData.vkSurface, nullptr);
			vkDestroyDevice(m_VulkanData.device, nullptr);
			vkDestroyInstance(m_VulkanData.instance, nullptr);
		}

		void VKContext::Init(Window* window)
		{
			this->m_VulkanData.m_Window = (GLFWwindow*)window->GetNativeWindow();

			if (!VKValidationLayer::IsValidationLayersSupported())
				HZR_THROW("Vulkan validation layers not supported");
			
			if (!CreateInstance()) 
				HZR_THROW("Failed to create Vulkan instance");

			VkResult result = glfwCreateWindowSurface(m_VulkanData.instance, m_VulkanData.m_Window, nullptr, (VkSurfaceKHR*)&m_VulkanData.vkSurface);

			switch (result)
			{
			case VK_ERROR_INITIALIZATION_FAILED: HZR_THROW("Vulkan surface failed to initialize");
			case VK_ERROR_EXTENSION_NOT_PRESENT: HZR_THROW("Vulkan surface extension not available");
			case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: HZR_THROW("Vulkan surface error native window");
			}

			(VkPhysicalDevice)m_VulkanData.physicalDevice = VKUtils::GetVulkanDevice(m_VulkanData.instance, m_VulkanData);
			if (m_VulkanData.physicalDevice == VK_NULL_HANDLE)
				HZR_THROW("Failed to get Vulkan device");

			QueueFamilyIndices indices = VKUtils::VKFindQueueFamilies(m_VulkanData.physicalDevice, m_VulkanData.vkSurface);

			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

			float queuePriority = 1.0f;
			for (uint32_t queueFamily : uniqueQueueFamilies) {
				VkDeviceQueueCreateInfo queueCreateInfo{};
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamily;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures{};

			VkDeviceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pQueueCreateInfos = queueCreateInfos.data();

			createInfo.pEnabledFeatures = &deviceFeatures;

			createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
			createInfo.ppEnabledExtensionNames = deviceExtensions.data();


			if (vkCreateDevice(m_VulkanData.physicalDevice, &createInfo, nullptr, (VkDevice*)&m_VulkanData.device) != VK_SUCCESS) {
				HZR_THROW("Failed to create Vulkan physical device!");
			}

			vkGetDeviceQueue(m_VulkanData.device, indices.graphicsFamily.value(), 0, (VkQueue*)&m_VulkanData.queue);
			vkGetDeviceQueue(m_VulkanData.device, m_VulkanData.queueFamily, 0, (VkQueue*)&m_VulkanData.queue);
			
			VKUtils::CreateSwapchain(m_VulkanData);
			CreateImageViews();

			HZR_CORE_WARN(GetVersion());
			HZR_CORE_WARN(GetDevice());
		}

		void VKContext::ClearFrame(glm::vec4 clearColor) const
		{

		}

		void VKContext::SetViewport(int x, int y, int w, int h) const
		{

		}

		void VKContext::SetDepthTest(DepthTest type) const
		{

		}

		void VKContext::Enable(Function fn) const
		{

		}

		void VKContext::Disable(Function fn) const
		{

		}

		void VKContext::DrawIndexed(VertexArray& array, uint32_t size) const
		{
			array.EnableAll();
		}

		void VKContext::SetErrorListener(const ErrorCallback& callback)
		{
			VKContext::s_Callback = callback;
		}

		std::string VKContext::GetVersion() const
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_VulkanData.physicalDevice, &props);

			std::stringstream ss;

			ss <<		 VK_VERSION_MAJOR(props.apiVersion);
			ss << "." << VK_VERSION_MINOR(props.apiVersion);
			ss << "." << VK_VERSION_PATCH(props.apiVersion);

			return ss.str();
		}
		std::string VKContext::GetDevice() const
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_VulkanData.physicalDevice, &props);
			return props.deviceName;
		}
		void VKContext::SendDebugMessage(const char* message, const char* code)
		{

		}
		GLFWvkproc VKContext::GetProc(const char* adress)
		{
			return glfwGetInstanceProcAddress(NULL, adress);
		}

		bool VKContext::CreateInstance() const
		{
			VkApplicationInfo appInfo{};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hazard Application";
			appInfo.pEngineName = "Hazard";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;
			appInfo.pNext = NULL;

			VkInstanceCreateInfo createInfo{};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = VKUtils::GetRequiredExtensions();

			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = extensions.data();
			createInfo.pNext = NULL;

			VKValidationLayer::InitValidationLayers(true, createInfo);
			
			return vkCreateInstance(&createInfo, nullptr, (VkInstance*)&m_VulkanData.instance) == VK_SUCCESS;
		}
		bool VKContext::CreateImageViews()
		{
			m_VulkanData.swapChainImageViews.resize(m_VulkanData.swapChainImages.size());

			for (size_t i = 0; i < m_VulkanData.swapChainImages.size(); i++) {
				
				VkImageViewCreateInfo createInfo{};
				createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				createInfo.image = m_VulkanData.swapChainImages[i];
				createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				createInfo.format = m_VulkanData.swapchainImageFormat;
				createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

				createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				createInfo.subresourceRange.baseMipLevel = 0;
				createInfo.subresourceRange.levelCount = 1;
				createInfo.subresourceRange.baseArrayLayer = 0;
				createInfo.subresourceRange.layerCount = 1;


				if (vkCreateImageView(m_VulkanData.device, &createInfo, nullptr, &m_VulkanData.swapChainImageViews[i]) != VK_SUCCESS) {
					HZR_THROW("Failed to create Vulkan ImageView");
				}
			}

			return true;
		}
	}
}