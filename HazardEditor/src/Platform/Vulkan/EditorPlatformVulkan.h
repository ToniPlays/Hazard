#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"

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
