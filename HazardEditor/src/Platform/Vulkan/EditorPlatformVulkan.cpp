#pragma once
#include "EditorPlatformVulkan.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

#include <Platform/GLFW/imgui_impl_vulkan.h>
#include <Platform/GLFW/imgui_impl_glfw.h>

using namespace Hazard::Rendering::Vulkan;

static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;

EditorPlatformVulkan::EditorPlatformVulkan(GLFWwindow* window, VulkanContext* context)
{
	this->m_Context = context;
	auto& swapchain = m_Context->GetSwapchain();
	auto device = m_Context->GetDevice();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan(window, true);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = context->GetInstance();
	init_info.PhysicalDevice = device->GetPhysicalDevice();
	init_info.Device = device->GetDevice();
	init_info.QueueFamily = VKUtils::GetQueueFamilyIndices(device->GetPhysicalDevice(),
		context->GetSurface()).graphicsFamily.value();
	init_info.Queue = device->GetGraphicsQueue().Queue;
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_Context->GetDevice()->GetDescriptorPool(0);
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = swapchain->GetImageCount();
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info, swapchain->GetRenderPass());
	VkCommandBuffer cmdBuffer = device->GetCommandBuffer(true);

	ImGui_ImplVulkan_CreateFontsTexture(cmdBuffer);
	device->FlushGraphicsCommandBuffer(cmdBuffer);

	device->WaitUntilIdle();
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	uint32_t framesInFlight = Hazard::Rendering::RenderContextCommand::GetImagesInFlight();
	s_ImGuiCommandBuffers.resize(framesInFlight);

	for (uint32_t i = 0; i < framesInFlight; i++)
		s_ImGuiCommandBuffers[i] = VulkanContext::GetDevice()->CreateSecondaryCommandBuffer();
}


EditorPlatformVulkan::~EditorPlatformVulkan()
{
	m_Context->GetDevice()->WaitUntilIdle();
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
}

void EditorPlatformVulkan::BeginFrame()
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
}

void EditorPlatformVulkan::EndFrame()
{
	auto& swapchain = m_Context->GetSwapchain();
	ImGuiIO& io = ImGui::GetIO();

	VkClearValue clearValues[2];
	clearValues[0].color = { {0.1f, 0.1f,0.1f, 1.0f} };
	clearValues[1].depthStencil = { 1.0f, 0 };

	uint32_t width = swapchain->GetWidth();
	uint32_t height = swapchain->GetHeight();

	uint32_t commandBufferIndex = swapchain->GetCurrentBufferIndex();

	VkCommandBufferBeginInfo drawCmdBufInfo = {};
	drawCmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	drawCmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	drawCmdBufInfo.pNext = nullptr;

	VkCommandBuffer drawCommandBuffer = swapchain->GetCurrentDrawCommandBuffer();
	VK_CHECK_RESULT(vkBeginCommandBuffer(drawCommandBuffer, &drawCmdBufInfo));

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.pNext = nullptr;
	renderPassBeginInfo.renderPass = swapchain->GetRenderPass();
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
	inheritanceInfo.renderPass = swapchain->GetRenderPass();
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

	VK_CHECK_RESULT(vkEndCommandBuffer(drawCommandBuffer));

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void EditorPlatformVulkan::Close()
{

}
