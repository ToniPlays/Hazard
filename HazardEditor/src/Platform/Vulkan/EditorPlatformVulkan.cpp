#pragma once
#include "EditorPlatformVulkan.h"
#include "Hazard/RenderContext/RenderContextCommand.h"

#include <Platform/GLFW/imgui_impl_vulkan.h>
#include <Platform/GLFW/imgui_impl_glfw.h>

using namespace Hazard::Rendering::Vulkan;

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
	init_info.DescriptorPool = m_Context->GetDevice()->GetDescriptorPool();
	init_info.Allocator = nullptr;
	init_info.MinImageCount = 2;
	init_info.ImageCount = swapchain->GetImageCount();
	init_info.CheckVkResultFn = nullptr;
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

	Rendering::RenderCommand::Submit([=]() mutable {
	
		});

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
void EditorPlatformVulkan::AddTexture(Ref<Rendering::Image> image)
{
	if (dynamic_cast<Rendering::Vulkan::VulkanImage2D*>(image.Raw())) {
		Rendering::Vulkan::VulkanImage2D* vt = (Rendering::Vulkan::VulkanImage2D*)image.Raw();
		//vt->SetID(ImGui_ImplVulkan_AddTexture(vt->GetData().Sampler, vt->GetData().ImageView, vt->GetData().Layout));
		return;
	}
	//Rendering::Vulkan::VulkanFrameBuffer* vt = (Rendering::Vulkan::VulkanFrameBuffer*)texture.Raw();
	//vt->SetID(ImGui_ImplVulkan_AddTexture(vt->GetData(0).Sampler, vt->GetData(0).ImageView, vt->GetData(0).Layout));
}
