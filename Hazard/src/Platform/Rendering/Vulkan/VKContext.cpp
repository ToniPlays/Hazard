#pragma once

#include <hzrpch.h>
#include "VKContext.h"
#include "Device/SwapChain.h"
#include <GLFW/glfw3.h>

namespace Hazard::Rendering::Vulkan {

	ErrorCallback VKContext::s_Callback;

	VKContext::VKContext(WindowProps* props)
	{
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		m_ImagesInFlight = props->ImagesInFlight;
	}

	VKContext::~VKContext()
	{
		delete m_Instance;
	}

	void VKContext::Init(Window* window, ApplicationCreateInfo* appInfo)
	{
		m_Instance = new Instance((GLFWwindow*)window->GetNativeWindow(), appInfo->logging);
		m_Device = new VulkanDevice(m_Instance, m_ImagesInFlight);
	}

	void VKContext::ClearFrame() const
	{

	}

	void VKContext::SetClearColor(glm::vec4 clearColor)
	{

	}

	void VKContext::SetViewport(int x, int y, int w, int h) const
	{
		m_Device->GetSwapChain().ResizeSwapChain(w, h);
	}

	void VKContext::Submit(CommandBuffer* buffer)
	{
		uint32_t index;
		auto result = m_Device->GetSwapChain().AcquireNextImage(&index);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			return;
		}
		m_Device->GetSwapChain().SubmitCommandBuffer(buffer, &index);
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
		return m_Device->GetSpec();
	}
	void VKContext::SendDebugMessage(const char* message, const char* code)
	{
		std::cout << message << std::endl;
	}
}