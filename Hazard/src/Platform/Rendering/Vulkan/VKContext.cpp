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
		}

		void VKContext::Init(Window* window, ApplicationCreateInfo* appInfo)
		{

			this->m_VulkanData.window = (GLFWwindow*)window->GetNativeWindow();
			m_VulkanData.vkInstance = new VulkanInstance(m_VulkanData.window, appInfo->logging);
			DeviceSpec spec = GetDeviceSpec();
		}

		void VKContext::ClearFrame(glm::vec4 clearColor) const
		{

		}

		void VKContext::SetViewport(int x, int y, int w, int h) const
		{
			
		}

		//void VKContext::SetDepthTest(DepthFunc type) const
		//{

		//}
		//void VKContext::DrawIndexed(VertexArray& array, uint32_t size, DrawType type) const
		//{
		//	array.EnableAll();
		//}

		void VKContext::SetErrorListener(const ErrorCallback& callback)
		{
			VKContext::s_Callback = callback;
		}

		DeviceSpec VKContext::GetDeviceSpec() const
		{
			DeviceSpec spec;
			VkPhysicalDeviceProperties props;
			vkGetPhysicalDeviceProperties(m_VulkanData.vkInstance->GetData().Device->GetPhysicalDevice(), &props);
			std::stringstream ss;

			ss << VK_VERSION_MAJOR(props.apiVersion);
			ss << "." << VK_VERSION_MINOR(props.apiVersion);
			ss << "." << VK_VERSION_PATCH(props.apiVersion);

			spec.renderer = ss.str();
			spec.name = props.deviceName;
			spec.textureSlots = props.limits.framebufferNoAttachmentsSampleCounts + 1;
			return spec;
		}
		void VKContext::SendDebugMessage(const char* message, const char* code)
		{

		}
	}
}