#pragma once

#include <hzrpch.h>
#include "VulkanContext.h"
#include "Core/ValidationLayer.h"
#include "VKUtils.h"
#include "Platform/System/Window.h"
#include "Pipeline/VulkanRenderPass.h"
#include "VulkanFrameBuffer.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

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
		HZR_PROFILE_FUNCTION();
		m_Device->WaitUntilIdle();
		s_Callback = nullptr;

		vkDestroyPipelineCache(m_Device->GetDevice(), m_PipelineCache, nullptr);

		m_SwapChain.Reset();
		m_WindowSurface.reset();
		VulkanAllocator::Shutdown();
		m_Device.reset();

		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanContext::Init(Window* window, ApplicationCreateInfo* appInfo)
	{
		HZR_PROFILE_FUNCTION();
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
			//ValidationLayer::InitValidationLayers(createInfo, debugInfo, appInfo->Logging);
		}
		vkCreateInstance(&createInfo, nullptr, &m_Instance);

		if (appInfo->Logging) {
			ValidationLayer::SetupDebugger(m_Instance);
		}

		m_WindowSurface = CreateScope<WindowSurface>(m_Instance, (GLFWwindow*)m_Window->GetNativeWindow());
		m_Device = CreateScope<VulkanDevice>(m_Instance, m_WindowSurface->GetVkSurface(), RenderContextCommand::GetImagesInFlight());

		VulkanAllocator::Init();

		uint32_t w = window->GetWidth();
		uint32_t h = window->GetHeight();
		m_SwapChain = Ref<VulkanSwapChain>::Create();
		m_SwapChain->Create(&w, &h, false);
	}

	void VulkanContext::Begin()
	{
		m_SwapChain->BeginFrame();
	}

	void VulkanContext::End()
	{

	}

	void VulkanContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);

		auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>();

		uint32_t width = fb->GetWidth();
		uint32_t height = fb->GetHeight();

		VkViewport viewport = {};
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		viewport.width = width;
		viewport.height = height;

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.Pass = fb->GetRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;

		if (fb->GetSpecification().SwapChainTarget) {
			Ref<VulkanSwapChain> swapchain = VulkanContext::GetSwapchain();

			width = swapchain->GetWidth();
			height = swapchain->GetHeight();
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.Pass = fb->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;
			renderPassBeginInfo.framebuffer = swapchain->GetCurrentFrameBuffer();

			viewport.x = 0.0f;
			viewport.y = (float)height;
			viewport.width = (float)width;
			viewport.height = -(float)height;
		}
		else
		{
			width = fb->GetWidth();
			height = fb->GetHeight();
			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassBeginInfo.Pass = fb->GetRenderPass();
			renderPassBeginInfo.renderArea.offset.x = 0;
			renderPassBeginInfo.renderArea.offset.y = 0;
			renderPassBeginInfo.renderArea.extent.width = width;
			renderPassBeginInfo.renderArea.extent.height = height;
			renderPassBeginInfo.framebuffer = fb->GetFrameBuffer();

			viewport.x = 0.0f;
			viewport.y = 0.0f;
			viewport.width = (float)width;
			viewport.height = (float)height;
		}

		const auto& clearValues = fb->GetVulkanClearValues();
		renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
		renderPassBeginInfo.pClearValues = clearValues.data();

		VkRect2D scissors = {};
		scissors.extent.width = width;
		scissors.extent.height = height;
		scissors.offset = { 0, 0 };

		vkCmdBeginRenderPass(vkBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
 		vkCmdSetViewport(vkBuffer, 0, 1, &viewport);
		vkCmdSetScissor(vkBuffer, 0, 1, &scissors);
	}

	void VulkanContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
		vkCmdEndRenderPass(vkBuffer);
	}
	void VulkanContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
	{
		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);

		vkCmdSetLineWidth(vkBuffer, lineWidth);
	}

	void VulkanContext::SwapBuffers()
	{
		m_SwapChain->Present();
	}

	void VulkanContext::SetViewport(int x, int y, int w, int h)
	{
		if (w == 0 || h == 0) return;

		auto device = m_Device->GetDevice();
		m_Device->WaitUntilIdle();

		uint32_t width = w;
		uint32_t height = h;

		m_SwapChain->Resize(width, height);
	}

	void VulkanContext::SetErrorListener(const ErrorCallback& callback)
	{
		s_Callback = callback;
	}

	DeviceSpec VulkanContext::GetDeviceSpec() const
	{
		return m_Device->GetSpec();
	}
	void VulkanContext::SendDebugMessage(const char* message, const char* code)
	{
		std::cout << message << std::endl;
		if (s_Callback != nullptr) {
			ErrorData data(message, code);
			s_Callback(data);
		}
	}
	VkDescriptorSet VulkanContext::RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo) 
	{
		uint32_t bufferIndex = m_SwapChain->GetCurrentBufferIndex();
		allocInfo.descriptorPool = m_Device->GetDescriptorPool(bufferIndex);
		VkDescriptorSet result;
		VK_CHECK_RESULT(vkAllocateDescriptorSets(m_Device->GetDevice(), &allocInfo, &result));
		m_DescriptorAllocations[bufferIndex] += allocInfo.descriptorSetCount;
		return result;
	}
}