#pragma once

#include <hzrpch.h>
#include "VKContext.h"

namespace Hazard::Rendering {

	namespace Vulkan
	{
		VkInstance instance;
		VkPhysicalDevice physicalDevice;
		VkDevice device;
		VkQueue graphicsQueue;

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
			vkDestroyInstance(instance, nullptr);
		}

		void VKContext::Init() const 
		{
			if (!glfwVulkanSupported()) {
				HZR_CORE_ERROR("Vulkan not supported");
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

			VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

			if(result != VK_SUCCESS) {
				HZR_CORE_ERROR("Vulkan failed to create instance");
				return;
			}
			
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

			if (deviceCount == 0) {
				throw std::runtime_error("failed to find GPUs with Vulkan support!");
			}

			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

			for (const auto& device : devices) {
				if (VKSuitableDevice(device)) {
					physicalDevice = device;
					break;
				}
			}

			if (physicalDevice == VK_NULL_HANDLE) {
				HZR_CORE_INFO("Failed to get suitable Vulkan device");
				return;
			}

			QueueFamilyIndices indices = VKFindQueueFamilies(physicalDevice);

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

			if (vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device) != VK_SUCCESS) {
				throw std::runtime_error("failed to create logical device!");
			}

			vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);


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
			vkGetPhysicalDeviceProperties(physicalDevice, &props);

			std::stringstream ss;

			ss << VK_VERSION_MAJOR(props.apiVersion);
			ss << "." << VK_VERSION_MINOR(props.apiVersion);
			ss << "." << VK_VERSION_PATCH(props.apiVersion);

			return ss.str();
		}
		std::string VKContext::GetDevice() const
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(physicalDevice, &props);

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