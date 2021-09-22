#pragma once
#include "hzreditor.h"
#include "EditorPlatformVulkan.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

using namespace Hazard::Rendering::Vulkan;
EditorPlatformVulkan::EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context)
{
	this->m_Context = context;

	VkDescriptorPoolSize pool_sizes[] =
	{
		{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
		{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
		{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
		{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
	};
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
	pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
	pool_info.pPoolSizes = pool_sizes;
	VkResult result = vkCreateDescriptorPool(context->GetDevice()->GetDevice(), &pool_info, nullptr, &m_DescriptorPool);


	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = context->GetInstance()->GetInstance();
	init_info.PhysicalDevice = context->GetDevice()->GetPhysicalDevice();
	init_info.Device = context->GetDevice()->GetDevice();
	init_info.QueueFamily = VKUtils::GetQueueFamilyIndices(context->GetDevice()->GetPhysicalDevice(),
		context->GetDevice()->GetDeviceSurface()->GetVkSurface()).graphicsFamily.value();
	init_info.Queue = context->GetDevice()->GetGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_DescriptorPool;
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = context->GetDevice()->GetSwapChain().GetImageCount();
	init_info.CheckVkResultFn = nullptr;
	ImGui_ImplVulkan_Init(&init_info, context->GetDevice()->GetSwapChain().GetRenderPass()->GetRenderPass());

	VkCommandBuffer singleBuffer = context->GetDevice()->BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(singleBuffer);
	context->GetDevice()->EndSingleTimeCommands(singleBuffer);
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
	ImGuiIO& io = ImGui::GetIO();

	CommandBuffer& buffer = m_Context->GetDevice()->GetSwapChain().GetCurrentBuffer();
	buffer.Begin();

	FrameBuffer* frameBuffer = m_Context->GetDevice()->GetSwapChain().GetCurrentFrameBuffer();
	m_Context->GetDevice()->GetSwapChain().GetRenderPass()->Begin(&buffer, frameBuffer->GetBuffer(), frameBuffer->GetExtent());

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *buffer.GetBuffer());
	m_Context->GetDevice()->GetSwapChain().GetRenderPass()->End(&buffer);

	buffer.End();
	m_Context->Submit(&buffer);

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
void EditorPlatformVulkan::FrameRender()
{

}
void EditorPlatformVulkan::FramePresent()
{

}
