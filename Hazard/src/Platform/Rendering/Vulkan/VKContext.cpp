#pragma once

#include <hzrpch.h>
#include "VKContext.h"
#include <GLFW/glfw3.h>

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
			delete m_VulkanData.vkInstance;

			/*vkDestroyCommandPool(m_VulkanData.device, m_VulkanData.commandPool, nullptr);

			for (auto frameBuffer : m_VulkanData.swapChainFrameBuffers) {
				vkDestroyFramebuffer(m_VulkanData.device, frameBuffer, nullptr);
			}

			for (auto imageView : m_VulkanData.swapChainImageViews) {
				vkDestroyImageView(m_VulkanData.device, imageView, nullptr);
			}

			vkDestroyDescriptorPool(m_VulkanData.device, m_VulkanData.descriptorPool, nullptr);
			vkDestroySurfaceKHR(m_VulkanData.instance, m_VulkanData.vkSurface, nullptr);
			vkDestroyDevice(m_VulkanData.device, nullptr);
			*/
		}

		void VKContext::Init(Window* window, ApplicationCreateInfo* appInfo)
		{

			this->m_VulkanData.window = (GLFWwindow*)window->GetNativeWindow();
			m_VulkanData.vkInstance = new VKInstance(m_VulkanData.window, appInfo->logging);
			
			
			/*if (!CreateImageViews()) HZR_THROW("Failed to create Vulkan image views");
			if (!CreateRenderPass()) HZR_THROW("Failed to create Vulkan render pass");
			if (!CreateDescriptorPool()) HZR_THROW("Failed to create Vulkan Descriptor pools");
			if (!CreateFramebuffers()) HZR_THROW("Failed to create Vulkan Frame buffers");
			if (!CreateCommandPool()) HZR_THROW("Failed to create Vulkan Command pool");
			if (!CreateCommandBuffers()) HZR_THROW("Failed to create Vulkan Command Buffers");
			*/
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
			vkGetPhysicalDeviceProperties(m_VulkanData.vkInstance->GetData().Device->GetPhysicalDevice(), &props);

			std::stringstream ss;

			ss <<		 VK_VERSION_MAJOR(props.apiVersion);
			ss << "." << VK_VERSION_MINOR(props.apiVersion);
			ss << "." << VK_VERSION_PATCH(props.apiVersion);

			return ss.str();
			return "";
		}
		std::string VKContext::GetDevice() const
		{
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_VulkanData.vkInstance->GetData().Device->GetPhysicalDevice(), &props);
			return props.deviceName;
		}
		void VKContext::SendDebugMessage(const char* message, const char* code)
		{

		}
	}
}