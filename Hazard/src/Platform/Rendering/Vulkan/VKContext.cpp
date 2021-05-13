#pragma once

#include <hzrpch.h>
#include "VKContext.h"

namespace Hazard::Rendering {

	namespace Vulkan
	{
		ErrorCallback VKContext::s_Callback;

		bool VKSuitableDevice(VkPhysicalDevice device);
		QueueFamilyIndices VKFindQueueFamilies(VkPhysicalDevice device);

		VKContext::VKContext(Window* window, WindowProps* props)
		{
			this->m_Window = (GLFWwindow*)window->GetNativeWindow();
			Init();
		}

		VKContext::~VKContext()
		{
			vkDestroyInstance(m_VulkanData.instance, nullptr);
		}

		void VKContext::Init() const 
		{
			if (!glfwVulkanSupported()) {
				HZR_THROW("Vulkan not supported");
			}

			VkApplicationInfo appInfo = {};
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hazard";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Hazard";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			VkResult result = vkCreateInstance(&createInfo, nullptr, (VkInstance*)&m_VulkanData.instance);

			if(result != VK_SUCCESS) {
				HZR_THROW("Vulkan vkCreateInstance failed");
			}
			
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(m_VulkanData.instance, &deviceCount, nullptr);

			if (deviceCount == 0) {
				HZR_THROW("Failed to find Vulkan capable device!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(m_VulkanData.instance, &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (VKSuitableDevice(device)) {
					(VkPhysicalDevice)m_VulkanData.physicalDevice = device;
					break;
				}
			}

			if (m_VulkanData.physicalDevice == VK_NULL_HANDLE) {
				HZR_THROW("Failed to get suitable Vulkan device");
			}

			QueueFamilyIndices indices = VKFindQueueFamilies(m_VulkanData.physicalDevice);
			

			VkDeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
			queueCreateInfo.queueCount = 1;


			float queuePriority = 1.0f;
			queueCreateInfo.pQueuePriorities = &queuePriority;

			VkPhysicalDeviceFeatures deviceFeatures = {};

			VkDeviceCreateInfo deviceCreateInfo = {};
			deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

			deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
			deviceCreateInfo.queueCreateInfoCount = 1;
			deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
			deviceCreateInfo.enabledExtensionCount = 0;

			if (vkCreateDevice(m_VulkanData.physicalDevice, &deviceCreateInfo, nullptr, (VkDevice*)&m_VulkanData.device) != VK_SUCCESS) {
				HZR_THROW("failed to create Vulkan logical device!");
			}

			vkGetDeviceQueue((VkDevice)m_VulkanData.device, indices.graphicsFamily.value(), 0, (VkQueue*)&m_VulkanData.queue);

			(uint32_t)m_VulkanData.queueFamily = indices.graphicsFamily.value();
			vkGetDeviceQueue(m_VulkanData.device, m_VulkanData.queueFamily, 0, (VkQueue*)&m_VulkanData.queue);
						
			VkDescriptorPoolSize poolSizes = {};
			poolSizes.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;


			VkDescriptorPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
			poolInfo.poolSizeCount = 1;
			poolInfo.pPoolSizes = &poolSizes;

			if (vkCreateDescriptorPool(m_VulkanData.device, &poolInfo, nullptr, (VkDescriptorPool*)&m_VulkanData.descriptorPool) != VK_SUCCESS) {
				HZR_THROW("Vulkan Create descriptor pool failed");
			}

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



		bool VKSuitableDevice(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices = VKFindQueueFamilies(device);
			return indices.isComplete();
		}
		QueueFamilyIndices VKFindQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices indices;

			uint32_t familyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, nullptr);

			std::vector<VkQueueFamilyProperties> families(familyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &familyCount, families.data());

			int i = 0;
			for (const auto& family : families) {
				if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
					indices.graphicsFamily = i;
				}
				if (indices.isComplete()) break;
				i++;
			}
			return indices;
		}
	}
}