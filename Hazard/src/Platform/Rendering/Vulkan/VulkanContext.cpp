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

		AllocateCommandBuffers();

		VkFenceCreateInfo fenceCI = {};
		fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(m_DrawCommandBuffers.size());

		for (auto& fence : m_WaitFences) {
			vkCreateFence(m_Device->GetDevice(), &fenceCI, nullptr, &fence);
		}

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Semaphores.PresentComplete);
		vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Semaphores.RenderComplete);

		VkPipelineStageFlags submitFlags = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.pWaitDstStageMask = &submitFlags;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		CreateDepthStencil();

		std::array<VkAttachmentDescription, 2> attachments = {};
		attachments[0].format = m_SwapChain.GetColorFormat();
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		
		attachments[1].format = m_DepthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorRef = {};
		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthRef = {};
		depthRef.attachment = 1;
		depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorRef;
		subpass.pDepthStencilAttachment = &depthRef;
		subpass.inputAttachmentCount = 0;
		subpass.pInputAttachments = nullptr;
		subpass.preserveAttachmentCount = 0;
		subpass.pPreserveAttachments = nullptr;
		subpass.pResolveAttachments = nullptr;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPass = {};
		renderPass.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPass.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPass.pAttachments = attachments.data();
		renderPass.subpassCount = 1;
		renderPass.pSubpasses = &subpass;
		renderPass.dependencyCount = 1;
		renderPass.pDependencies = &dependency;

		vkCreateRenderPass(m_Device->GetDevice(), &renderPass, nullptr, &m_RenderPass);

		VkPipelineCacheCreateInfo cache = {};
		cache.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		vkCreatePipelineCache(m_Device->GetDevice(), &cache, nullptr, &m_PipelineCache);

		CreateFrameBuffer();
		CreateDrawBuffers();
	}

	void VulkanContext::SwapBuffers()
	{
		Render();
	}

	void VulkanContext::SetViewport(int x, int y, int w, int h)
	{
		m_Device->WaitUntilIdle();
		auto device = m_Device->GetDevice();

		uint32_t width = w;
		uint32_t height = h;
		m_SwapChain.Create(&width, &height, m_Window->IsVSync());

		vkDestroyImageView(device, m_DepthStencil.view, nullptr);
		vkDestroyImage(device, m_DepthStencil.image, nullptr);
		vkFreeMemory(device, m_DepthStencil.mem, nullptr);

		vkFreeCommandBuffers(device, m_Device->GetCommandPool(), m_DrawCommandBuffers.size(), m_DrawCommandBuffers.data());

		for (auto& frameBuffer : m_FrameBuffers) {
			vkDestroyFramebuffer(device, frameBuffer, nullptr);
		}
		CreateDepthStencil();
		CreateFrameBuffer();
		AllocateCommandBuffers();
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
	void VulkanContext::CreateDepthStencil()
	{
		std::vector<VkFormat> depthFormats = {
			VK_FORMAT_D32_SFLOAT_S8_UINT,
			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM_S8_UINT,
			VK_FORMAT_D16_UNORM
		};

		for (auto& format : depthFormats) {
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(m_Device->GetPhysicalDevice(), format, &props);
			if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
				m_DepthFormat = format;
				break;
			}
		}

		VkImageCreateInfo imageCI = {};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = m_DepthFormat;
		imageCI.extent = { m_SwapChain.GetWidth(), m_SwapChain.GetHeight(), 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		vkCreateImage(m_Device->GetDevice(), &imageCI, nullptr, &m_DepthStencil.image);
		VkMemoryRequirements memRegs = {};
		vkGetImageMemoryRequirements(m_Device->GetDevice(), m_DepthStencil.image, &memRegs);
		m_Allocator.Allocate(memRegs, &m_DepthStencil.mem);
		vkBindImageMemory(m_Device->GetDevice(), m_DepthStencil.image, m_DepthStencil.mem, 0);

		VkImageViewCreateInfo depthAttachmentView = {};
		depthAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		depthAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		depthAttachmentView.image = m_DepthStencil.image;
		depthAttachmentView.format = m_DepthFormat;
		depthAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		depthAttachmentView.subresourceRange.baseMipLevel = 0;
		depthAttachmentView.subresourceRange.levelCount = 1;
		depthAttachmentView.subresourceRange.baseArrayLayer = 0;
		depthAttachmentView.subresourceRange.layerCount = 1;

		if (m_DepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
			depthAttachmentView.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
		}
		vkCreateImageView(m_Device->GetDevice(), &depthAttachmentView, nullptr, &m_DepthStencil.view);
	}
	void VulkanContext::CreateFrameBuffer()
	{
		VkImageView ivAttachments[2];
		ivAttachments[1] = m_DepthStencil.view;

		VkFramebufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass = m_RenderPass;
		frameBufferInfo.attachmentCount = 2;
		frameBufferInfo.pAttachments = ivAttachments;
		frameBufferInfo.width = m_SwapChain.GetWidth();
		frameBufferInfo.height = m_SwapChain.GetHeight();
		frameBufferInfo.layers = 1;

		m_FrameBuffers.resize(m_SwapChain.GetImageCount());

		for (uint32_t i = 0; i < m_FrameBuffers.size(); i++) {
			ivAttachments[0] = m_SwapChain.GetBuffer(i).view;
			vkCreateFramebuffer(m_Device->GetDevice(), &frameBufferInfo, nullptr, &m_FrameBuffers[i]);
		}
	}
	void VulkanContext::AllocateCommandBuffers()
	{
		m_DrawCommandBuffers.resize(m_SwapChain.GetImageCount());

		VkCommandBufferAllocateInfo commandBufferAlloc = {};
		commandBufferAlloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAlloc.commandPool = m_Device->GetCommandPool();
		commandBufferAlloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAlloc.commandBufferCount = static_cast<uint32_t>(m_DrawCommandBuffers.size());
		vkAllocateCommandBuffers(m_Device->GetDevice(), &commandBufferAlloc, m_DrawCommandBuffers.data());
	}
	void VulkanContext::CreateDrawBuffers()
	{
		VkCommandBufferBeginInfo cmdInfo = {};
		cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		VkClearValue clearVal[2];
		clearVal[0].color = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
		clearVal[1].depthStencil = { 1.0f, 0 };

		uint32_t w = m_SwapChain.GetWidth();
		uint32_t h = m_SwapChain.GetHeight();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.renderPass = m_RenderPass;
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = { w, h };
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearVal;

		for (int32_t i = 0; i < m_DrawCommandBuffers.size(); i++) {
			renderPassBeginInfo.framebuffer = m_FrameBuffers[i];
			auto& buffer = m_DrawCommandBuffers[i];
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
	void VulkanContext::Render()
	{
		auto device = m_Device->GetDevice();
		auto result = m_SwapChain.AcquireNextImage(m_Semaphores.PresentComplete, &m_CurrentBufferIndex);
		if (result == VK_ERROR_OUT_OF_DATE_KHR) {
			SetViewport(0, 0, m_SwapChain.GetWidth(), m_SwapChain.GetHeight());
			return;
		}

		vkWaitForFences(device, 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX);
		vkResetFences(device, 1, &m_WaitFences[m_CurrentBufferIndex]);

		VkPipelineStageFlags waitStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		info.pWaitDstStageMask = &waitStageMask;
		info.waitSemaphoreCount = 1;
		info.pWaitSemaphores = &m_Semaphores.PresentComplete;
		info.signalSemaphoreCount = 1;
		info.pSignalSemaphores = &m_Semaphores.RenderComplete;
		info.commandBufferCount = 1;
		info.pCommandBuffers = &m_DrawCommandBuffers[m_CurrentBufferIndex];

		VkQueue queue = m_Device->GetGraphicsQueue();
		auto submit = vkQueueSubmit(queue, 1, &info, m_WaitFences[m_CurrentBufferIndex]);
		VkResult present = m_SwapChain.QueuePresent(m_Device->GetPresentQueue(), m_CurrentBufferIndex, m_Semaphores.RenderComplete);
		if (present == VK_ERROR_OUT_OF_DATE_KHR) {
			SetViewport(0, 0, m_SwapChain.GetWidth(), m_SwapChain.GetHeight());
			return;
		}
	}
}