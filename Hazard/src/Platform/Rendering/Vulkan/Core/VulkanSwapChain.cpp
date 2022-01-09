#pragma once
#include <hzrpch.h>
#include <vulkan/vulkan.h>
#include "VulkanSwapChain.h"
#include "../VulkanContext.h"
#include "../VKUtils.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

namespace Hazard::Rendering::Vulkan
{
	VulkanSwapChain::VulkanSwapChain() {
		m_Instance = VulkanContext::GetInstance();
		m_Device = VulkanContext::GetDevice();
	}
	VulkanSwapChain::~VulkanSwapChain()
	{
		auto device = m_Device->GetDevice();
		Cleanup();

		vkDestroyRenderPass(device, m_RenderPass, nullptr);

		vkDestroySemaphore(device, m_Semaphores.RenderComplete, nullptr);
		vkDestroySemaphore(device, m_Semaphores.PresentComplete, nullptr);

		for (auto fence : m_WaitFences) {
			vkDestroyFence(device, fence, nullptr);
		}
		for (auto fb : m_FrameBuffers) {
			vkDestroyFramebuffer(device, fb, nullptr);
		}
	}
	void VulkanSwapChain::Create(uint32_t width, uint32_t height, bool vSync)
	{
		m_Width = width;
		m_Height = height;
		m_VSync = vSync;
		m_Surface = VulkanContext::GetSurface();

		auto device = m_Device->GetDevice();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice();

		VkPresentModeKHR preferred = m_VSync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR;

		VkSwapchainKHR oldSwapchain = m_Swapchain;
		VkSurfaceCapabilitiesKHR surfCaps = VKUtils::GetSurfaceCapabilities(m_Device->GetPhysicalDevice(), m_Surface);
		SwapChainSupportDetails details = VKUtils::GetSwapChainDetails(m_Device->GetPhysicalDevice(), m_Surface);

		VkExtent2D swapchainExtent = VKUtils::ChooseSwapChainExtent(details.capabilities, (int)width, (int)height);
		VkPresentModeKHR swapchainPresentMode = VKUtils::ChooseSwapChainPresentMode(details.presentModes, preferred);
		VkSurfaceFormatKHR format = VKUtils::ChooseSwapChainFormat(details.formats, VKUtils::GetFormat(ImageFormat::RGBA));
		m_ColorFormat = format.format;
		m_ColorSpace = format.colorSpace;


		// Determine the number of images
		uint32_t desiredNumberOfSwapchainImages = surfCaps.minImageCount + 1;
		if ((surfCaps.maxImageCount > 0) && (desiredNumberOfSwapchainImages > surfCaps.maxImageCount))
		{
			desiredNumberOfSwapchainImages = surfCaps.maxImageCount;
		}

		// Find the transformation of the surface
		VkSurfaceTransformFlagsKHR preTransform;
		if (surfCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			// We prefer a non-rotated transform
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = surfCaps.currentTransform;
		}

		// Find a supported composite alpha format (not all devices support alpha opaque)
		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		// Simply select the first composite alpha format available
		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
		};
		for (auto& compositeAlphaFlag : compositeAlphaFlags) {
			if (surfCaps.supportedCompositeAlpha & compositeAlphaFlag) {
				compositeAlpha = compositeAlphaFlag;
				break;
			};
		}

		VkSwapchainCreateInfoKHR swapchainCI = {};
		swapchainCI.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCI.pNext = NULL;
		swapchainCI.surface = m_Surface;
		swapchainCI.minImageCount = desiredNumberOfSwapchainImages;
		swapchainCI.imageFormat = m_ColorFormat;
		swapchainCI.imageColorSpace = m_ColorSpace;
		swapchainCI.imageExtent = { swapchainExtent.width, swapchainExtent.height };
		swapchainCI.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCI.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainCI.imageArrayLayers = 1;
		swapchainCI.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainCI.queueFamilyIndexCount = 0;
		swapchainCI.pQueueFamilyIndices = NULL;
		swapchainCI.presentMode = swapchainPresentMode;
		swapchainCI.oldSwapchain = oldSwapchain;
		// Setting clipped to VK_TRUE allows the implementation to discard rendering outside of the surface area
		swapchainCI.clipped = VK_TRUE;
		swapchainCI.compositeAlpha = compositeAlpha;

		// Enable transfer source on swap chain images if supported
		if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		// Enable transfer destination on swap chain images if supported
		if (surfCaps.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT) {
			swapchainCI.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		VK_CHECK_RESULT(vkCreateSwapchainKHR(device, &swapchainCI, nullptr, &m_Swapchain));

		// If an existing swap chain is re-created, destroy the old swap chain
		// This also cleans up all the presentable images
		if (oldSwapchain != VK_NULL_HANDLE)
		{
			for (uint32_t i = 0; i < m_ImageCount; i++)
			{
				vkDestroyImageView(device, m_Buffers[i].View, nullptr);
			}
			vkDestroySwapchainKHR(device, oldSwapchain, nullptr);
		}

		VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, m_Swapchain, &m_ImageCount, NULL));
		// Get the swap chain images
		m_Images.resize(m_ImageCount);
		VK_CHECK_RESULT(vkGetSwapchainImagesKHR(device, m_Swapchain, &m_ImageCount, m_Images.data()));

		// Get the swap chain buffers containing the image and imageview
		m_Buffers.resize(m_ImageCount);

		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.pNext = NULL;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.components = {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};
			colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			colorAttachmentView.subresourceRange.baseMipLevel = 0;
			colorAttachmentView.subresourceRange.levelCount = 1;
			colorAttachmentView.subresourceRange.baseArrayLayer = 0;
			colorAttachmentView.subresourceRange.layerCount = 1;
			colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
			colorAttachmentView.flags = 0;

			m_Buffers[i].Image = m_Images[i];

			colorAttachmentView.image = m_Buffers[i].Image;

			VK_CHECK_RESULT(vkCreateImageView(device, &colorAttachmentView, nullptr, &m_Buffers[i].View));
		}

		// Create command buffers
		{
			VkCommandBufferAllocateInfo commandBufferAllocateInfo{};
			commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			commandBufferAllocateInfo.commandPool = m_Device->GetCommandPool();
			commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			uint32_t count = m_ImageCount;
			commandBufferAllocateInfo.commandBufferCount = count;

			m_CommandBuffers.resize(count);
			VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &commandBufferAllocateInfo, m_CommandBuffers.data()));
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// Synchronization Objects
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		VkSemaphoreCreateInfo semaphoreCreateInfo{};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		// Create a semaphore used to synchronize image presentation
		// Ensures that the image is displayed before we start submitting new commands to the queu
		VK_CHECK_RESULT(vkCreateSemaphore(m_Device->GetDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.PresentComplete));
		// Create a semaphore used to synchronize command submission
		// Ensures that the image is not presented until all commands have been sumbitted and executed
		VK_CHECK_RESULT(vkCreateSemaphore(m_Device->GetDevice(), &semaphoreCreateInfo, nullptr, &m_Semaphores.RenderComplete));

		// Set up submit info structure
		// Semaphores will stay the same during application lifetime
		// Command buffer submission info is set by each example
		VkPipelineStageFlags pipelineStageFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.pWaitDstStageMask = &pipelineStageFlags;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		// Wait fences to sync command buffer access
		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		m_WaitFences.resize(RenderContextCommand::GetImagesInFlight());
		for (auto& fence : m_WaitFences)
			VK_CHECK_RESULT(vkCreateFence(m_Device->GetDevice(), &fenceCreateInfo, nullptr, &fence));

		CreateDepthStencil();

		VkFormat depthFormat = m_Device->GetDepthFormat();

		// Render Pass
		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = m_ColorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Depth attachment
		attachments[1].format = depthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		subpassDescription.pDepthStencilAttachment = &depthReference;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 2;
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VK_CHECK_RESULT(vkCreateRenderPass(m_Device->GetDevice(), &renderPassInfo, nullptr, &m_RenderPass));
		CreateFramebuffer();
	}
	void VulkanSwapChain::Resize(uint32_t width, uint32_t height)
	{
		m_Device->WaitUntilIdle();

		for (auto& framebuffer : m_FrameBuffers) {
			vkDestroyFramebuffer(m_Device->GetDevice(), framebuffer, nullptr);
		}
		vkFreeCommandBuffers(m_Device->GetDevice(), m_Device->GetCommandPool(), m_CommandBuffers.size(), m_CommandBuffers.data());

		Create(width, height, m_VSync);

		m_Device->WaitUntilIdle();

		for (auto ev : m_ResizeEvent) {
			ev(m_Width, m_Height);
		}
	}
	void VulkanSwapChain::BeginFrame()
	{
		HZ_SCOPE_PERF("BeginFrame");

		VkResult result = AcquireNextImage(m_Semaphores.PresentComplete, &m_CurrentImageIndex);
		if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				Resize(m_Width, m_Height);
			}
		}
	}
	void VulkanSwapChain::Present()
	{
		HZ_SCOPE_PERF("Present");
		const uint64_t DEFAULT_FENCE_TIMEOUT = 100000000000;

		VkPipelineStageFlags waitFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &waitFlags;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentBufferIndex];
		submitInfo.commandBufferCount = 1;

		VK_CHECK_RESULT(vkResetFences(m_Device->GetDevice(), 1, &m_WaitFences[m_CurrentBufferIndex]));
		VK_CHECK_RESULT(vkQueueSubmit(m_Device->GetGraphicsQueue().Queue, 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]));

		VkResult result = QueuePresent(m_Device->GetGraphicsQueue().Queue, m_CurrentImageIndex, m_Semaphores.RenderComplete);

		if (result != VK_SUCCESS || result == VK_SUBOPTIMAL_KHR) {
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
				Resize(m_Width, m_Height);
				return;
			}
		}
		const uint32_t frameInFlight = RenderContextCommand::GetImagesInFlight();
		m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % frameInFlight;
		VK_CHECK_RESULT(vkWaitForFences(m_Device->GetDevice(), 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX));
	}
	void VulkanSwapChain::Cleanup()
	{
		m_Device->WaitUntilIdle();
		VkDevice device = m_Device->GetDevice();

		if (m_Swapchain)
		{
			for (uint32_t i = 0; i < m_ImageCount; i++)
				vkDestroyImageView(device, m_Buffers[i].View, nullptr);
			vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
		}
		if (m_DepthStencil.allocation) 
		{
			VulkanAllocator allocator("Swapchain");
			//vkDestroyImageView(device, m_DepthStencil.View, nullptr);
			//allocator.DestroyImage(m_DepthStencil.Image, m_DepthStencil.allocation);
		}

		m_Surface = VK_NULL_HANDLE;
		m_Swapchain = VK_NULL_HANDLE;
	}
	VkResult VulkanSwapChain::AcquireNextImage(VkSemaphore presentCompleteSemaphore, uint32_t* imageIndex)
	{
		return vkAcquireNextImageKHR(m_Device->GetDevice(), m_Swapchain, UINT64_MAX, presentCompleteSemaphore, (VkFence)nullptr, imageIndex);
	}
	VkResult VulkanSwapChain::QueuePresent(VkQueue queue, uint32_t imageIndex, VkSemaphore waitSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &imageIndex;

		if (waitSemaphore != VK_NULL_HANDLE) {
			presentInfo.pWaitSemaphores = &waitSemaphore;
			presentInfo.waitSemaphoreCount = 1;
		}
		return vkQueuePresentKHR(queue, &presentInfo);
	}
	void VulkanSwapChain::CreateFramebuffer()
	{
		// Setup Framebuffer
		VkImageView ivAttachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		ivAttachments[1] = m_DepthStencil.View;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = m_RenderPass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = ivAttachments;
		frameBufferCreateInfo.width = m_Width;
		frameBufferCreateInfo.height = m_Height;
		frameBufferCreateInfo.layers = 1;

		m_FrameBuffers.resize(m_ImageCount);
		for (uint32_t i = 0; i < m_FrameBuffers.size(); i++) {
			ivAttachments[0] = m_Buffers[i].View;
			VK_CHECK_RESULT(vkCreateFramebuffer(m_Device->GetDevice(), &frameBufferCreateInfo, nullptr, &m_FrameBuffers[i]));
		}

	}
	void VulkanSwapChain::CreateDepthStencil()
	{
		VkDevice device = m_Device->GetDevice();
		VkFormat depthFormat = m_Device->GetDepthFormat();

		VkImageCreateInfo imageCI{};
		imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCI.imageType = VK_IMAGE_TYPE_2D;
		imageCI.format = depthFormat;
		imageCI.extent = { m_Width, m_Height, 1 };
		imageCI.mipLevels = 1;
		imageCI.arrayLayers = 1;
		imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

		VulkanAllocator allocator("SwapChain");
		m_DepthStencil.allocation = allocator.AllocateImage(imageCI, VMA_MEMORY_USAGE_GPU_ONLY, m_DepthStencil.Image);

		VkImageViewCreateInfo imageViewCI{};
		imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCI.image = m_DepthStencil.Image;
		imageViewCI.format = depthFormat;
		imageViewCI.subresourceRange.baseMipLevel = 0;
		imageViewCI.subresourceRange.levelCount = 1;
		imageViewCI.subresourceRange.baseArrayLayer = 0;
		imageViewCI.subresourceRange.layerCount = 1;
		imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		// Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
		if (depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT)
			imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;

		VK_CHECK_RESULT(vkCreateImageView(device, &imageViewCI, nullptr, &m_DepthStencil.View));
	}
}