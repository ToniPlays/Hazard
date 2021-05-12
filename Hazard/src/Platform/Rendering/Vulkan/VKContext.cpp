#pragma once

#include <hzrpch.h>
#include "VKContext.h"

namespace Hazard::Rendering {

	namespace Vulkan
	{
		VkInstance instance;
		ErrorCallback VKContext::s_Callback;

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
			return "Vulkan something";
		}
		std::string VKContext::GetDevice() const
		{
			return "VKDevice is not present";
		}
		void VKContext::SendDebugMessage(const char* message, const char* code)
		{

		}
		GLFWvkproc VKContext::GetProc(const char* adress)
		{
			return glfwGetInstanceProcAddress(NULL, adress);
		}
	}
}