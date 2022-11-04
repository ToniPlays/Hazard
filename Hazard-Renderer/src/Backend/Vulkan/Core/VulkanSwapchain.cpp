
#include "VulkanSwapchain.h"
#ifdef HZR_INCLUDE_VULKAN

#include "Backend/Core/Renderer.h"
#include "../VulkanContext.h"
#include "../VkUtils.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#include "spdlog/fmt/fmt.h"

#include <array>
#include <Profiling/PerformanceProfiler.h>


static PFN_vkGetPhysicalDeviceSurfaceSupportKHR fpGetPhysicalDeviceSurfaceSupportKHR;
static PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR fpGetPhysicalDeviceSurfaceCapabilitiesKHR;
static PFN_vkGetPhysicalDeviceSurfaceFormatsKHR fpGetPhysicalDeviceSurfaceFormatsKHR;
static PFN_vkGetPhysicalDeviceSurfacePresentModesKHR fpGetPhysicalDeviceSurfacePresentModesKHR;
static PFN_vkCreateSwapchainKHR fpCreateSwapchainKHR;
static PFN_vkDestroySwapchainKHR fpDestroySwapchainKHR;
static PFN_vkGetSwapchainImagesKHR fpGetSwapchainImagesKHR;
static PFN_vkAcquireNextImageKHR fpAcquireNextImageKHR;
static PFN_vkQueuePresentKHR fpQueuePresentKHR;

// Nvidia extensions
PFN_vkCmdSetCheckpointNV fpCmdSetCheckpointNV;
PFN_vkGetQueueCheckpointDataNV fpGetQueueCheckpointDataNV;

VKAPI_ATTR void VKAPI_CALL vkCmdSetCheckpointNV(
	VkCommandBuffer                             commandBuffer,
	const void* pCheckpointMarker)
{
	fpCmdSetCheckpointNV(commandBuffer, pCheckpointMarker);
}

VKAPI_ATTR void VKAPI_CALL vkGetQueueCheckpointDataNV(
	VkQueue                                     queue,
	uint32_t* pCheckpointDataCount,
	VkCheckpointDataNV* pCheckpointData)
{
	fpGetQueueCheckpointDataNV(queue, pCheckpointDataCount, pCheckpointData);
}

namespace HazardRenderer::Vulkan
{
	VulkanSwapchain::VulkanSwapchain()
	{
		HZR_PROFILE_FUNCTION();
	}
	VulkanSwapchain::~VulkanSwapchain()
	{
		HZR_PROFILE_FUNCTION();
	}
	void VulkanSwapchain::Init(VkInstance instance, Ref<VulkanDevice> device)
	{
		HZR_PROFILE_FUNCTION();
		m_Instance = instance;
		m_Device = device;

		VkDevice vkDevice = device->GetVulkanDevice();

		GET_DEVICE_PROC_ADDR(vkDevice, CreateSwapchainKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, DestroySwapchainKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, GetSwapchainImagesKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, AcquireNextImageKHR);
		GET_DEVICE_PROC_ADDR(vkDevice, QueuePresentKHR);

		GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceSupportKHR);
		GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceCapabilitiesKHR);
		GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfaceFormatsKHR);
		GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceSurfacePresentModesKHR);

		GET_INSTANCE_PROC_ADDR(instance, CmdSetCheckpointNV);
		GET_INSTANCE_PROC_ADDR(instance, GetQueueCheckpointDataNV);

	}
	void VulkanSwapchain::InitSurface(GLFWwindow* nativeWindow)
	{
		HZR_PROFILE_FUNCTION();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetVulkanPhysicalDevice();

		glfwCreateWindowSurface(m_Instance, nativeWindow, nullptr, &m_Surface);

		uint32_t queueCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
		HZR_ASSERT(queueCount > 0, "Unable to get Physical Device Queue Family Properties");

		std::vector<VkQueueFamilyProperties> properties(queueCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, properties.data());

		std::vector<VkBool32> supportsPresent(queueCount);
		for (uint32_t i = 0; i < queueCount; i++)
			fpGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, m_Surface, &supportsPresent[i]);

		uint32_t graphicQueueIndex = UINT32_MAX;
		uint32_t presentQueueIndex = UINT32_MAX;

		for (uint32_t i = 0; i < queueCount; i++)
		{
			if ((properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0)
			{
				graphicQueueIndex = i;
			}
			if (supportsPresent[i] == VK_TRUE)
			{
				graphicQueueIndex = i;
				presentQueueIndex = i;
				break;
			}
		}

		if (presentQueueIndex == UINT32_MAX)
		{
			for (uint32_t i = 0; i < queueCount; i++)
			{
				if (supportsPresent[i] == VK_TRUE) {
					presentQueueIndex = i;
					break;
				}
			}
		}
		HZR_ASSERT(graphicQueueIndex != UINT32_MAX, "Graphics queue not found");
		HZR_ASSERT(presentQueueIndex != UINT32_MAX, "Present queue not found");

		m_QueueNodeIndex = graphicQueueIndex;

		FindImageFormatAndColorSpace();
	}
	void VulkanSwapchain::Create(uint32_t* width, uint32_t* height, bool enableVSync)
	{
		HZR_PROFILE_FUNCTION();
		m_VSync = enableVSync;

		VkDevice device = m_Device->GetVulkanDevice();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetVulkanPhysicalDevice();

		VkSwapchainKHR oldSwapchain = m_Swapchain;
		VkSurfaceCapabilitiesKHR capabilities;
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_Surface, &capabilities), "Failed to get surface capabilities");

		uint32_t presentModeCount;
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, nullptr), "Failed to get present modes");
		HZR_ASSERT(presentModeCount > 0, "");

		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_Surface, &presentModeCount, presentModes.data()), "Failed to get present modes");

		VkExtent2D swapchainExtent = {};
		if (capabilities.currentExtent.width == 0xFFFFFFFF) {
			swapchainExtent.width = *width;
			swapchainExtent.height = *height;
		}
		else
		{
			swapchainExtent = capabilities.currentExtent;
			*width = capabilities.currentExtent.width;
			*height = capabilities.currentExtent.height;
		}

		m_Width = *width;
		m_Height = *height;

		VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

		//Select present mode
		if (!enableVSync)
		{
			for (size_t i = 0; i < presentModeCount; i++)
			{
				if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}
				if ((presentMode != VK_PRESENT_MODE_MAILBOX_KHR) && (presentModes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR))
				{
					presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
					break;
				}
			}
		}

		//Get image count for swapchain
		uint32_t desiredImageCount = capabilities.minImageCount + 1;
		if ((capabilities.maxImageCount > 0) && (desiredImageCount > capabilities.maxImageCount))
		{
			desiredImageCount = capabilities.maxImageCount;
		}

		VkSurfaceTransformFlagsKHR preTransform;
		if (capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		else
			preTransform = capabilities.currentTransform;

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

		std::vector<VkCompositeAlphaFlagBitsKHR> compositeAlphaFlags = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
		};

		for (auto& flag : compositeAlphaFlags)
		{
			if (capabilities.supportedCompositeAlpha & flag)
			{
				compositeAlpha = flag;
				break;
			}
		}

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = m_Surface;
		swapchainInfo.minImageCount = desiredImageCount;
		swapchainInfo.imageFormat = m_ColorFormat;
		swapchainInfo.imageColorSpace = m_ColorSpace;
		swapchainInfo.imageExtent = { swapchainExtent.width, swapchainExtent.height };
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainInfo.preTransform = (VkSurfaceTransformFlagBitsKHR)preTransform;
		swapchainInfo.imageArrayLayers = 1;
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapchainInfo.queueFamilyIndexCount = 0;
		swapchainInfo.pQueueFamilyIndices = NULL;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.oldSwapchain = oldSwapchain;

		swapchainInfo.clipped = VK_TRUE;
		swapchainInfo.compositeAlpha = compositeAlpha;

		if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT)
			swapchainInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		if (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_DST_BIT)
			swapchainInfo.imageUsage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;

		VK_CHECK_RESULT(fpCreateSwapchainKHR(device, &swapchainInfo, nullptr, &m_Swapchain), "Failed to create swapchain");
		if (oldSwapchain) fpDestroySwapchainKHR(device, oldSwapchain, nullptr);

		for (auto& image : m_Images)
			vkDestroyImageView(device, image.ImageView, nullptr);
		m_Images.clear();

		VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_Swapchain, &m_ImageCount, nullptr), "");
		m_Images.resize(m_ImageCount);
		m_VulkanImages.resize(m_ImageCount);

		VK_CHECK_RESULT(fpGetSwapchainImagesKHR(device, m_Swapchain, &m_ImageCount, m_VulkanImages.data()), "");

		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			VkImageViewCreateInfo colorAttachmentView = {};
			colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			colorAttachmentView.format = m_ColorFormat;
			colorAttachmentView.image = m_VulkanImages[i];
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

			m_Images[i].Image = m_VulkanImages[i];

			VK_CHECK_RESULT(vkCreateImageView(device, &colorAttachmentView, nullptr, &m_Images[i].ImageView), "Failed to create VkImageView");
			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_IMAGE_VIEW, fmt::format("Swapchain ImageView: {}", i), m_Images[i].ImageView);
		}

		//Command buffers
		for (auto& commandBuffer : m_CommandBuffers)
			vkDestroyCommandPool(device, commandBuffer.CommandPool, nullptr);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = m_QueueNodeIndex;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		m_CommandBuffers.resize(m_ImageCount);
		for (auto& buffer : m_CommandBuffers)
		{
			VK_CHECK_RESULT(vkCreateCommandPool(device, &poolInfo, nullptr, &buffer.CommandPool), "Failed to create command pool");
			allocInfo.commandPool = buffer.CommandPool;
			VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &allocInfo, &buffer.CommandBuffer), "Failed to allocate command buffer");
		}

		if (!m_Semaphores.RenderComplete || !m_Semaphores.PresentComplete)
		{
			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_Semaphores.RenderComplete), "Failed to create semaphore");

			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, "Swapchain RenderComplete Semaphore", m_Semaphores.RenderComplete);

			VK_CHECK_RESULT(vkCreateSemaphore(device, &semaphoreInfo, nullptr, &m_Semaphores.PresentComplete), "Failed to create semaphore");
			VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_SEMAPHORE, "Swapchain PresentComplete Semaphore", m_Semaphores.PresentComplete);
		}

		if (m_WaitFences.size() != m_ImageCount)
		{
			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

			m_WaitFences.resize(m_ImageCount);
			for (auto& fence : m_WaitFences)
			{
				VK_CHECK_RESULT(vkCreateFence(device, &fenceInfo, nullptr, &fence), "Failed to create VkFence");
				VkUtils::SetDebugUtilsObjectName(device, VK_OBJECT_TYPE_FENCE, "Swapchain Fence", fence);
			}
		}

		m_SubmitInfo = {};
		m_SubmitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		m_SubmitInfo.waitSemaphoreCount = 1;
		m_SubmitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		m_SubmitInfo.signalSemaphoreCount = 1;
		m_SubmitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;

		VkFormat depthFormat = m_Device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetDepthFormat();

		//Renderpass
		VkAttachmentDescription colorDesc = {};
		colorDesc.format = m_ColorFormat;
		colorDesc.samples = VK_SAMPLE_COUNT_1_BIT;
		colorDesc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;;
		colorDesc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorDesc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorDesc.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorRef = {};
		colorRef.attachment = 0;
		colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthRef = {};
		depthRef.attachment = 1;
		depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDesc = {};
		subpassDesc.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDesc.colorAttachmentCount = 1;
		subpassDesc.pColorAttachments = &colorRef;
		subpassDesc.inputAttachmentCount = 0;
		subpassDesc.pInputAttachments = nullptr;
		subpassDesc.preserveAttachmentCount = 0;
		subpassDesc.pPreserveAttachments = nullptr;
		subpassDesc.pResolveAttachments = nullptr;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorDesc;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDesc;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		VK_CHECK_RESULT(vkCreateRenderPass(m_Device->GetVulkanDevice(), &renderPassInfo, nullptr, &m_RenderPass), "Failed to create RenderPass");
		VkUtils::SetDebugUtilsObjectName(m_Device->GetVulkanDevice(), VK_OBJECT_TYPE_RENDER_PASS, "Swapchain RenderPass", m_RenderPass);

		for (auto& framebuffer : m_FrameBuffers)
			vkDestroyFramebuffer(m_Device->GetVulkanDevice(), framebuffer, nullptr);

		VkFramebufferCreateInfo frameBufferInfo = {};
		frameBufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferInfo.renderPass = m_RenderPass;
		frameBufferInfo.attachmentCount = 1;
		frameBufferInfo.width = m_Width;
		frameBufferInfo.height = m_Height;
		frameBufferInfo.layers = 1;

		m_FrameBuffers.resize(m_ImageCount);
		for (uint32_t i = 0; i < m_ImageCount; i++)
		{
			frameBufferInfo.pAttachments = &m_Images[i].ImageView;

			VK_CHECK_RESULT(vkCreateFramebuffer(m_Device->GetVulkanDevice(), &frameBufferInfo, nullptr, &m_FrameBuffers[i]), "Failed to create Framebuffer");
			VkUtils::SetDebugUtilsObjectName(m_Device->GetVulkanDevice(), VK_OBJECT_TYPE_FRAMEBUFFER, "Swapchain Framebuffer", m_FrameBuffers[i]);
		}
		
		m_RenderCommandBuffer = RenderCommandBuffer::CreateFromSwapchain("SwapchainCommandBuffer");

		if (m_DefaultFramebuffer)
		{
			m_DefaultFramebuffer->Resize_RT(m_Width, m_Height);
			return;
		}

		if (nullptr == nullptr) //TODO: Fix this
		{
			//Create default target

			FrameBufferCreateInfo frameBufferInfo = {};
			frameBufferInfo.DebugName = "ScreenFBO";
			frameBufferInfo.SwapChainTarget = true;
			frameBufferInfo.AttachmentCount = 2;
			frameBufferInfo.Attachments = { { ImageFormat::RGBA }, { ImageFormat::Depth } };

			m_DefaultFramebuffer = FrameBuffer::Create(&frameBufferInfo);

			RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.DebugName = "ScreenTarget";
			renderPassInfo.pTargetFrameBuffer = m_DefaultFramebuffer;

			m_DefaultRenderPass = RenderPass::Create(&renderPassInfo);
		}
		else
		{
			m_DefaultFramebuffer = FrameBuffer::Create(nullptr);

			RenderPassCreateInfo renderPassInfo = {};
			renderPassInfo.DebugName = "ScreenTarget";
			renderPassInfo.pTargetFrameBuffer = m_DefaultFramebuffer;

			m_DefaultRenderPass = RenderPass::Create(&renderPassInfo);
		}
	}
	void VulkanSwapchain::Destroy()
	{
		HZR_PROFILE_FUNCTION();
		auto device = m_Device->GetVulkanDevice();
		vkDeviceWaitIdle(device);

		if (m_Swapchain)
			vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
		for (auto& image : m_Images)
			vkDestroyImageView(device, image.ImageView, nullptr);
		for (auto& buffer : m_CommandBuffers)
			vkDestroyCommandPool(device, buffer.CommandPool, nullptr);
		if (m_RenderPass)
			vkDestroyRenderPass(device, m_RenderPass, nullptr);

		for (auto framebuffer : m_FrameBuffers)
			vkDestroyFramebuffer(device, framebuffer, nullptr);

		if (m_Semaphores.RenderComplete)
			vkDestroySemaphore(device, m_Semaphores.RenderComplete, nullptr);

		if (m_Semaphores.PresentComplete)
			vkDestroySemaphore(device, m_Semaphores.PresentComplete, nullptr);

		for (auto& fence : m_WaitFences)
			vkDestroyFence(device, fence, nullptr);

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);

		vkDeviceWaitIdle(device);
	}
	void VulkanSwapchain::Resize(uint32_t width, uint32_t height)
	{
		HZR_PROFILE_FUNCTION();
		vkDeviceWaitIdle(m_Device->GetVulkanDevice());
		Create(&width, &height, m_VSync);
		vkDeviceWaitIdle(m_Device->GetVulkanDevice());
	}
	void VulkanSwapchain::BeginFrame()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();
		m_CurrentImageIndex = AcquireSwapchainImage();
		VK_CHECK_RESULT(vkResetCommandPool(m_Device->GetVulkanDevice(), m_CommandBuffers[m_CurrentBufferIndex].CommandPool, 0), "Failed to reset command pool");

		m_RenderCommandBuffer->Begin();
	}
	void VulkanSwapchain::Present()
	{
		HZR_PROFILE_FUNCTION();
		HZR_TIMED_FUNCTION();
		m_RenderCommandBuffer->End();
		const uint64_t DEFAULT_TIMEOUT = 100000000000;

		auto vkDevice = m_Device->GetVulkanDevice();

		VkPipelineStageFlags pipelineFlags = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.pWaitDstStageMask = &pipelineFlags;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = &m_Semaphores.PresentComplete;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = &m_Semaphores.RenderComplete;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers[m_CurrentBufferIndex].CommandBuffer;

		VK_CHECK_RESULT(vkResetFences(vkDevice, 1, &m_WaitFences[m_CurrentBufferIndex]), "Failed to reset fence");
		VK_CHECK_RESULT(vkQueueSubmit(m_Device->GetGraphicsQueue(), 1, &submitInfo, m_WaitFences[m_CurrentBufferIndex]), "Failed to submit");

		VkResult result;
		{
			VkPresentInfoKHR presentInfo = {};
			presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			presentInfo.swapchainCount = 1;
			presentInfo.pSwapchains = &m_Swapchain;
			presentInfo.pImageIndices = &m_CurrentImageIndex;
			presentInfo.waitSemaphoreCount = 1;
			presentInfo.pWaitSemaphores = &m_Semaphores.RenderComplete;

			result = fpQueuePresentKHR(m_Device->GetGraphicsQueue(), &presentInfo);
		}

		if (result != VK_SUCCESS)
		{
			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
				Resize(m_Width, m_Height);
		}
		//Wait for fence
		{
			HZR_PROFILE_SCOPE("VulkanSwapchain::Present() WaitForFences");
			const auto& imageCount = VulkanContext::GetImagesInFlight();
			m_CurrentBufferIndex = (m_CurrentBufferIndex + 1) % imageCount;
			VK_CHECK_RESULT(vkWaitForFences(vkDevice, 1, &m_WaitFences[m_CurrentBufferIndex], VK_TRUE, UINT64_MAX), "");
		}
	}
	void VulkanSwapchain::FindImageFormatAndColorSpace()
	{
		HZR_PROFILE_FUNCTION();
		VkPhysicalDevice physicalDevice = m_Device->GetPhysicalDevice().As<VulkanPhysicalDevice>()->GetVulkanPhysicalDevice();

		uint32_t formatCount = 0;
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, nullptr), "Failed to get formats");
		HZR_ASSERT(formatCount > 0, "Failed to get formats");

		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		VK_CHECK_RESULT(fpGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_Surface, &formatCount, surfaceFormats.data()), "Failed to get formats");

		if ((formatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		{
			m_ColorFormat = VK_FORMAT_B8G8R8A8_UNORM;
			m_ColorSpace = surfaceFormats[0].colorSpace;
		}
		else
		{
			bool found_BGRA8_UNORM = false;
			for (auto&& surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
				{
					m_ColorFormat = surfaceFormat.format;
					m_ColorSpace = surfaceFormat.colorSpace;
					found_BGRA8_UNORM = true;
					break;
				}
			}
			if (!found_BGRA8_UNORM)
			{
				m_ColorFormat = surfaceFormats[0].format;
				m_ColorSpace = surfaceFormats[0].colorSpace;
			}
		}
	}
	uint32_t VulkanSwapchain::AcquireSwapchainImage()
	{
		HZR_PROFILE_FUNCTION();
		uint32_t index;
		VK_CHECK_RESULT(fpAcquireNextImageKHR(m_Device->GetVulkanDevice(), m_Swapchain, UINT64_MAX, m_Semaphores.PresentComplete, (VkFence)nullptr, &index), "Failed to Acquire next image");
		return index;
	}
}
#endif
