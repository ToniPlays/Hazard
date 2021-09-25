#pragma once

#include <hzrpch.h>
#include "Instance.h"
#include "ValidationLayer.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan 
{
	Instance* Instance::s_Instance = nullptr;

	Instance::Instance(GLFWwindow* window, bool enableDebugging)
	{
		s_Instance = this;
		m_Window = window;

		auto extensions = VKUtils::GetRequiredExtensions(enableDebugging);

		VkApplicationInfo appInfo = {};

		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hazard-Client";
		appInfo.pEngineName = "Hazard";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};

		if (ValidationLayer::IsValidationSupported()) {
			ValidationLayer::InitValidationLayers(createInfo, debugInfo, enableDebugging);
		}

		if (vkCreateInstance(&createInfo, nullptr, &m_Instance) != VK_SUCCESS) {
			HZR_THROW("Failed to create Vulkan instance");
		}

		if (enableDebugging)
			ValidationLayer::SetupDebugger(m_Instance);

		m_Surface = new WindowSurface(this, m_Window);
	}
	Instance::~Instance()
	{
		delete m_Surface;
		vkDestroyInstance(m_Instance, nullptr);
	}
}