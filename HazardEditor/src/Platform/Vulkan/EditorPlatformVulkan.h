#pragma once

#include "Hazard.h"
#include "../EditorGuiPlatform.h"

class EditorPlatformVulkan : public EditorGuiPlatform {
public:

	EditorPlatformVulkan(GLFWwindow* window, Rendering::Vulkan::VKContext* context);
	~EditorPlatformVulkan();

	void BeginFrame() override;
	void EndFrame() override;
	void Close() override;

	void AddTexture(Hazard::Rendering::Texture* texture);

private:
	void FrameRender();
	void FramePresent();
private:

	Rendering::Vulkan::VKContext* m_Context = nullptr;
};