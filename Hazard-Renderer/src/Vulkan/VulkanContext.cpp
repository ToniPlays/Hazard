
#include "VulkanContext.h"
#include "VKUtils.h"

namespace HazardRenderer::Vulkan {

	VulkanContext::VulkanContext(WindowProps* props)
	{
		if (!glfwVulkanSupported()) {
			("Vulkan not supported");
		}
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	}

	VulkanContext::~VulkanContext()
	{
//		m_Device->WaitUntilIdle();
//		s_Callback = nullptr;
//
//		vkDestroyPipelineCache(m_Device->GetDevice(), m_PipelineCache, nullptr);
//
//		m_SwapChain.Reset();
//		m_WindowSurface.reset();
//
//		m_Device.reset();
//		VulkanAllocator::Shutdown();
//
//		vkDestroyInstance(m_Instance, nullptr);
	}

	void VulkanContext::Init(Window* window, HazardRendererCreateInfo* info)
	{
		m_Window = window;

		auto extensions = VKUtils::GetRequiredExtensions(info->Logging);

		VkApplicationInfo vkApp = {};
		vkApp.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vkApp.pApplicationName = "Hazard";
		vkApp.pEngineName = "Hazard";
		vkApp.apiVersion = VK_API_VERSION_1_2;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.enabledExtensionCount = (uint32_t)extensions.size();
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.pApplicationInfo = &vkApp;

		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};

		if (ValidationLayer::IsValidationSupported()) {
			ValidationLayer::InitValidationLayers(createInfo, debugInfo, info->Logging);
		}
//		vkCreateInstance(&createInfo, nullptr, &m_Instance);

		if (info->Logging) {
			ValidationLayer::SetupDebugger(m_Instance);
		}
//
//		m_WindowSurface = CreateScope<WindowSurface>(m_Instance, (GLFWwindow*)m_Window->GetNativeWindow());
//		m_Device = CreateScope<VulkanDevice>(m_Instance, m_WindowSurface->GetVkSurface(), RenderContextCommand::GetImagesInFlight());
//		m_PhysicalDevice = new PhysicalDevice();
//
//		VulkanAllocator::Init();
//
//		uint32_t w = window->GetWidth();
//		uint32_t h = window->GetHeight();
//
//		m_SwapChain = Ref<VulkanSwapChain>::Create();
//		m_SwapChain->Create(w, h, window->IsVSync());
	}

	void VulkanContext::BeginFrame()
	{
//		m_SwapChain->BeginFrame();
	}
//
	void VulkanContext::Present()
	{
//		m_SwapChain->Present();
	}

//	void VulkanContext::Close()
//	{
//		m_Device->WaitUntilIdle();
//	}
//
//	void VulkanContext::BeginRenderPass(Ref<RenderCommandBuffer> buffer, Ref<RenderPass> renderPass)
//	{
//		HZR_PROFILE_FUNCTION();
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//
//		auto fb = renderPass->GetSpecs().TargetFrameBuffer.As<VulkanFrameBuffer>();
//
//		uint32_t width = fb->GetWidth();
//		uint32_t height = fb->GetHeight();
//
//		VkViewport viewport = {};
//		viewport.minDepth = 0.0f;
//		viewport.maxDepth = 1.0f;
//		viewport.width = (float)width;
//		viewport.height = (float)height;
//
//		VkRenderPassBeginInfo renderPassBeginInfo = {};
//		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//		renderPassBeginInfo.renderPass = fb->GetRenderPass();
//		renderPassBeginInfo.renderArea.offset.x = 0;
//		renderPassBeginInfo.renderArea.offset.y = 0;
//		renderPassBeginInfo.renderArea.extent.width = width;
//		renderPassBeginInfo.renderArea.extent.height = height;
//
//		if (fb->GetSpecification().SwapChainTarget) {
//			Ref<VulkanSwapChain> swapchain = VulkanContext::GetSwapchain();
//
//			width = swapchain->GetWidth();
//			height = swapchain->GetHeight();
//			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//			renderPassBeginInfo.renderPass = fb->GetRenderPass();
//			renderPassBeginInfo.renderArea.offset.x = 0;
//			renderPassBeginInfo.renderArea.offset.y = 0;
//			renderPassBeginInfo.renderArea.extent.width = width;
//			renderPassBeginInfo.renderArea.extent.height = height;
//			renderPassBeginInfo.framebuffer = swapchain->GetCurrentFrameBuffer();
//
//			viewport.x = 0.0f;
//			viewport.y = (float)height;
//			viewport.width = (float)width;
//			viewport.height = -(float)height;
//		}
//		else
//		{
//			width = fb->GetWidth();
//			height = fb->GetHeight();
//			renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
//			renderPassBeginInfo.renderPass = fb->GetRenderPass();
//			renderPassBeginInfo.renderArea.offset.x = 0;
//			renderPassBeginInfo.renderArea.offset.y = 0;
//			renderPassBeginInfo.renderArea.extent.width = width;
//			renderPassBeginInfo.renderArea.extent.height = height;
//			renderPassBeginInfo.framebuffer = fb->GetFrameBuffer();
//
//			viewport.x = 0.0f;
//			viewport.y = 0.0f;
//			viewport.width = (float)width;
//			viewport.height = (float)height;
//		}
//
//		const auto& clearValues = fb->GetVulkanClearValues();
//		renderPassBeginInfo.clearValueCount = (uint32_t)clearValues.size();
//		renderPassBeginInfo.pClearValues = clearValues.data();
//
//		VkRect2D scissors = {};
//		scissors.extent.width = width;
//		scissors.extent.height = height;
//		scissors.offset = { 0, 0 };
//
//		vkCmdBeginRenderPass(vkBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
// 		vkCmdSetViewport(vkBuffer, 0, 1, &viewport);
//		vkCmdSetScissor(vkBuffer, 0, 1, &scissors);
//	}
//
//	void VulkanContext::EndRenderPass(Ref<RenderCommandBuffer> buffer)
//	{
//		HZR_PROFILE_FUNCTION();
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//		vkCmdEndRenderPass(vkBuffer);
//	}
//	void VulkanContext::SetLineWidth(Ref<RenderCommandBuffer> buffer, float lineWidth)
//	{
//		uint32_t frameIndex = VulkanContext::GetSwapchain()->GetCurrentBufferIndex();
//		VkCommandBuffer vkBuffer = buffer.As<VulkanRenderCommandBuffer>()->GetBuffer(frameIndex);
//
//		vkCmdSetLineWidth(vkBuffer, lineWidth);
//	}
//
//	void VulkanContext::SetViewport(int x, int y, int w, int h)
//	{
//		if (w == 0 || h == 0) return;
//
//		auto device = m_Device->GetDevice();
//		m_Device->WaitUntilIdle();
//
//		uint32_t width = w;
//		uint32_t height = h;
//
//		m_SwapChain->Resize(width, height);
//	}
//
//	void VulkanContext::SetErrorListener(const ErrorCallback& callback)
//	{
//		s_Callback = callback;
//	}
//
//	PhysicalDevice& VulkanContext::GetPhysicalDevice() const
//	{
//		return *m_PhysicalDevice;
//	}
//	void VulkanContext::SendDebugMessage(const char* message, const char* code)
//	{
//		std::cout << message << std::endl;
//		if (s_Callback != nullptr) {
//			ErrorData data(message, code);
//			s_Callback(data);
//		}
//	}
//	VkDescriptorSet VulkanContext::RT_AllocateDescriptorSet(VkDescriptorSetAllocateInfo& allocInfo) 
//	{
//		uint32_t bufferIndex = m_SwapChain->GetCurrentBufferIndex();
//		allocInfo.descriptorPool = m_Device->GetDescriptorPool(bufferIndex);
//		VkDescriptorSet result;
//		VK_CHECK_RESULT(vkAllocateDescriptorSets(m_Device->GetDevice(), &allocInfo, &result));
//		m_DescriptorAllocations[bufferIndex] += allocInfo.descriptorSetCount;
//		return result;
//	}
}