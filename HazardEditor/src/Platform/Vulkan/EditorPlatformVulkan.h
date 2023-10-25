#pragma once

#include <hzrpch.h>
#ifdef HZR_INCLUDE_VULKAN

#include "../EditorGuiPlatform.h"
#include "../ImGui_Backend/imgui_impl_vulkan.h"
#include "Vulkan/VulkanContext.h"

using namespace HazardRenderer::Vulkan;

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(HazardRenderer::Window& window);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

private:
	HazardRenderer::Window* m_Window;
	VulkanContext* m_Context = nullptr;
};
#endif
