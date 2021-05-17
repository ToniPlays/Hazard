#pragma once
#include "hzreditor.h"
#include "EditorPlatformVulkan.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

EditorPlatformVulkan::EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context)
{
	ImGui_ImplGlfw_InitForVulkan(window, true);

	using namespace Hazard::Rendering::Vulkan;

	VKInstanceData data = context->GetData().vkInstance->GetData();

	ImGui_ImplVulkan_InitInfo createInfo;
	createInfo.Instance = data.Instance;
	createInfo.PhysicalDevice = data.Device->GetPhysicalDevice();
	createInfo.Device = data.Device->GetDevice();



	createInfo.MinImageCount = 2;
	createInfo.ImageCount = 2;

	ImGui_ImplVulkan_Init(&createInfo, nullptr);
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
	ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), nullptr);
}
