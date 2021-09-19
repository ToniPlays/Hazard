#pragma once
#include "hzreditor.h"
#include "EditorPlatformVulkan.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

EditorPlatformVulkan::EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context)
{
	using namespace Hazard::Rendering::Vulkan;

	ImGui_ImplGlfw_InitForVulkan(window, true);

	VKInstanceData data = context->GetData().vkInstance->GetData();
	
	ImGui_ImplVulkan_InitInfo createInfo = {};
	createInfo.Instance = data.Instance;
	createInfo.PhysicalDevice = data.Device->GetPhysicalDevice();
	createInfo.Device = data.Device->GetDevice();
	createInfo.Queue = data.Device->GetGraphicsQueue();
	createInfo.DescriptorPool = data.DescriptorPool->GetPool();
	createInfo.Allocator = nullptr;
	createInfo.CheckVkResultFn = nullptr;
	createInfo.PipelineCache = data.Cache;
	createInfo.MinImageCount = 2;
	createInfo.ImageCount = 2;
	
	ImGui_ImplVulkan_Init(&createInfo, data.RenderPass->GetRenderPass());
	ImGui_ImplVulkan_CreateFontsTexture(data.Buffer->GetBuffer());

	this->context = context;
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
}

void EditorPlatformVulkan::EndFrame()
{
	using namespace Hazard::Rendering::Vulkan;
	VKInstanceData data = context->GetData().vkInstance->GetData();
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), data.Buffer->GetBuffer());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}
