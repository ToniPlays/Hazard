
#include "Hazard.h"

#include "EditorPlatformVulkan.h"

#ifdef HZR_INCLUDE_VULKAN

#include "Core/Renderer.h"
#include "Vulkan/VKUtils.h"
#include "imgui.h"
#include <../ImGui_Backend/imgui_impl_glfw.h>

using namespace HazardRenderer;
static std::vector<VkCommandBuffer> s_ImGuiCommandBuffers;

EditorPlatformVulkan::EditorPlatformVulkan(HazardRenderer::Window& window)
{
	m_Window = &window;
	m_Context = (VulkanContext*)window.GetContext();
	Ref<VulkanSwapchain> swapchain = m_Context->GetSwapchain().As<VulkanSwapchain>();
	auto physicalDevice = m_Context->GetDevice().As<VulkanPhysicalDevice>();
	auto device = m_Context->GetLogicalDevice();
	//Create descriptor pools

	VkDescriptorPool descriptorPool;

	VkDescriptorPoolSize poolSizes[] = {
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 100 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 100 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 100 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 100 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 100 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 100 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 100 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 100 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 100 },
	};
	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	poolInfo.maxSets = 100 * IM_ARRAYSIZE(poolSizes);
	poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
	poolInfo.pPoolSizes = poolSizes;

	VK_CHECK_RESULT(vkCreateDescriptorPool(device->GetVulkanDevice(), &poolInfo, nullptr, &descriptorPool), "Failed to create descriptor pool");

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)window.GetNativeWindow(), true);

	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = m_Context->GetVulkanInstance();
	init_info.PhysicalDevice = physicalDevice->GetVulkanPhysicalDevice();
	init_info.Device = device->GetVulkanDevice();
	init_info.QueueFamily = physicalDevice->GetQueueFamilyIndices().Graphics;
	init_info.Queue = device->GetGraphicsQueue();
	init_info.PipelineCache = m_Context->GetPipelineCache();;
	init_info.DescriptorPool = descriptorPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = m_Context->GetImagesInFlight();
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info, swapchain->GetVulkanRenderPass());
	VkCommandBuffer cmdBuffer = device->GetCommandBuffer(true);

	ImGui_ImplVulkan_CreateFontsTexture(cmdBuffer);
	device->FlushCommandBuffer(cmdBuffer);

	vkDeviceWaitIdle(device->GetVulkanDevice());
	ImGui_ImplVulkan_DestroyFontUploadObjects();

	uint32_t framesInFlight = m_Context->GetImagesInFlight();
	s_ImGuiCommandBuffers.resize(framesInFlight);

	for (uint32_t i = 0; i < framesInFlight; i++)
		s_ImGuiCommandBuffers[i] = device->CreateSecondaryCommandBuffer("EditorPlatformVulkan secondary CommandBuffer");
}

EditorPlatformVulkan::~EditorPlatformVulkan()
{
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

	HZR_PROFILE_FUNCTION("EditorPlatformVulkan::EndFrame() RT");

	auto swapchain = context->GetSwapchain().As<VulkanSwapchain>();
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
	renderPassBeginInfo.renderPass = swapchain->GetVulkanRenderPass();
	renderPassBeginInfo.renderArea.offset.x = 0;
	renderPassBeginInfo.renderArea.offset.y = 0;
	renderPassBeginInfo.renderArea.extent.width = width;
	renderPassBeginInfo.renderArea.extent.height = height;
	renderPassBeginInfo.clearValueCount = 2; // Color + depth
	renderPassBeginInfo.pClearValues = clearValues;
	renderPassBeginInfo.framebuffer = swapchain->GetCurrentFramebuffer();

	vkCmdBeginRenderPass(drawCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS);

	VkCommandBufferInheritanceInfo inheritanceInfo = {};
	inheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
	inheritanceInfo.renderPass = swapchain->GetVulkanRenderPass();
	inheritanceInfo.framebuffer = swapchain->GetCurrentFramebuffer();

	VkCommandBufferBeginInfo cmdBufInfo = {};
	cmdBufInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	cmdBufInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT;
	cmdBufInfo.pInheritanceInfo = &inheritanceInfo;

	VK_CHECK_RESULT(vkBeginCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex], &cmdBufInfo), "");

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

	VK_CHECK_RESULT(vkEndCommandBuffer(s_ImGuiCommandBuffers[commandBufferIndex]), "");

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
}

void EditorPlatformVulkan::Close()
{
	ImGui_ImplGlfw_Shutdown();
}
#endif
