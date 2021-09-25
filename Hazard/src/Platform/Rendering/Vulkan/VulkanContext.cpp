#pragma once

#include <hzrpch.h>
#include "VulkanContext.h"
#include "Core/ValidationLayer.h"
#include "VKUtils.h"
#include "Platform/System/Window.h"

namespace Hazard::Rendering::Vulkan {

	ErrorCallback VulkanContext::s_Callback;

	VulkanContext::VulkanContext(WindowProps* props)
	{
		if (!glfwVulkanSupported()) {
			HZR_THROW("Vulkan not supported");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	VulkanContext::~VulkanContext()
	{

	}

	void VulkanContext::Init(Window* window, ApplicationCreateInfo* appInfo)
	{
		m_Window = window;

		auto extensions = VKUtils::GetRequiredExtensions(appInfo->Logging);

		VkApplicationInfo vkApp = {};
		vkApp.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vkApp.pApplicationName = "Hazard";
		vkApp.pEngineName = "Hazard";
		vkApp.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.pApplicationInfo = &vkApp;

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};

		if (ValidationLayer::IsValidationSupported()) {
			ValidationLayer::InitValidationLayers(createInfo, debugInfo, appInfo->Logging);
		}
		vkCreateInstance(&createInfo, nullptr, &m_Instance);

		if (appInfo->Logging) {
			ValidationLayer::SetupDebugger(m_Instance);
		}

		m_WindowSurface = CreateScope<WindowSurface>(m_Instance, (GLFWwindow*)m_Window->GetNativeWindow());
		m_Device = CreateScope<VulkanDevice>(m_Instance, m_WindowSurface->GetVkSurface());

		m_SwapChain.Connect(m_Instance, m_Device.get(), m_WindowSurface->GetVkSurface());

		uint32_t w = window->GetWidth();
		uint32_t h = window->GetHeight();

		m_SwapChain.Create(&w, &h, window->IsVSync());

		VkPipelineCacheCreateInfo cache = {};
		cache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		vkCreatePipelineCache(m_Device->GetDevice(), &cache, nullptr, &m_PipelineCache);

		CreateDrawBuffers();
	}

	void VulkanContext::Begin()
	{
		auto result = m_SwapChain.AcquireNextImage(&m_CurrentBufferIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			SetViewport(0, 0, m_SwapChain.GetWidth(), m_SwapChain.GetHeight());
			return;
		}
	}

	void VulkanContext::SwapBuffers()
	{
		Begin();

		CreateDrawBuffers();

		m_SwapChain.SwapBuffers();
	}

	void VulkanContext::SetViewport(int x, int y, int w, int h)
	{
		m_Device->WaitUntilIdle();
		auto device = m_Device->GetDevice();

		uint32_t width = w;
		uint32_t height = h;
		m_SwapChain.Clear();
		m_SwapChain.Create(&width, &height, m_Window->IsVSync());

		CreateDrawBuffers();

		m_Device->WaitUntilIdle();
	}

	void VulkanContext::DrawIndexed(VertexArray* array, uint32_t size)
	{

	}

	void VulkanContext::SetErrorListener(const ErrorCallback& callback)
	{

	}

	DeviceSpec VulkanContext::GetDeviceSpec() const
	{
		return m_Device->GetSpec();
	}
	void VulkanContext::SendDebugMessage(const char* message, const char* code)
	{
		std::cout << message << std::endl;
	}

	void VulkanContext::CreateDrawBuffers()
	{
		VkCommandBufferBeginInfo cmdInfo = {};
		cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		VkClearValue clearVal[2];
		clearVal[0].color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
		clearVal[1].depthStencil = { 1.0f, 0 };

		uint32_t w = m_SwapChain.GetWidth();
		uint32_t h = m_SwapChain.GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = m_SwapChain.GetRenderPass();
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = { w, h };
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearVal;

		auto buffer = m_SwapChain.GetCurrentDrawCommandBuffer();
		renderPassBeginInfo.framebuffer = m_SwapChain.GetCurrentFrameBuffer();

		vkBeginCommandBuffer(buffer, &cmdInfo);

		vkCmdBeginRenderPass(buffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		VkViewport viewport = {};
		viewport.width = (float)w;
		viewport.height = (float)h;
		viewport.minDepth = (float)0.0f;
		viewport.maxDepth = (float)1.0f;

		vkCmdSetViewport(buffer, 0, 1, &viewport);
		VkRect2D scissors = {};
		scissors.extent = { w, h };
		scissors.offset = { 0, 0 };

		vkCmdSetScissor(buffer, 0, 1, &scissors);
		vkCmdEndRenderPass(buffer);
		vkEndCommandBuffer(buffer);
	}
}