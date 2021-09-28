#pragma once
#include <hzrpch.h>
#include "VulkanSwapChain.h"
#include "../VKUtils.h"

namespace Hazard::Rendering::Vulkan
{
	void VulkanSwapChain::Connect(VkInstance instance, VulkanDevice* device, VkSurfaceKHR surface)
	{
		this->m_Device = device;
		m_Surface = surface;

		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice();

		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(m_Device->GetPhysicalDevice(), m_Surface);
		VkSurfaceFormatKHR surfaceFormat = VKUtils::ChooseSwapChainFormat(details.formats);

		m_PresentMode = VKUtils::ChooseSwapChainPresentMode(details.presentModes);
		m_ColorFormat = surfaceFormat.format;
		m_ColorSpace = surfaceFormat.colorSpace;

		m_ImageCount = details.capabilities.minImageCount + 1;

		if (details.capabilities.maxImageCount > 0 && m_ImageCount > details.capabilities.maxImageCount) {
			m_ImageCount = details.capabilities.maxImageCount;
		}

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Semaphores.PresentComplete);
		vkCreateSemaphore(m_Device->GetDevice(), &semaphoreInfo, nullptr, &m_Semaphores.RenderComplete);

		VkFenceCreateInfo fenceCI = {};
		fenceCI.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCI.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(m_ImageCount);

		for (auto& fence : m_WaitFences) {
			vkCreateFence(m_Device->GetDevice(), &fenceCI, nullptr, &fence);
		}
	}
	void VulkanSwapChain::Create(uint32_t* width, uint32_t* height, bool vsync)
	{
		m_Device->WaitUntilIdle();
		m_Width = *width;
		m_Height = *height;

		m_OldSwapChain = m_SwapChain;

		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(m_Device->GetPhysicalDevice(), m_Surface);

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface;
		createInfo.minImageCount = m_ImageCount;
		createInfo.imageFormat = m_ColorFormat;
		createInfo.imageColorSpace = m_ColorSpace;
		createInfo.imageExtent = { m_Width, m_Height };
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.preTransform = details.capabilities.currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_PresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = m_OldSwapChain;

		QueueFamilyIndices indices = VKUtils::GetQueueFamilyIndices(m_Device->GetPhysicalDevice(), m_Surface);
		uint32_t families[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = 2;
			createInfo.pQueueFamilyIndices = families;
		}
		else {
			createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			createInfo.queueFamilyIndexCount = 0;
			createInfo.pQueueFamilyIndices = nullptr;
		}
		auto result = vkCreateSwapchainKHR(m_Device->GetDevice(), &createInfo, nullptr, &m_SwapChain);
		HZR_CORE_ASSERT(result == VK_SUCCESS, "Failed to create swapchain");

		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, nullptr);
		m_Images.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(m_Device->GetDevice(), m_SwapChain, &m_ImageCount, m_Images.data());

		m_Buffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++) {
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.components = { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G, VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			m_Buffers[i].image = m_Images[i];
			colorAttachmentView.image = m_Images[i];

			vkCreateImageView(m_Device->GetDevice(), &colorAttachmentView, nullptr, &m_Buffers[i].view);
		}
		VkPipelineStageFlags submitFlags = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.pWaitDstStageMask = &submitFlags;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		AllocateCommandBuffers();
		CreateDepthStencil();
		CreateRenderPass();
		CreateFrameBuffer();
	}
	void VulkanSwapChain::SwapBuffers()
	{
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

		vkResetFences(m_Device->GetDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]);

		VkQueue queue = m_Device->GetGraphicsQueue();
		auto submit = vkQueueSubmit(queue, 1, &info, m_WaitFences[m_CurrentBufferIndex]);
		VkResult present = QueuePresent(m_Device->GetPresentQueue(), m_CurrentBufferIndex, m_Semaphores.RenderComplete);
		if (present == VK_ERROR_OUT_OF_DATE_KHR) {
			Clear();
			Create(&m_Width, &m_Height);
			return;
		}
	}
	VkResult VulkanSwapChain::AcquireNextImage(uint32_t* imageIndex)
	{
		auto result = vkAcquireNextImageKHR(m_Device->GetDevice(), m_SwapChain, UINT64_MAX, m_Semaphores.PresentComplete, (VkFence)nullptr, imageIndex);
		m_CurrentBufferIndex = *imageIndex;

		vkWaitForFences(m_Device->GetDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX);

		return result;
	}
	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_SwapChain;
		presentInfo.pImageIndices = &imageIndex;

		if (waitSemaphore) {
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &waitSemaphore;
		}
		return vkQueuePresentKHR(queue, &presentInfo);
	}
	void VulkanSwapChain::Clear()
	{
		auto device = m_Device->GetDevice();

		for (auto& frameBuffer : m_FrameBuffers) {
			vkDestroyFramebuffer(device, frameBuffer, nullptr);
		}

		vkFreeCommandBuffers(device, m_Device->GetCommandPool(), m_DrawCommandBuffers.size(), m_DrawCommandBuffers.data());
		vkDestroyImageView(device, m_DepthStencil.view, nullptr);
		vkDestroyImage(device, m_DepthStencil.image, nullptr);
		vkFreeMemory(device, m_DepthStencil.mem, nullptr);
	}
	void VulkanSwapChain::Cleanup()
	{
		VkDevice device = m_Device->GetDevice();
		m_Device->WaitUntilIdle();
		vkDestroyRenderPass(device, m_RenderPass, nullptr);
		vkDestroyImageView(device, m_DepthStencil.view, nullptr);
		vkDestroyImage(device, m_DepthStencil.image, nullptr);
		vkFreeMemory(device, m_DepthStencil.mem, nullptr);

		for(auto fence : m_WaitFences)
			vkDestroyFence(device, fence, nullptr);

		vkDestroySemaphore(device, m_Semaphores.PresentComplete, nullptr);
		vkDestroySemaphore(device, m_Semaphores.RenderComplete, nullptr);

		for (auto frameBuffer : m_FrameBuffers)
			vkDestroyFramebuffer(device, frameBuffer, nullptr);

		if (m_SwapChain) {
			for (uint32_t i = 0; i < m_ImageCount; i++) {
				vkDestroyImageView(device, m_Buffers[i].view, nullptr);
			}
		}
		if (m_Surface) {
			vkDestroySwapchainKHR(device, m_SwapChain, nullptr);
		}
		m_Surface = VK_NULL_HANDLE;
		m_SwapChain = VK_NULL_HANDLE;
	}
	void VulkanSwapChain::AllocateCommandBuffers()
	{
		m_DrawCommandBuffers.resize(m_ImageCount);

		VkCommandBufferAllocateInfo commandBufferAlloc = {};
		commandBufferAlloc.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAlloc.commandPool = m_Device->GetCommandPool();
		commandBufferAlloc.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAlloc.commandBufferCount = static_cast<uint32_t>(m_DrawCommandBuffers.size());
		vkAllocateCommandBuffers(m_Device->GetDevice(), &commandBufferAlloc, m_DrawCommandBuffers.data());
	}
	void VulkanSwapChain::CreateDepthStencil()
	{
		std::array<VkFormat, 5> depthFormats = {
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
		imageCI.extent = { m_Width, m_Height, 1 };
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
	void VulkanSwapChain::CreateRenderPass()
	{
		std::array<VkAttachmentDescription, 2> attachments = {};
		attachments[0].format = m_ColorFormat;
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
	}
	void VulkanSwapChain::CreateFrameBuffer()
	{
		VkImageView ivAttachments[2];
		ivAttachments[1] = m_DepthStencil.view;

		VkFramebufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass = m_RenderPass;
		frameBufferInfo.attachmentCount = 2;
		frameBufferInfo.pAttachments = ivAttachments;
		frameBufferInfo.width = m_Width;
		frameBufferInfo.height = m_Height;
		frameBufferInfo.layers = 1;

		m_FrameBuffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_FrameBuffers.size(); i++) {
			ivAttachments[0] = m_Buffers[i].view;
			vkCreateFramebuffer(m_Device->GetDevice(), &frameBufferInfo, nullptr, &m_FrameBuffers[i]);
		}
	}
}