#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "Platform/Vulkan/imgui_impl_vulkan.h"

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, Hazard::Rendering::Vulkan::VulkanContext* context);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

private:
	Hazard::Rendering::Vulkan::VulkanContext* m_Context = nullptr;
};
