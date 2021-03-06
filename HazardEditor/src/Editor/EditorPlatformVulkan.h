#pragma once

#include "Hazard.h"
#include "EditorGuiPlatform.h"

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context);
	~EditorPlatformVulkan();

	void BeginFrame();
	void EndFrame();

private:
	Rendering::GraphicsContext* context = nullptr;
};