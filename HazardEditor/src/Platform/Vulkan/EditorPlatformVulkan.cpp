#pragma once
#include "hzreditor.h"
#include "EditorPlatformVulkan.h"

#include <imgui_impl_vulkan.h>
#include "imgui_impl_glfw.h"

#if 0

using namespace Hazard::Rendering::Vulkan;

EditorPlatformVulkan::EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context)
{
	this->m_Context = context;
	auto& swapchain = m_Context->GetDevice()->GetSwapChain();
	auto device = m_Context->GetDevice();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForVulkan(window, true);
	ImGui_ImplVulkan_InitInfo init_info = {};
	init_info.Instance = context->GetInstance()->GetInstance();
	init_info.PhysicalDevice = device->GetPhysicalDevice();
	init_info.Device = device->GetDevice();
	init_info.QueueFamily = VKUtils::GetQueueFamilyIndices(device->GetPhysicalDevice(),
		device->GetDeviceSurface()->GetVkSurface()).graphicsFamily.value();
	init_info.Queue = device->GetGraphicsQueue();
	init_info.PipelineCache = VK_NULL_HANDLE;
	init_info.DescriptorPool = m_Context->GetDevice()->GetDescriptorPool();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = swapchain.GetImageCount();
	init_info.CheckVkResultFn = nullptr;

	ImGui_ImplVulkan_Init(&init_info, swapchain.GetRenderPass()->GetRenderPass());

	VkCommandBuffer singleBuffer = device->BeginSingleTimeCommands();
	ImGui_ImplVulkan_CreateFontsTexture(singleBuffer);
	device->EndSingleTimeCommands(singleBuffer);
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
	CommandBuffer& buffer = m_Context->GetDevice()->GetSwapChain().GetCurrentBuffer();
	buffer.Begin();
}

void EditorPlatformVulkan::EndFrame()
{
	ImGuiIO& io = ImGui::GetIO();

	auto& swapChain = m_Context->GetDevice()->GetSwapChain();

	CommandBuffer& buffer = swapChain.GetCurrentBuffer();
	VulkanFrameBuffer* frameBuffer = swapChain.GetCurrentFrameBuffer();
	swapChain.GetRenderPass()->Begin(&buffer, frameBuffer->GetBuffer(), frameBuffer->GetExtent());

	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), *buffer.GetBuffer());
	swapChain.GetRenderPass()->End(&buffer);

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
void EditorPlatformVulkan::AddTexture(Ref<Rendering::Texture> texture)
{
	if (dynamic_cast<Rendering::Vulkan::VulkanTexture2D*>(texture.Raw())) {
		Rendering::Vulkan::VulkanTexture2D* vt = (Rendering::Vulkan::VulkanTexture2D*)texture.Raw();
		vt->SetID(ImGui_ImplVulkan_AddTexture(vt->GetData().Sampler, vt->GetData().ImageView, vt->GetData().Layout));
		return;
	}
	Rendering::Vulkan::VulkanFrameBuffer* vt = (Rendering::Vulkan::VulkanFrameBuffer*)texture.Raw();
	vt->SetID(ImGui_ImplVulkan_AddTexture(vt->GetData(0).Sampler, vt->GetData(0).ImageView, vt->GetData(0).Layout));
}
#endif
