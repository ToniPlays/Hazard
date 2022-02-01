#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "Platform/Vulkan/imgui_impl_vulkan.h"

using namespace Hazard::Rendering::Vulkan;

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, VulkanContext* context);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

private:
	VulkanContext* m_Context = nullptr;
};
