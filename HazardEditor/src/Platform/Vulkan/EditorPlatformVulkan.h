#pragma once

#include <hzrpch.h>
#include "../EditorGuiPlatform.h"
#include "Platform/Vulkan/imgui_impl_vulkan.h"
#include "Backend/Vulkan/VulkanContext.h"

using namespace HazardRenderer::Vulkan;
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
