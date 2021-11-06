#pragma once

#include "Hazard.h"
#include "../EditorGuiPlatform.h"

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, Hazard::Rendering::Vulkan::VulkanContext* context);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

	void AddTexture(Ref<Rendering::Image> image);

private:
	Hazard::Rendering::Vulkan::VulkanContext* m_Context = nullptr;
};
