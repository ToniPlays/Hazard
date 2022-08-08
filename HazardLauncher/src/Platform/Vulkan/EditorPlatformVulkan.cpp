
#include "Hazard.h"

#ifdef HZR_INCLUDE_VULKAN
#include "EditorPlatformVulkan.h"
#include "Backend/Core/Renderer.h"
#include "Backend/Vulkan/VKUtils.h"
#include "imgui.h"
#include <../ImGui_Backend/imgui_impl_glfw.h>

static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;

EditorPlatformVulkan::EditorPlatformVulkan(HazardRenderer::Window& window)
{
	m_Context = (VulkanContext*)window.GetContext();
	Ref<VulkanSwapchain> swapchain = m_Context->GetSwapchain().As<VulkanSwapchain>();
	auto& device = (VulkanDevice&)m_Context->GetDevice();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window.GetNativeWindow(), true);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_Context->GetVulkanInstance();
	init_info.PhysicalDevice = device.GetVulkanPhysicalDevice();
	init_info.Device = device.GetVulkanDevice();
	init_info.QueueFamily = VKUtils::GetQueueFamilyIndices(device.GetVulkanPhysicalDevice(),
		m_Context->GetWindowSurface()).graphicsFamily.value();
	init_info.Queue = device.GetGraphicsQueue().Queue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = device.GetDescriptorPool(0);
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = swapchain->GetImageCount();
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info, swapchain->GetVulkanRenderPass());
	VkCommandBuffer cmdBuffer = device.GetCommandBuffer(true);

	ImGui_ImplVulkan_CreateFontsTexture(cmdBuffer);
	device.FlushGraphicsCommandBuffer(cmdBuffer);

	device.WaitUntilIdle();
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	uint32_t framesInFlight = swapchain->GetImageCount();
	s_ImGuiCommandBuffers.resize(framesInFlight);

	for (uint32_t i = 0; i < framesInFlight; i++)
		s_ImGuiCommandBuffers[i] = device.CreateSecondaryCommandBuffer();
}


EditorPlatformVulkan::~EditorPlatformVulkan()
{
	m_Context->GetPhysicalDevice().WaitUntilIdle();
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void EditorPlatformVulkan::BeginFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void EditorPlatformVulkan::EndFrame()
{
	VulkanContext* context = m_Context;

	HazardRenderer::Renderer::Submit([context]() mutable {

		HZR_PROFILE_FUNCTION("EditorPlatformVulkan::EndFrame() RT");
		auto& swapchain = context->GetSwapchain().As<VulkanSwapchain>();
		ImGuiIO& io = ImGui::GetIO();

		glm::vec4 color = swapchain->GetRenderTarget()->GetSpecification().ClearColor;

		VkClearValue clearValues[2];
		clearValues[0].color = { color.r, color.g, color.b, color.a };
		clearValues[1].depthStencil = { 1.0f, 0 };

		uint32_t width = swapchain->GetWidth();
		uint32_t height = swapchain->GetHeight();

		uint32_t commandBufferIndex = swapchain->GetCurrentBufferIndex();
		VkCommandBuffer drawCommandBuffer = swapchain->GetCurrentDrawCommandBuffer();

		VkRenderPassBeginInfo renderPassBeginInfo = {};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = swapchain->GetVulkanRenderPass();
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = width;
		renderPassBeginInfo.renderArea.extent.height = height;
		renderPassBeginInfo.clearValueCount = 2; // Color + depth
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = swapchain->GetCurrentFrameBuffer();

		vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

		VkCommandBufferInheritanceInfo inheritanceInfo = {};
		inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		inheritanceInfo.renderPass = swapchain->GetVulkanRenderPass();
		inheritanceInfo.framebuffer = swapchain->GetCurrentFrameBuffer();

		VkCommandBufferBeginInfo cmdBufInfo = {};
		cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
		cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

		VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufInfo));

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = (float)height;
		viewport.height = -(float)height;
		viewport.width = (float)width;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &viewport);

		VkRect2D scissor = {};
		scissor.extent.width = width;
		scissor.extent.height = height;
		scissor.offset.x = 0;
		scissor.offset.y = 0;
		vkCmdSetScissor(s_ImGuiCommandBuffers[commandBufferIndex], 0, 1, &scissor);

		ImDrawData* main_draw_data = ImGui::GetDrawData();
		ImGui_ImplVulkan_RenderDrawData(main_draw_data, s_ImGuiCommandBuffers[commandBufferIndex]);

		VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]));

		std::vector<VkCommandBuffer> commandBuffers;
		commandBuffers.push_back(s_ImGuiCommandBuffers[commandBufferIndex]);

		vkCmdExecuteCommands(drawCommandBuffer, uint32_t(commandBuffers.size()), commandBuffers.data());
		vkCmdEndRenderPass(drawCommandBuffer);

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		});
}

void EditorPlatformVulkan::Close()
{

}
#endif
